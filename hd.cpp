#include "./hd.hpp"


#define APPEND        0x00 
#define NO_APPEND     0x01

//Scrive sul disco in due modalità append e non append
//nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void* const buff,size_t len,size_t mode = APPEND){
    u_int8_t n_sect = mode >> 4;
    if(!buff || n_sect >= NUM_SECT){
        printf("Parametri non valido\n");
        return false;
    }
    u_int8_t* buffer = (u_int8_t*)buff;
    if(!mode){
        for (int i = 0;i != DIM_TRACK;i++)
        {
            u_int8_t* track = &hd[n_sect].track[i];
            if(!len)
                return true;

            if(!(*track)){
                *track = buffer[i];
                len--;
            }
        }
    }else if(mode & NO_APPEND){
        for (int i = 0;i != DIM_TRACK;i++)
        {
            u_int8_t* track = &hd[n_sect].track[i];
            if(!len)
                return true;
            
            *track = buffer[i];
            len--;
            i++;    
        }
    }
    return false;
    
    
}

//legge un intero settore
bool readHD(void* buff,size_t len = DIM_TRACK,u_int64_t n_sect = 0){
    if(!buff || !len || n_sect >= NUM_SECT){
        printf("Parametri non validi\n");
        return false;
    }
    u_int8_t* dest = (u_int8_t*)buff;
    int i = 0;
    while(i != DIM_TRACK){
        if(!len)
            return true;
        len--;
        u_int8_t* track = &hd[n_sect].track[i];
        

        dest[i] = *track;
        i++;
    }
    return false;
    
}

#define BIT_START 1 << 5
#define BIT_READ  1 << 4 
#define BIT_WRITE 1 << 3 

#define ERR_READ        0x04
#define ERR_WRITE       0x03
#define ERR_MOD_UNKOWN  0x05 //modalità non supportata

void* inth(void*){
    if(int_hd.control & BIT_READ){
        if(!readHD(&int_hd.data.track,sizeof(sector)-1,int_hd.num_sect)){
            int_hd.status = ERR_READ;
            memset(&int_hd.data,0xff,sizeof(sector));
        }
    }else if(int_hd.control & BIT_WRITE){
        if(!writeHD(&int_hd.data.track,sizeof(sector)-1,(int_hd.num_sect << 4) | NO_APPEND)){
            int_hd.status = ERR_WRITE;
            memset(&int_hd.data,0xff,sizeof(sector));
        }
    }else{
        int_hd.status = ERR_MOD_UNKOWN;
        memset(&int_hd.data,0xff,sizeof(sector));
    }
    int_hd.status = EXIT_SUCCESS;
    pthread_exit(nullptr);
}


bool start_disp(){
    if(int_hd.control & BIT_START){
        //futura impplementazione dove nelle seguenti righe verrà lanciato un thread/o un processo pesante che si occupa di fare il trasfermento vero e proprio
        pthread_t processo_esterno;
        pthread_create(&processo_esterno,nullptr,&inth,nullptr);
        pthread_join(processo_esterno,nullptr);
        int_hd.control = 0;
        if(int_hd.status == ERR_READ){
            printf("[ERRORE]: Errore nella lettura\n");
            return false;
        }else if(int_hd.status == ERR_WRITE){
            printf("[ERRORE]: Errore nella scrittura\n");
            return false;
        }else if(int_hd.status == ERR_MOD_UNKOWN){
            printf("[ERRORE]: Modalità non supportata\n");
            return false;
        }else if(!int_hd.status){
            return true;
        }
    }
    return false;
}


bool saveHD(const char* const path = "./hd.txt"){
    FILE* fd;
    fd = fopen(path,"w");
    if(!fd){
        perror("Errore nell'apertura del file");
        return false;
    }

    printf("Salvataggio in corso");
    for (int i = 0; i < NUM_SECT; i++)
    {
        printf(".");
        for (int j = 0; j < DIM_TRACK; j++)
        {
            fprintf(fd,"%x ",hd[i].track[j]);
            
        }
        fprintf(fd,"\n");
        
    }
    printf("\nSalvataggio andato a buon fine\n");
    fclose(fd);
    return true;
}

bool loadHD(const char* const path = "./hd.txt"){
    FILE* fd;
    fd = fopen(path,"r");
    if(!fd){
        perror("Errore nell'apertura del file:");
        return false;
    }

    printf("Caricamento Disco");
    for (int i = 0; i < NUM_SECT; i++)
    {
        printf(".");
        for (int j = 0; j < DIM_TRACK; j++)
            fscanf(fd,"%hhx ",&hd[i].track[j]);
        
    }
    
    printf("\nCaricamento andato a buon fine\n");
    fclose(fd);
    return true;
}


//Modalità di stampa del disco
#define ALL_SECT    (const u_int8_t) 0x00 //Modalità che stampa tutto il disco
#define FIRST_SECT  (const u_int8_t) 0x01 //Modalità che stampa il primo settore
#define N_SECT      (const u_int8_t) 0x02 //modalità dove viene stampato un intero settore il numero di settore è nella parte alta della parola
#define LAST_SECT   (const u_int8_t) 0x04 //Modalità che stampa l'ultimo settore 

void printHD(const size_t mode = ALL_SECT){
    if(!mode){
        for (int i = 0; i < NUM_SECT; i++)
        {
            printf("[SECTOR %d]:\t",i);
            for (int j = 0; j < DIM_TRACK; j++){
                printf("%.2x ",hd[i].track[j]);
            }
            printf("\n");
        }
    }else if(mode & FIRST_SECT){ 
        
        printf("[SECTOR %d]:\t",0);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[0].track[i]);
            
        }
        printf("\n");

    }else if(mode & N_SECT){
        u_int8_t num_sect = (mode & 0XF0) >> 4;
        
        if(num_sect >= NUM_SECT){
            printf("Settore inesistente\n");
            return;
        }
        
        printf("[SECTOR %d]:\t",num_sect);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[num_sect].track[i]);
        }
        printf("\n");
    }else if(mode & LAST_SECT){
        
        printf("[SECTOR %d]:\t",NUM_SECT-1);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[NUM_SECT-1].track[i]);
            
        }
        printf("\n");
    
    }else
        printf("Modalità non supportata");

}

void writeTest(void){
    u_int8_t buffer[DIM_TRACK];
    memset(buffer,0xff,sizeof(buffer));
    for (int i = 0; i < NUM_SECT; i++)
    {
        writeHD(buffer,sizeof(buffer), ((i << 4) | NO_APPEND));
    }
}

#ifdef MAIN
    int main(void){
        memset((void *)&hd,0,sizeof(hd));
        //test
        //stampo tutto il disco
        printHD();
        printf("\n");
        //stampo il primo settore
        printHD(FIRST_SECT);
        printf("\n");
        //stampo il settore numero 5
        {
            u_int8_t tmp = (5 << 4);
            printf("Prima di scrivere nel settore %d\n",tmp>>4);
            printHD(tmp | N_SECT);
            printf("\n");
            
            printf("Dopo di scrivere nel settore %d\n",tmp>>4);
            char* a = "Hello World";
            if(!writeHD(a,strlen(a),tmp | NO_APPEND))
                printf("Scrittura non riuscita\n");
            else
                printHD(tmp | N_SECT);
            char* b = new char[strlen(a)+1];
            if(!readHD(b,strlen(a),tmp >> 4))
                printf("Lettura non riuscita\n");
            else
                printf("Lettura riuscita: %s\n",b);
            printf("\n");
        }
        //stampo l'ultimo settore
        printHD(LAST_SECT);
        printf("\n");

        

        //salvo il disco
        if(!saveHD())
            printf("Salvataggio fallito\n");
        else
            printHD();
        
        //Sporco il disco
        printf("\n");
        writeTest();
        printHD();

        //carico il vecchio disco
        if(!loadHD())
            printf("Caricamento fallito\n");
        else
            printHD();
        
        printf("Trasfermento mediante interfaccia\n");
        int_hd.num_sect = 5;
        int_hd.control |= BIT_START | BIT_READ;
        memset(&int_hd.data,0,sizeof(sector));
        if(!start_disp())
            return EXIT_FAILURE;
        
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",int_hd.data.track[i]);
        }
        printf("\n");
        
        printf("Scrittura mediante dispositivo\n");
        char* buf = "prova di scrittura tramite interfaccia\n";
        int_hd.control |= BIT_START | BIT_WRITE;
        memset(&int_hd.data,0,sizeof(sector));
        memcpy(&int_hd.data,buf,strlen(buf)+1);
        int_hd.num_sect = 1;
        start_disp();
        printHD();





    }
#endif
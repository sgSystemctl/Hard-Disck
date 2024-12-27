#include "./hd.hpp"

//Scrive sul disco in due modalità append e non append
//nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void* const buff,size_t len,size_t mode){
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
bool readHD(void* buff,size_t len,u_int64_t n_sect){
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
    if(!(int_hd.control & BIT_START)){
        //futura impplementazione dove nelle seguenti righe verrà lanciato un thread/o un processo pesante che si occupa di fare il trasfermento vero e proprio
        pthread_t processo_esterno;
        
        #ifdef DEBUG
            printf("[DEBUG]:Creazione processo esterno:");
        #endif

        pthread_create(&processo_esterno,nullptr,&inth,nullptr);
        
        #ifdef DEBUG
            printf("[DEBUG]:OK\n");
        #endif

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


bool saveHD(const char* const path){
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

bool loadHD(const char* const path){
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
            if(fscanf(fd,"%hhx ",&hd[i].track[j]) == -1){
                printf("Errore nella lettura del file %s",path);
                return false;
            
            }
        
    }
    
    printf("\nCaricamento andato a buon fine\n");
    fclose(fd);
    return true;
}

void printHD(const size_t mode){
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

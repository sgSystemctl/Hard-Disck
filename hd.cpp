#include "./hd.hpp"

extern HD hd[NUM_FACES];

extern interface int_hd;

//Scrive sul disco in due modalità append e non append
//nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void* const buff,size_t len,size_t mode){
    u_int8_t n_sect = GET_SECT(mode);
    u_int8_t n_face = GET_FACE(mode);

    if(!buff || n_sect >= NUM_SECT || n_face >= NUM_FACES){
        printf("Parametri non valido\n");
        return false;
    }
    u_int8_t* buffer = (u_int8_t*)buff;
    if(!mode){
        for (int i = 0;i != DIM_TRACK;i++)
        {
            u_int8_t* track = &hd[n_face].sect[n_sect].track[i];
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
            u_int8_t* track = &hd[n_face].sect[n_sect].track[i];

            if(!len)
                return true;
            
            *track = buffer[i];
            len--;
        }
    }
    return false;
    
    
}

//legge un intero settore
bool readHD(void* buff,size_t len,u_int64_t n_sect,u_int8_t n_face){
    if(!buff || !len || n_sect >= NUM_SECT || n_face >= NUM_FACES){
        printf("Parametri non validi\n");
        return false;
    }

    u_int8_t* dest = (u_int8_t*)buff;

    for(int i = 0 ; i != DIM_TRACK;i++){
        if(!len)
            return true;
        len--;
        u_int8_t* track = &hd[n_face].sect[n_sect].track[i];

        dest[i] = *track;
    }
    return false;
    
}

void* inth(void*){
    memset(&int_hd.data,0,sizeof(sector)-1);
    if(int_hd.control & BIT_READ){
        if(!readHD(&int_hd.data.track,sizeof(sector)-1,int_hd.num_sect,int_hd.num_face)){
            int_hd.status = ERR_READ;
            memset(&int_hd.data,0xff,sizeof(sector));
        }
    }else if(int_hd.control & BIT_WRITE){
        u_int16_t tmp = 0;
        
        tmp = ((int_hd.num_sect << 8) | (int_hd.num_face << 4)) | NO_APPEND;

        if(!writeHD(&int_hd.data,sizeof(sector)-1,tmp)){
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
    if((int_hd.control & BIT_START)){
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
    for (int z = 0; z < NUM_FACES ; z++)
    {
        for (int i = 0; i < NUM_SECT; i++)
        {
            printf(".");
            for (int j = 0; j < DIM_TRACK; j++)
            {
                fprintf(fd,"%x ",hd[z].sect[i].track[j]);
                
            }
            fprintf(fd,"\n");
            
        }
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
    for (int z = 0; z < NUM_FACES; z++)
    {
        for (int i = 0; i < NUM_SECT; i++)
        {
            printf(".");
            for (int j = 0; j < DIM_TRACK; j++)
                if(fscanf(fd,"%hhx ",&hd[z].sect[i].track[j]) == -1){
                    printf("Errore nella lettura del file %s",path);
                    fclose(fd);
                    return false;
                }
        }
    }
    
    
    
    printf("\nCaricamento andato a buon fine\n");
    fclose(fd);
    return true;
}

void printHD(const size_t mode){
    if(!mode){
        for (int z = 0; z < NUM_FACES; z++)
        {
            printf("[FACE %d]:\n",z);
            for (int i = 0; i < NUM_SECT; i++)
            {
                printf("[SECTOR %d]:\t",i);
                for (int j = 0; j < DIM_TRACK; j++){
                    printf("%.2x ",hd[z].sect[i].track[j]);
                }
                printf("\n");
            }
        }
        
        
    }else if(mode & FIRST_SECT){ 
        
        printf("[SECTOR %d]:\t",0);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[0].sect[0].track[i]);
            
        }
        printf("\n");

    }else if(mode & N_SECT){
        u_int8_t num_sect = (mode & ~0X0FF) >> 8;
        u_int8_t num_face = (mode & 0X08) >> 4; 
        
        if(num_sect >= NUM_SECT){
            printf("Settore inesistente\n");
            return;
        }
        
        printf("[SECTOR %d]:\t",num_sect);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[num_face].sect[num_sect].track[i]);
        }
        printf("\n");
    }else if(mode & LAST_SECT){
        
        printf("[SECTOR %d]:\t",NUM_SECT-1);
        for (int i = 0; i < DIM_TRACK; i++)
        {
            printf("%.2x ",hd[0].sect[NUM_SECT-1].track[i]);
            
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
        u_int16_t tmp = 0;
        #ifdef DEBUG
            printf("[DEBUG]:Inizio scrittura della faccia 0 e del settore %d\n",i);
        #endif
        tmp = ((i << 8) | NO_APPEND);
        
        writeHD(buffer,sizeof(buffer), tmp);
        
        tmp = 0;

        #ifdef DEBUG
            printf("[DEBUG]:Inizio scrittura della faccia 1 e del settore %d\n",i);
        #endif

        tmp = ((i << 8) | (0x01<<4)) | NO_APPEND;

        writeHD(buffer,sizeof(buffer),tmp);
    }
}

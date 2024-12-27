#include "./hd.hpp"


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
    if(!start_disp()){
        printf("Errore nell'inizializzazione del dispositivo\n");
        return EXIT_FAILURE;
    }
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
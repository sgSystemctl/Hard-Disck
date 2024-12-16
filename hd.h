#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_SECT 8
#define DIM_TRACK 64 //Un settore è fatto da DIM_SECT * 8 bit

typedef struct
{
    u_int8_t track[DIM_TRACK]; 
}sector;

static sector hd[NUM_SECT];

typedef struct 
{
    u_int8_t control = 0;//registro per pilotare il dispositivo
    u_int8_t status = 0;//registro di stato usato dal dispositivo per comunicare con la cpu il proprio stato
    sector data;//registro dove viene messo il settore appena letto
    size_t num_sect;//numero di settore che identifica la traccia
    size_t num_face;//implementazione futura per hardisk a doppia densitò
    size_t num_cil;//implementazione futura per hardisk multilivello
}interface;

static interface int_hd;

#define APPEND        0x00 
#define NO_APPEND     0x01

//Scrive sul disco in due modalità append e non append
//nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void* const,size_t,size_t);

//legge un intero settore
bool readHD(void*,size_t,u_int64_t);

#define BIT_START 1 << 5
#define BIT_READ  1 << 4 
#define BIT_WRITE 1 << 3 

#define ERR_READ        0x04
#define ERR_WRITE       0x03
#define ERR_MOD_UNKOWN  0x05 //modalità non supportata
void* inth(void*);

bool start_disp();

bool saveHD(const char* const);

bool loadHD(const char* const);

//Modalità di stampa del disco
#define ALL_SECT    (const u_int8_t) 0x00 //Modalità che stampa tutto il disco
#define FIRST_SECT  (const u_int8_t) 0x01 //Modalità che stampa il primo settore
#define N_SECT      (const u_int8_t) 0x02 //modalità dove viene stampato un intero settore il numero di settore è nella parte alta della parola
#define LAST_SECT   (const u_int8_t) 0x04 //Modalità che stampa l'ultimo settore 

void printHD(const size_t);

void writeTest(void);
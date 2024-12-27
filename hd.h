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


//Scrive sul disco in due modalità append e non append
//nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void* const,size_t,size_t);

//legge un intero settore
bool readHD(void*,size_t,u_int64_t);


void* inth(void*);

bool start_disp();

bool saveHD(const char* const);

bool loadHD(const char* const);


void printHD(const size_t);

void writeTest(void);
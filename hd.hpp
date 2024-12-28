#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_SECT 8
#define DIM_TRACK 64 // Un settore è fatto da DIM_SECT * 8 bit
#define NUM_FACES 2

typedef struct
{
    u_int8_t track[DIM_TRACK];
} sector;
// Funzione di utilità per ricavare il valore del settore
#define GET_SECT(arg) ((arg & ~0XFF) >> 8)

typedef struct
{
    sector sect[NUM_SECT];
} HD;
// Funzione di utilità per ottenere il valore della faccia
#define GET_FACE(arg) ((arg & 0x10) >> 4)

typedef struct
{
    u_int8_t control;  // registro per pilotare il dispositivo
    u_int8_t status;   // registro di stato usato dal dispositivo per comunicare con la cpu il proprio stato
    sector data;       // registro dove viene messo il settore appena letto
    size_t num_sect;   // numero di settore che identifica la traccia
    u_int8_t num_face; // implementazione futura per hardisk a doppia densitò
    size_t num_cil;    // implementazione futura per hardisk multilivello
} interface;

#define APPEND 0x00
#define NO_APPEND 0x01
// Scrive sul disco in due modalità append e non append
// nel 4 track più significati è scritto il settore da scrivere
bool writeHD(const void *const buff, size_t len, size_t mode = APPEND);

// legge un intero settore
bool readHD(void *buff, size_t len = DIM_TRACK, u_int64_t n_sect = 0, u_int8_t n_face = 0);

#define BIT_START 0x01
#define BIT_READ 0x02
#define BIT_WRITE 0x04

#define ERR_READ 0x04
#define ERR_WRITE 0x03
#define ERR_MOD_UNKOWN 0x05 // modalità non supportata
void *inth(void *);

bool start_disp();

bool saveHD(const char *const path = "./hd.txt");

bool loadHD(const char *const path = "./hd.txt");

// Modalità di stampa del disco
#define ALL_SECT (const u_int8_t)0x00   // Modalità che stampa tutto il disco
#define FIRST_SECT (const u_int8_t)0x01 // Modalità che stampa il primo settore
#define N_SECT (const u_int8_t)0x02     // modalità dove viene stampato un intero settore il numero di settore è nella parte alta della parola
#define LAST_SECT (const u_int8_t)0x04  // Modalità che stampa l'ultimo settore
void printHD(const size_t mode = ALL_SECT);

void writeTest(void);

void set_int_controll(u_int8_t);
void set_int_status(u_int8_t);
void set_int_data(sector);
void set_int_num_sect(size_t);
void set_int_face(u_int8_t);

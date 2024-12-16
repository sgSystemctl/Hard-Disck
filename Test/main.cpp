#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

extern bool writeHD(const void* const,size_t,size_t);
#define NTHREADS 8

extern bool readHD(void*,size_t,u_int8_t);
extern bool saveHD(const char* const);
extern bool loadHD(const char* const);
extern void printHD(const size_t);

pthread_mutex_t M;


void* testDisk(void* arg){
    int block = 0;
    
    pthread_mutex_lock(&M);
    u_int8_t* buffer = (u_int8_t*)arg;
    
    u_int8_t id = *buffer;
    buffer++;

    u_int8_t len = *buffer;
    buffer++;
    
    u_int8_t num_sect = *buffer;
    buffer++;

    
    writeHD(buffer,len,num_sect | 1);
    printf("[thread %u]: Scrittura sul disco completata\n",id);
    block++;

    pthread_mutex_unlock(&M);
    printf("[thread %u]:terminazione....\n",id);
    pthread_exit(nullptr);
    
}

int main(void){
    
    pthread_t thread[NTHREADS];
    u_int8_t data[] = {1,2,3,4,5,6,7,8};
    u_int8_t len = sizeof(data)/sizeof(u_int8_t);
    u_int8_t* args = new u_int8_t[len+3];
    pthread_mutex_init(&M,nullptr);
    

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_mutex_lock(&M);
        ((u_int8_t*)args)[0] = i+1;
        ((u_int8_t*)args)[1] = len;
        ((u_int8_t*)args)[2] = i << 4;
        strncat((char*)args,(const char*)data,len);
        pthread_mutex_unlock(&M);
        pthread_create(&thread[i],nullptr,testDisk,args);
    }
    pthread_join(thread[0],nullptr);
    printHD(0);
    pthread_exit(nullptr);
    
}
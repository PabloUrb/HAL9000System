#include "thread.h"
extern ConfigPoole * configPoole;
extern int socketFD;
pthread_mutex_t sum_mutex;

char * reciveTrama2(char * trama){
    uint16_t header_length = (trama[1] << (8*1)) + trama[2];
    char * header = (char*)malloc(sizeof(char)*header_length);
    memcpy(header, &trama[3], header_length);
    char * data = (char*)malloc(sizeof(char)*256-3-header_length);
    memcpy(data, &trama[3+header_length], 256-3-header_length);
    free(header);
    return data;
    free(data);
}
//crear struct para guardar la data, el header asignar un identificador
void * init_hilos(){
    char * trama = (char*)malloc(sizeof(char)*256);
    uint16_t header_length;
    int sumlenData = 0;
    char * header = (char*)malloc(sizeof(char));
    char * sumData = (char*)malloc(sizeof(char));
    while(1){
        read(socketFD, trama, 256);
        header_length = (trama[1] << (8*1)) + trama[2];
        header = (char*)realloc(header, sizeof(char)*header_length);
        memcpy(header, &trama[3], header_length);
        printf("header: %s\n", header);
        char* data = reciveTrama2(trama);
        int sizeData = strlen(data);
        printf("sizeData: %d\n", sizeData);
        sumlenData += sizeData;
        //add data to sumData
        sumData = (char*)realloc(sumData, sizeof(char)*sumlenData);
        strcat(sumData, data);
        replace_char(sumData, '&', ',');
        replace_char(sumData, '#', '\n');
        printf("sumData: %s\n", sumData);
        bzero(trama, 256);
    }
    free(trama);
    return NULL;
}

void create_hilos(){
    pthread_mutex_init(&sum_mutex, NULL);

    key_t key = ftok("thread.c", 1);
    int mq_id = msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1) {
        printa(ERR_COLA);
        exit(EXIT_FAILURE);
    }

    pthread_t thread = (pthread_t)malloc(sizeof(pthread_t));
    int id;
    id = pthread_create(&thread, NULL, init_hilos, NULL);
    if (id != 0){
        perror(ERR_INVALID_INPUT);
    }

    if(pthread_detach(thread) != 0) {
        perror("pthread_detach() error");
        exit(4);
    }
    pthread_mutex_destroy(&sum_mutex);
    
    
}
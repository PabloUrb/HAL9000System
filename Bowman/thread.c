#include "thread.h"
extern ConfigPoole * configPoole;
extern int socketFD;
pthread_mutex_t sum_mutex;



// Función para recibir datos de la trama
char * reciveTrama2(char * trama){
    uint16_t header_length = (trama[1] << (8*1)) + trama[2];
    char * header = (char*)malloc(header_length);
    memcpy(header, &trama[3], header_length);
    char * data = (char*)malloc(256 - 3 - header_length); // Tamaño fijo 256
    memcpy(data, &trama[3 + header_length], 256 - 3 - header_length);
    free(header); // Liberar la memoria asignada a header
    return data; // Devolver data antes de liberar la memoria asignada
}

// Función del thread
void * init_hilos(void *arg){
    int socketFD = (uintptr_t)arg;
    char * trama = (char*)malloc(256);
    uint16_t header_length;
    int sumlenData = 0;
    char * header = NULL;
    char * sumData = NULL;
    
    printf("socketFD: %d\n", socketFD);
    
    while(1){
        int bytesRead = read(socketFD, trama, 256);
        if (bytesRead <= 0) {
            if(bytesRead == 0){
                close(socketFD);
                printf(SERVER_DOWN);
            }else{
                printf(ERR_READ);
            }
            close(socketFD);
            break;
        }
        
        header_length = (trama[1] << (8*1)) + trama[2];
        header = (char*)realloc(header, header_length);
        memcpy(header, &trama[3], header_length);
        printf("header: %s\n", header);
        
        char* data = reciveTrama2(trama);
        int sizeData = strlen(data);
        printf("sizeData: %d\n", sizeData);
        
        sumlenData += sizeData;
        sumData = (char*)realloc(sumData, sumlenData);
        strcat(sumData, data);
        
        replace_char(sumData, '&', ',');
        replace_char(sumData, '#', '\n');
        printf("sumData: %s\n", sumData);
        
        bzero(trama, 256);
        free(data); // Liberar memoria asignada a data
    }
    
    free(trama); // Liberar memoria asignada a trama
    free(header); // Liberar memoria asignada a header
    free(sumData); // Liberar memoria asignada a sumData
    
    return NULL;
}

void create_hilos(int socketFD){
    pthread_mutex_init(&sum_mutex, NULL);
    
    int project_id = getpid() % 255;
    key_t key = ftok("thread.c", project_id);
    int mq_id = msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1) {
        perror("msgget error");
        exit(EXIT_FAILURE);
    }

    pthread_t thread;
    int id = pthread_create(&thread, NULL, init_hilos, (void*)(uintptr_t)socketFD);
    if (id != 0) {
        fprintf(stderr, "Error al crear el thread: %s\n", strerror(id));
        exit(EXIT_FAILURE);
    }

    if(pthread_detach(thread) != 0) {
        perror("pthread_detach() error");
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_destroy(&sum_mutex);
}
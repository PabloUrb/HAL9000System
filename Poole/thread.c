#include "thread.h"
extern ConfigPoole * configPoole;
pthread_mutex_t sum_mutex;

unsigned char* generateTrama2(char * header, int fd, char * songs){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x02;
    uint16_t size = strlen(header);
    trama[1] = (size >> (size*1)) & 0xff;
    trama[2] = (size >> (size*0)) & 0xff;
    strcpy((char*) &trama[3], header);
    if(strcmp(header, SONGS_RESPONSE)==0){
        char * data = (char*)malloc(sizeof(char)*256-3-size);
        strcpy(data, songs);
        int sizeData = strlen(data);
        memcpy(&trama[3+size], data, sizeData);
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
        
        printF("\nTrama enviada\n");
        printf("Type: %d\n", trama[0]);
        printf("Header Length: %d%d\n", trama[1], trama[2]);
        printf("Header: %s\n", &trama[3]);
        printf("Data: %s\n", &trama[3+size]);
    }else if(strcmp(header, PLAYLISTS_RESPONSE)==0){
        char * data = (char*)malloc(sizeof(char)*256-3-size);
        replace_char(songs, ',', '&');
        replace_char(songs, '\n', '#');
        strcpy(data, songs);
        int sizeData = strlen(data);
        memcpy(&trama[3+size], data, sizeData);
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
        
        printF("\nTrama enviada\n");
        printf("Type: %d\n", trama[0]);
        printf("Header Length: %d%d\n", trama[1], trama[2]);
        printf("Header: %s\n", &trama[3]);
        printf("Data: %s\n", &trama[3+size]);

    }
    

    if(write(fd, trama, 256)<0){
        perror(ERR_SEND);
    }else{
        printa("trama enviada\n");
    }
    
    
    
    return trama;
    free(trama);
}

char * leer_canciones(){
    DIR *d;
    struct dirent *dir;
    char * songs;
    int contador = 0;
    char*buff = malloc(200 * sizeof(char));
    sprintf(buff, "./music%s/songs/", configPoole->nom_carpeta);
    d = opendir(buff);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            int size = strlen(dir->d_name);
            if(contador==0){
                songs = (char*)malloc(sizeof(char)*(size+1));
                strcpy(songs, dir->d_name);
                strcat(songs, "&");
            }else{
                songs = (char*)realloc(songs, sizeof(char*)*(size+1));
                strcat(songs, dir->d_name);
                strcat(songs, "&");
            }
            contador++;
        }
    closedir(d);
    free(buff);
    }
    return songs;
}
char * leerCSV(){
    char * playlists;
    int contador = 0;
    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char*buff = malloc(200 * sizeof(char));
    sprintf(buff, "./music%s/playlists.csv", configPoole->nom_carpeta);
    fp = fopen(buff, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        if(contador==0){
            playlists = (char*)malloc(sizeof(char)*(read+1));
            replace_char(line, ',', '&');
            replace_char(line, '\n', '#');
            strcpy(playlists, line);
            
        }else{
            //realloc de la cadena mas la nueva linea
            playlists = (char*)realloc(playlists, strlen(playlists)+(read+1));
            replace_char(line, ',', '&');
            replace_char(line, '\n', '#');
            strcat(playlists, line);
        }
        
        contador++;
        printf("\nRetrieved line of length %zu:\n", read);
        printa("=====================================\n");
        printf("%s\n", line);
        printf("size : %ld \n", strlen(playlists));
        printf("playlists : %s\n", playlists);
        }
        playlists[read] = '\0';

    

    fclose(fp);
    if (line){
        free(line);
    }
    free(buff);
    return playlists;
    free(playlists);
}
void dividir_cadena(char * cadena, int tamano, char * header, int fd){
    int cadena_len = strlen(cadena);
    int num_paquetes = (cadena_len + tamano - 1) / tamano; // Calcula el número de paquetes necesarios

    // Asignar memoria para el array de paquetes
    char **paquetes = (char **)malloc(num_paquetes * sizeof(char *));
    if (paquetes == NULL) {
        perror("Error al asignar memoria para los paquetes");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_paquetes; ++i) {
        paquetes[i] = (char *)malloc((tamano + 1) * sizeof(char)); // +1 para el terminador nulo
        if (paquetes[i] == NULL) {
            perror("Error al asignar memoria para un paquete");
            // Liberar la memoria asignada hasta ahora
            for (int j = 0; j < i; ++j) {
                free(paquetes[j]);
            }
            free(paquetes);
            exit(EXIT_FAILURE);
        }

        // Copiar la parte correspondiente de la cadena al paquete
        strncpy(paquetes[i], cadena + i * tamano, tamano);
        paquetes[i][tamano] = '\0'; // Asegurarse de que el paquete está null-terminated
        generateTrama2(header, fd, paquetes[i]);
    }
}
long random_long(long min, long max) {
    if (min > max) {
        long temp = min;
        min = max;
        max = temp;
    }

    // Generar un número aleatorio en el rango [min, max]
    return min + (rand() % (max - min + 1));
}

void * init_hilos(void* arg){
    printa("Hilo iniciado\n");
    int mq_id = *((int *)arg);
    printf("mq_id : %d\n", mq_id);
    int fd;
    char * songs;
    char * playlists;
    Message msg;
    while(1){
        ssize_t bytes_read = msgrcv(mq_id, &msg, (2*sizeof(int))+(60*sizeof(char)), 1, 0);
        if (bytes_read < 0 ) {
                perror(ERR_MSG);
                break;
        }else{
            Message m2;
            m2.fd = msg.fd;
            printf("fd : %d\n", m2.fd);
            m2.mtype = random_long(100000L, 999999L);
            strcpy(m2.header, msg.header);
            printf("fd : %d, mtype: %ld, header: %s\n", m2.fd, m2.mtype, m2.header);
            fd = m2.fd;
            

            pthread_mutex_lock (&sum_mutex);
            if(strcmp(m2.header, LIST_SONGS)==0){
                songs = leer_canciones();
                //Si el tamano de la cadena es menor que el tamano que tiene que ocupar en la trama
                if(strlen(songs)<(256-3-strlen(SONGS_RESPONSE))){
                    generateTrama2(SONGS_RESPONSE, fd, songs);
                }else{
                    //TODO generar varias tramas
                }
            }if(strcmp(m2.header, LIST_PLAYLISTS)==0){
                playlists = leerCSV();
                printa("\n*******************************\n");
                printf("size : %ld \n", strlen(playlists));
                printf("%s\n", playlists);
                //Si el tamano de la cadena es menor que el tamano que tiene que ocupar en la trama
                if(strlen(playlists)<(256-3-strlen(PLAYLISTS_RESPONSE))){
                    generateTrama2(PLAYLISTS_RESPONSE, fd, playlists);
                }else{
                    //TODO generar varias tramas
                    //dividir la cadena en varias subcadenas de 256-3-strlen(PLAYLISTS_RESPONSE)
                    printf("strlen(PLAYLISTS_RESPONSE) : %ld\n", strlen(PLAYLISTS_RESPONSE));
                    printf("256-3-strlen(PLAYLISTS_RESPONSE) : %ld\n", 256-3-strlen(PLAYLISTS_RESPONSE));
                    dividir_cadena(playlists, 256-3-strlen(PLAYLISTS_RESPONSE), PLAYLISTS_RESPONSE, fd);
                    //generar trama con cada subcadena

                }
            }
            
            pthread_mutex_unlock (&sum_mutex);
        }
    }
    printf("%s\n", songs);
    printf("size : %ld \n", strlen(songs));
    return NULL;
}

void create_hilos(){
    pthread_mutex_init(&sum_mutex, NULL);
    int project_id = getpid() % 255;
    key_t key = ftok("thread.c", project_id);
    int mq_id = msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1) {
        printa(ERR_COLA);
        exit(EXIT_FAILURE);
    }

    pthread_t* threads = malloc(sizeof(pthread_t)*NI_THREADS);
    int id[NI_THREADS];
    for(int i=0;i<NI_THREADS;i++){

        id[i] = pthread_create(&threads[i], NULL, init_hilos, &mq_id);
        if (id[i] != 0){
            perror(ERR_INVALID_INPUT);
        }

        if(pthread_detach(threads[i]) != 0) {
            perror("pthread_detach() error");
            exit(4);
        }
        pthread_mutex_destroy(&sum_mutex);
    }
    
}
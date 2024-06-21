#include "thread.h"
extern int socketFD; // Assuming this is declared and defined elsewhere
pthread_mutex_t sum_mutex = PTHREAD_MUTEX_INITIALIZER;
Playlist *playlists = NULL;

void mostrar_playlist(const char *entrada) {
    // Variables para almacenar el nombre de la playlist y las canciones
    char nombre_playlist[256];  // Tamaño suficiente para manejar el nombre de la playlist
    char *token;
    const char delimiter[] = ",";
    int count = 0;
    // Copiar la entrada en un buffer para modificarla
    char entrada_copy[256];
    strncpy(entrada_copy, entrada, sizeof(entrada_copy));
    entrada_copy[sizeof(entrada_copy) - 1] = '\0';  // Asegurar que el buffer esté terminado en null

    // Obtener el nombre de la playlist
    token = strtok(entrada_copy, delimiter);
    if (token == NULL) return;  // Salir si no hay tokens
    
    // Mostrar el nombre de la playlist
    if(strstr(token, "mp3")==NULL){
        //playlist
        strcpy(nombre_playlist, token);
        printf("%s\n", nombre_playlist);
        /*Playlist *playlist = (Playlist*)malloc(sizeof(Playlist));
        playlist->nombre = (char*)malloc(strlen(nombre_playlist) + 1);
        strcpy(playlist->nombre, nombre_playlist);
        printf("playlist->nombre: %s\n", playlist->nombre);*/
        count=0;
        // Mostrar las canciones
        while ((token = strtok(NULL, delimiter)) != NULL) {
            // Mostrar la canción
            count++;
            printf("    %d. %s\n", count, token);
            if (strchr(token, '\n') != NULL) {
                count = 0;
            }
            
            /*playlist->canciones = (Cancion*)malloc(sizeof(Cancion) * count);
            playlist->num_canciones = count;
            playlist->canciones[count-1].nombre = (char*)malloc(strlen(token) + 1);
            strcpy(playlist->canciones[count-1].nombre, token);
            printf("playlist->canciones[count-1].nombre: %s\n", playlist->canciones[count-1].nombre);
            printf("playlist->num_canciones: %d\n", playlist->num_canciones);*/
        }
        
        /*free(playlist);
        free(playlist->nombre);
        free(playlist->canciones);
        free(playlist->canciones->nombre);*/
    }else{
        //canciones
        count=1;
        printf("    %d. %s\n", count, token);
        // Mostrar las canciones
        while ((token = strtok(NULL, delimiter)) != NULL) {
        // Mostrar la canción
        count++;
        printf("    %d. %s\n", count, token);
        }
    }
    
    
    free(token);  // Liberar la memoria asignada por strtok
    printf("\n");  // Imprimir una línea en blanco para separar playlists
}

char * reciveTrama2(const char * trama){
    uint16_t header_length = (trama[1] << (8*1)) + trama[2];
    char * header = (char*)malloc(header_length + 1); // +1 for null terminator
    if (header == NULL) {
        perror("Error allocating memory for header");
        return NULL;
    }
    memcpy(header, &trama[3], header_length);
    header[header_length] = '\0';
    
    // Assuming the data starts right after the header
    int data_length = 256 - 3 - header_length;
    char * data = (char*)malloc(data_length + 1); // +1 for null terminator
    if (data == NULL) {
        perror("Error allocating memory for data");
        free(header); // Free allocated header memory
        return NULL;
    }
    memcpy(data, &trama[3 + header_length], data_length);
    data[data_length] = '\0';
    
    free(header); // Free allocated header memory
    return data; // Return data before freeing allocated memory
}


// Function to initialize threads
void *init_hilos(void *arg) {
    int socketFD = (uintptr_t)arg;
    unsigned char trama[256];
    char *header = NULL;
    char *sumData = NULL;
    int total_tramas = 0;
    int received_tramas = 0;
    
    printf("socketFD: %d\n", socketFD);
    
    while (1) {
        int bytesRead = read(socketFD, trama, 256);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                close(socketFD);
                printf("Servidor desconectado\n");
            } else {
                perror("Error al leer del socket");
            }
            close(socketFD);
            break;
        }
        
        // Read header and total number of frames
        uint16_t header_length = (trama[1] << 8) + trama[2];
        header = (char*)realloc(header, header_length + 1);
        if (header == NULL) {
            perror("Error reallocating memory for header");
            close(socketFD);
            break;
        }
        memcpy(header, &trama[3], header_length);
        header[header_length] = '\0';
        printf("Received Header: %s\n", header);
        
        // Extract total number of frames from format "00005$"
        char total_tramas_str[6];
        memcpy(total_tramas_str, &trama[header_length + 3], 5);
        total_tramas_str[5] = '\0';
        total_tramas = atoi(total_tramas_str);
        
        // Check current frame number
        int num_trama = (trama[252] << 8) + trama[253];
        printf("Received Trama Number: %d\n", num_trama);
        
        // Check if it is the first frame of a new series
        if (num_trama == 1) {
            received_tramas = 0;
            sumData = (char*)malloc(1);
            if (sumData == NULL) {
                perror("Error allocating memory for sumData");
                free(header);
                close(socketFD);
                break;
            }
            sumData[0] = '\0';
        }
        
        // If all frames of the current series have been received, print to screen
        received_tramas++;
        char* data = reciveTrama2((const char *)trama);
        if (data != NULL) {
            int sizeData = strlen(data);
            printf("sizeData: %d\n", sizeData);
            
            pthread_mutex_lock(&sum_mutex);
            int sumlenData = strlen(sumData);
            sumData = (char*)realloc(sumData, sumlenData + sizeData + 1);
            if (sumData == NULL) {
                perror("Error reallocating memory for sumData");
                pthread_mutex_unlock(&sum_mutex);
                free(header);
                close(socketFD);
                break;
            }
            strcat(sumData, data);
            pthread_mutex_unlock(&sum_mutex);
            printf("total_tramas_str: %s\n", total_tramas_str);
            printf("Received Tramas: %d\n", received_tramas);
            printf("Total Tramas: %d\n\n", total_tramas);
            char * total_tramas_str2;
            total_tramas_str2 = malloc(strlen(total_tramas_str)+1);
            strcpy(total_tramas_str2, total_tramas_str); /* copy name into the new var */
            strcat(total_tramas_str2, "$");
            char *ptr = strstr(sumData, total_tramas_str2);
            if (ptr != NULL) {
                strcpy(ptr, ptr + strlen(total_tramas_str2));
            }
            replace_char(sumData, '&', ',');
            replace_char(sumData, '#', '\n');
            
            pthread_mutex_lock(&sum_mutex);
            if(received_tramas == total_tramas){
                printf("=============================\n");
                printf("sumData: %s\n", sumData);
                mostrar_playlist(sumData);
                free(sumData);
            }
            free(total_tramas_str2);
            pthread_mutex_unlock(&sum_mutex);
        }
        
        free(data);
        bzero(trama, 256); // Clear trama for the next read
    }
    
    free(header); // Free allocated memory for header
    free(sumData); // Free allocated memory for sumData
    close(socketFD); // Close socket on completion
    return NULL;
}

// Function to create threads
void create_hilos(int socketFD){
    pthread_mutex_init(&sum_mutex, NULL);
    
    pthread_t thread;
    int id = pthread_create(&thread, NULL, init_hilos, (void*)(uintptr_t)socketFD);
    if (id != 0) {
        fprintf(stderr, "Error creating thread: %s\n", strerror(id));
        exit(EXIT_FAILURE);
    }

    if (pthread_detach(thread) != 0) {
        perror("pthread_detach() error");
        exit(EXIT_FAILURE);
    }
}
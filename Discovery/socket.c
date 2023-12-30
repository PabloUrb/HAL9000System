#include "socket.h"
#define MAX_CHAR 50

int sockfdCopy[2], newsockfdCopy[2];

typedef struct{
    uint8_t type;
    uint16_t header_length;
    char * header;
    char * data;
    int contador;
}Trama;
//array de tramas
Trama * tramas;
int numTramas;

void initSockets(){
    printa("Init Sockets\n");
}

void intHandler2(){

    if(newsockfdCopy[0]!=0){printf("cierrra newSock POOLE\n");close(newsockfdCopy[0]);}
    if(newsockfdCopy[0]!=0){printf("cierrra newSock BOWMAN\n");close(newsockfdCopy[1]);}
    if(newsockfdCopy[0]!=0){printf("cierrra Sock POOLE\n");close(sockfdCopy[0]);}
    if(newsockfdCopy[0]!=0){printf("cierrra Sock BOWMAN\n");close(sockfdCopy[1]);}
    
    printa(SIGINT1);
    free(tramas);
    intHandler();
    //raise(SIGKILL);
}
void shutdown_Poole(int port){
    if(tramas!=NULL){
        char * nom;
        char * ip;
        char * port2;
        for(int i = 0; tramas[i].data != NULL; i++){
            char * temp1 = (char*)malloc(sizeof((char *)tramas[i].data));
            memcpy(temp1, &tramas[i].data, sizeof(tramas[i].data));
            nom = strtok(temp1, "&");
            ip = strtok(NULL, "&");
            port2 = strtok(NULL, "&");

            printf("\n====================\n");
            printf("Enviado a Poole\n");
            printf("nom: %s\n", nom);
            printf("ip: %s\n", ip);
            printf("i: %d\n", i);
            printf("numTramas: %d\n", numTramas);
            printf("port 1: %d\n", port);
            printf("port 2: %s\n", port2);
            printf("\n====================\n");
            if(port2 != NULL && atoi(port2) == port){
                tramas = (Trama*)realloc(tramas, sizeof(Trama)*(numTramas+1));
                tramas[i] = tramas[i+1];
            }if(numTramas == 1){
                //free(tramas);
            }
            free(temp1);
        }
        numTramas--;
    }
}
Trama getMinUsersConnected(){
    Trama minTrama;
    int min = 1000;
    int cont = 0;
    printa("\n");
    printa("\n");
    printa("\n");
    if(tramas!=NULL){
        for(int i = 0; tramas[i].data != NULL; i++){
            printf("contador de la trama %d: %d\n", i, tramas[i].contador);
            if(tramas[i].contador <= min){
                min = tramas[i].contador;
            }
            
        }
        printf("\n\nMIN: %d\n\n", min);
        for(int i = 0; tramas[i].data != NULL; i++){
            printf("\ncontador de la trama %d: %d\n", i, tramas[i].contador);
            if(tramas[i].contador == min && cont == 0){
                printf("trama data: %s\n", tramas[i].data);
                minTrama = tramas[i];
                tramas[i].contador++;
                cont++;
            }
        }
    }else{
        minTrama.data = NULL;
    }
    return minTrama;
}
unsigned char* generateTrama(char * header, char * data){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    printf("header: %s\n", header);
    uint16_t size = strlen(header);
    trama[1] = (size >> (size*1)) & 0xff;
    trama[2] = (size >> (size*0)) & 0xff;
    strcpy((char*) &trama[3], header);
    if(data != NULL){
        char * data2 = (char*)malloc(sizeof(char)*256-3-size);
        strcat(data2, data);
        int sizeData = strlen(data2);
        //add data to trama
        memcpy(&trama[3+size], data2, sizeData);
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    }else{
        bzero(&trama[3+size], 256-3-size);
    }
    

    printF("\nTrama enviada\n");
    printf("Type: %d\n", trama[0]);
    printf("Header Length: %d%d\n", trama[1], trama[2]);
    printf("Header: %s\n", &trama[3]);
    printf("Data: %s\n", &trama[3+size]);
    return trama;
}
int validatePortPoole(int port){
    int response = 0;
    if(tramas!=NULL){
        char * nom;
        char * ip;
        char * port2;
        for(int i = 0; tramas[i].data != NULL; i++){
            char * temp2 = (char*)malloc(sizeof((char *)tramas[i].data));
            memcpy(temp2, &tramas[i].data, sizeof(tramas[i].data));
            nom = strtok(temp2, "&");
            ip = strtok(NULL, "&");
            port2 = strtok(NULL, "&");

            printf("\n====================\n");
            printf("Validate port\n");
            printf("nom: %s\n", nom);
            printf("ip: %s\n", ip);
            printf("i: %d\n", i);
            printf("numTramas: %d\n", numTramas);
            printf("port 1: %d\n", port);
            printf("port 2: %s\n", port2);
            printf("posicion: %d\n", i);
            printf("\n====================\n");
            if(port2 != NULL && atoi(port2) == port){
                return 1;
            }
            free(temp2);
        }
        
    }
    return response;
}

int reciveTrama(int newsockfd){
    int response = 0;
    printa("Recive Trama\n");
    unsigned char trama[256];
    read(newsockfd, trama, 256);
    
    printa("\n");
    printf("Size of trama: %lu\n", sizeof(trama));
    printa("\n\n");
    if(sizeof(trama)==256){
        Trama trama1;
        trama1.contador = 0;
        trama1.type = trama[0];
        trama1.data = malloc(sizeof(char)*256);
        trama1.header_length = (trama[1] << (8*1)) + trama[2];
        trama1.header = (char*)malloc(sizeof(char)*trama1.header_length);
        memcpy(trama1.header, &trama[3], trama1.header_length);
        
        printf("Header: %s\n", trama1.header);
        if(strcmp(trama1.header, NEW_POOLE)==0){
            trama1.data = (char*)malloc(sizeof(char)*(256-trama1.header_length-3));
            memcpy(trama1.data, &trama[3+trama1.header_length], 256-trama1.header_length-3);
            
            

            printf("Type: %d\n", trama1.type);
            printf("Header Length: %u\n", (unsigned int)trama1.header_length);
            printf("Header: %s\n", trama1.header);
            printf("Data 1: %s\n", trama1.data);
            printf("Contador: %d\n", trama1.contador);
            
            //añadir trama en la siguente posicion del array de tramas
            printf("NumTramas: %d\n", numTramas);
            
            char * temp3 = (char*)malloc(sizeof(char)*(256-trama1.header_length-3));
            memcpy(temp3, &trama[3+trama1.header_length], 256-trama1.header_length-3);
            char * nom = strtok(temp3, "&");
            char * ip = strtok(NULL, "&");
            char * port2 = strtok(NULL, "&");
            printf("nom: %s\n", nom);
            printf("ip: %s\n", ip);
            printf("port: %s\n", port2);
            if(validatePortPoole(atoi(port2))==0){
                if(numTramas == 0){
                    tramas = (Trama*)malloc(sizeof(Trama));
                }else{
                    tramas = (Trama*)realloc(tramas, sizeof(Trama)*(numTramas+1));
                }
                printf("Data 2: %s\n", trama1.data);
                tramas[numTramas] = trama1;
                printf("Size of tramas: %lu\n", sizeof(tramas));
                printf("header de la trama: %s\n", tramas[numTramas].header);
                printf("data de la trama: %s\n", tramas[numTramas].data);
                numTramas++;
                
            }
            free(temp3);
            printa("\n");
        }else if(strcmp(trama1.header, DISCONECT)==0){
            trama1.data = (char*)malloc(sizeof(char)*(256-trama1.header_length-3));
            memcpy(trama1.data, &trama[3+trama1.header_length], 256-trama1.header_length-3);
            char * nom = strtok(trama1.data, "&");
            char * ip = strtok(NULL, "&");
            char * port2 = strtok(NULL, "&");
            printf("nom: %s\n", nom);
            printf("ip: %s\n", ip);
            printf("port: %s\n", port2);
            shutdown_Poole(atoi(port2));
        }else{
            free(trama1.header);
        }
        
        response = 1;
        
    }else{
        printa(ERR_TRAMA_SIZE);
    }
    return response;
}

int launch_server(int port, char * ip, ConfigDiscovery * configDiscovery) {
    
    int sockfd;
    
    unsigned char* response;

    signal(SIGINT, intHandler2);
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(port == configDiscovery->portPoole){
        sockfdCopy[0] = sockfd;
    }else{
        sockfdCopy[1] = sockfd;
    }
    
    
    if (sockfd < 0) {
        perror(ERR_SOCKET);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        
        if (bind(sockfd, (void *) &serv_addr, sizeof(serv_addr)) < 0){
            perror(ERR_BIND);
            if(port == configDiscovery->portPoole){
                printf("FALLA EL PUERTO %d / POOLE", port);
            }else{
                printf("FALLA EL PUERTO %d / BOWMAN", port);
            }
            
            exit(EXIT_FAILURE);
        }

    if(listen(sockfd, 10) < 0){
        printa(ERR_LISTEN);
    }else{
        while (1)
        {
            struct sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);
            printa(WAITING_CONNECTIONS);
            
            int newsockfd = accept(sockfd, (void *) &cli_addr, &clilen);
            if(port == configDiscovery->portPoole){
                newsockfdCopy[0] = newsockfd;
            }else{
                newsockfdCopy[1] = newsockfd;
            }
            if(newsockfd<0){
                printa(ERR_ACCEPT);
                return 0;
            }

            if(port == configDiscovery->portPoole){
                printa(NEW_POOLE_CONNECTION);
                if(reciveTrama(newsockfd)==1){
                    //write(newsockfd, OK, myStrlen(OK));
                    response = generateTrama(CON_OK, NULL);
                    write(newsockfd, response, 256);
                    free(response);
                }else{
                    //write(newsockfd, KO, myStrlen(KO));
                    response = generateTrama(CON_KO, NULL);
                    write(newsockfd, response, 256);
                    free(response);
                }
            }else if(port == configDiscovery->portBowman){
                printa(NEW_BOWMAN_CONNECTION);
                if(reciveTrama(newsockfd)==1){
                    Trama trama = getMinUsersConnected();
                    if(trama.data!=NULL){
                        response = generateTrama(CON_OK, trama.data);
                        write(newsockfd, response, 256);
                    }else{
                        response = generateTrama(CON_KO, NULL);
                        write(newsockfd, response, 256);
                    }
                }else{
                    response = generateTrama(CON_KO, NULL);
                    write(newsockfd, response, 256);
                }
                
                free(response);
            }else{
                printa(ERR_INVALID_INPUT);
                return 0;
            }
            //close(newsockfd);
        }
        free(tramas);
        free(response);
    }
    //close(sockfd);
    return 0;
}
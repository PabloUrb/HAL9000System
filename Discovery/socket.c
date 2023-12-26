#include "socket.h"
#define MAX_CHAR 50
int sockfd, newsockfd;


typedef struct{
    uint8_t type;
    uint16_t header_length;
    char * header;
    char * data;
}Trama;
//array de tramas
Trama * tramas;
int numTramas;

void initSockets(){
    printa("Init Sockets\n");
}

void intHandler2(){
    printa(SIGINT1);
    shutdown(newsockfd, SHUT_RDWR);
    shutdown(sockfd, SHUT_RDWR);
    raise(SIGKILL);
}
unsigned char* generateTrama(char * header){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    uint16_t size = strlen(header);
    trama[1] = (size >> (8*1)) & 0xff;
    trama[2] = (size >> (8*0)) & 0xff;
    strcpy((char*) &trama[3], header);
    bzero(&trama[3+size], 256-3-size);

    printF("\nTrama enviada\n");
    printf("Type: %d\n", trama[0]);
    printf("Header Length: %d%d\n", trama[1], trama[2]);
    printf("Header: %s\n", &trama[3]);
    return trama;
}


int reciveTrama(){
    int response = 0;
    printa("Recive Trama\n");
    unsigned char trama[256];
    read(newsockfd, trama, 256);
    
    printa("\n");
    printf("Size of trama: %lu\n", sizeof(trama));
    printa("\n\n");
    if(sizeof(trama)==256){
        Trama trama1;
        trama1.type = trama[0];
        trama1.header_length = (trama[1] << (8*1)) + trama[2];
        trama1.header = (char*)malloc(sizeof(char)*trama1.header_length);
        memcpy(trama1.header, &trama[3], trama1.header_length);
        
        trama1.data = (char*)malloc(sizeof(char)*(256-trama1.header_length-3));
        memcpy(trama1.data, &trama[3+trama1.header_length], 256-trama1.header_length-3);

        printa("Type: ");
        printa((char*)&trama1.type);
        printa("\n");
        printa("Header Length: ");
        printa((char*)&trama1.header_length);
        printa("\n");
        printa("Header: ");
        printa(trama1.header);
        printa("\n");
        printa("Data: ");
        printa(trama1.data);
        printa("\n");
        
        //añadir trama en la siguente posicion del array de tramas
        printf("NumTramas: %d\n", numTramas);
        
        
        tramas[numTramas] = trama1;
        numTramas++;
        tramas = (Trama*)realloc(tramas, sizeof(Trama)*(numTramas+1));
        
        
        printf("Size of tramas: %lu\n", sizeof(tramas));
        printf("data de la trama: %s\n", tramas[numTramas-1].data);
        printa("\n");
        response = 1;
        
        free(trama1.header);
        free(trama1.data);
    }else{
        printa(ERR_TRAMA_SIZE);
    }
    return response;
}

int launch_server(int port, char * ip, ConfigDiscovery * configDiscovery) {
    
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    unsigned char* response;

    signal(SIGINT, intHandler2);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    tramas = (Trama*)malloc(sizeof(Trama)*100);
    if (sockfd < 0) {
        printa(ERR_SOCKET);
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);
        
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0){
            perror(ERR_BIND);
            return 2;
        }

    if(listen(sockfd, 10) < 0){
        printa(ERR_LISTEN);
    }else{
        clilen = sizeof(cli_addr);
        while (1)
        {
            
            printa(WAITING_CONNECTIONS);
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if(newsockfd<0){
                printa(ERR_ACCEPT);
                return 0;
            }

            if(port == configDiscovery->portPoole){
                printa(NEW_POOLE_CONNECTION);
                if(reciveTrama()==1){
                    write(newsockfd, OK, myStrlen(OK));
                    response = generateTrama(CON_OK);
                    write(newsockfd, response, 256);
                }else{
                    write(newsockfd, KO, myStrlen(KO));
                    response = generateTrama(CON_KO);
                    write(newsockfd, response, 256);
                }
                
            }else if(port == configDiscovery->portBowman){
                printa(NEW_BOWMAN_CONNECTION);
            }else{
                printa(ERR_INVALID_INPUT);
                return 0;
            }
            close(newsockfd);
        }
        
    }
    return 0;
}
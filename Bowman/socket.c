#include "socket.h"

#define MAX_CHAR 50
#define printF(x) write(1, x, strlen(x))

int socketFD;

void intHandler2(){
    printa(SIGINT1);
    close(socketFD);
    intHandler();
    //raise(SIGKILL);
}

int reciveTrama(unsigned char * trama, int socketFD){
    int response = 0;
    unsigned char op [256];
    read(socketFD, op, 256);
        if(sizeof(op)==256){
            uint8_t type = trama[0];
            uint16_t header_length = (trama[1] << (8*1)) + trama[2];
            char * header = (char*)malloc(sizeof(char)*header_length);
            memcpy(header, &trama[3], header_length);
            char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
            memcpy(data, &trama[3+header_length], 256-header_length-3);
            
            printF("\nTrama enviada\n");
            printf("Type: %d\n", type);
            printf("Header Length: %u\n", (unsigned int)header_length);
            printf("Header: %s\n", header);
            printf("Data: %s\n", data);
        }else{
            printF(ERR_RECIVE);
        }
    return response;
}

void create_connection(ConfigBowman * configBowman){

    
    struct sockaddr_in servidor;

    signal(SIGINT, intHandler2);

    if( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(configBowman->port);
    if(inet_pton(AF_INET, configBowman->ipServer, &servidor.sin_addr) < 0){
        printF("Error configurant IP\n");
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF("Error fent el connect\n");
    }else{
        unsigned char op [256];
        read(socketFD, op, 256);
        printf("sizeof op: %lu\n", sizeof(op));
        if(sizeof(op)==256){
            uint16_t header_length = (op[1] << (8*1)) + op[2];
            char * header = (char*)malloc(sizeof(char)*header_length);
            memcpy(header, &op[3], header_length);
            char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
            memcpy(data, &op[3+header_length], 256-header_length-3);
            
            printF("\nTrama recibida\n");
            printf("Header Length: %u\n", header_length);
            printf("Header: %s\n", header);
            printf("Data: %s\n", data);
        }else{
            printF(ERR_RECIVE);
        }
        
        
        close(socketFD);
    }
}
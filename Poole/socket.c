#include "socket.h"
int socketFD;
struct sockaddr_in servidor;

unsigned char* generateTrama(char * header, ConfigPoole *configPoole){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    uint16_t size = strlen(header);
    trama[1] = (size >> (8*1)) & 0xff;
    trama[2] = (size >> (8*0)) & 0xff;
    strcpy((char*) &trama[3], header);

    char * data = (char*)malloc(sizeof(char)*256-3-size);
    strcpy(data, configPoole->nomServidor);
    strcat(data, "&");
    strcat(data, configPoole->ipServerPoole);
    strcat(data, "&");
    char buffer[20];
    sprintf(buffer, "%d", configPoole->portPoole-1);
    strcat(data, buffer);
    int sizeData = strlen(data);
    //add data to trama
    memcpy(&trama[3+size], data, sizeData);
    //rellenar lo que falta de data con 0
    bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    
    return trama;
}

int launch_server(ConfigPoole * configPoole){
    char op[MAX_INPUT];
    char buffer[MAX_INPUT];
    int response = 0;

    if( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printF(ERR_SOCKET);
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(configPoole->portDiscovery);

    if(inet_pton(AF_INET, configPoole->ipServerDiscovery, &servidor.sin_addr) < 0){
        printF(ERR_CONFIG_IP);
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF(ERR_CONNECT);
    }else{
        sprintf(buffer,"Connecting %s Server to the system...\n", configPoole->nomServidor);
        write(1, buffer, strlen(buffer));
        unsigned char* trama = generateTrama(NEW_POOLE, configPoole);
        if(write(socketFD, trama, 256)<0){
            printF(ERR_SEND);
        }
        read(socketFD, op, MAX_INPUT);
        while(op[0]!='\0' && strcmp(&op[0],"KO")!=0){
            printa("LLEGA\n");
            response = 1;
            op[0]='\0';
        }
    }
    close(socketFD);

    return response;
}
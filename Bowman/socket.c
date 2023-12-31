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
void printMenu(){
    printF("\n====================\n");
    printF("2. LIST SONGS\n");
    printF("3. LIST PLAYLIST\n");
    printF("4. LOGOUT\n");
    printF("Choose an option: ");
}

unsigned char* generateTrama(char * header, ConfigBowman *configBowman){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    uint16_t size = strlen(header);
    trama[1] = (size >> (8*1)) & 0xff;
    trama[2] = (size >> (8*0)) & 0xff;
    strcpy((char*) &trama[3], header);

    char * data = (char*)malloc(sizeof(char)*256-3-size);
    strcpy(data, configBowman->nom);
    int sizeData = strlen(data);
    //add data to trama
    memcpy(&trama[3+size], data, sizeData);
    //rellenar lo que falta de data con 0
    bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    
    return trama;
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
void connect_Poole(char * data, ConfigBowman * configBowman){
    struct sockaddr_in servidor;

    signal(SIGINT, intHandler2);

    char * nom = strtok(data, "&");
    char * ip = strtok(NULL, "&");
    char * port = strtok(NULL, "&");

    if(ip!=NULL && port != NULL){
        printf("\n====================\n");
        printf("Enviado a Poole\n");
        printf("nom: %s\n", nom);
        printf("ip: %s\n", ip);
        printf("port: %s\n", port);

        if( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            printF(ERR_SOCKET);
        }

        bzero(&servidor, sizeof(servidor));
        servidor.sin_family = AF_INET;
        servidor.sin_port = htons(atoi(port));
        if(inet_pton(AF_INET, ip, &servidor.sin_addr) < 0){
            printF(ERR_CONFIG_IP);
        }

        if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
            printF(ERR_CONNECT);
        }else{
            //envia trama a Poole
            unsigned char* trama = generateTrama(NEW_BOWMAN, configBowman);
            write(socketFD, trama, 256);

            //lee la trama desde Poole
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
            
            
            //close(socketFD);
        }
    }else{
        printF(ERR_RECIVE);
    }
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
        //envia trama a Discovery
        unsigned char* trama = generateTrama(NEW_BOWMAN, configBowman);
        write(socketFD, trama, 256);

        //lee la trama desde Discovery
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
            connect_Poole(data, configBowman);
            if(strcmp(header, CON_OK) != 0){
                perror("Error en la conexion\n");
                raise(SIGKILL);
            }
            int opcio = 0;
            char *input;
            int n_espais = 0;
            char option[MAX_INPUT];
            while(!opcio){
                printMenu();
                input = readUntil(FD_READER, '\n');
                n_espais = prepareData(input, option);
                printa("\nInput: --");
                printa(input);
                printa("--\n");
                printa("\nOption: ");
                printa(option);
                printa("\nEspais: ");
                printf("%d\n", n_espais);
                if(myStrcmp(option, LOGOUT) == 1){           //LOGOUT
                    printa("\nEntra en Logout\n");
                    opcio = 1; 
                }
            }
        }else{
            printF(ERR_RECIVE);
        }
        close(socketFD);
    }
}
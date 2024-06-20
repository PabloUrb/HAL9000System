#include "socket.h"

#define MAX_CHAR 50
#define printF(x) write(1, x, strlen(x))

int socketFD;
char * port;
extern ConfigBowman *configBowman;
int connected = 0;

void intHandler2(){
    printa(SIGINT1);
    printf("connected %d", connected);
    printf("socketFD %d", socketFD);
    if(connected==0){create_connection(configBowman, 1);}
    printf("socketFD %d", socketFD);
    close(socketFD);
    intHandler();
}
void printMenu(){
    printF("\n====================\n");
    printF("2. LIST SONGS\n");
    printF("3. LIST PLAYLIST\n");
    printF("4. LOGOUT\n");
    printF("Choose an option: ");
}
/*void sendsocket(int mq_id, int fd){

    Message msg;
        msg.fd = fd;
        msg.mtype = 1;
        bzero(msg.header, 60);
    if (msgsnd(mq_id, &msg, (2*sizeof(int))+(60*sizeof(char)), 0) == -1) {
        perror("msgsnd");
    }
}*/

unsigned char* generateTrama(char * header, ConfigBowman *configBowman, char * port){
    //int contador_ERR_TRAMA = 0;
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    uint16_t size = strlen(header);
    trama[1] = (size >> (8*1)) & 0xff;
    trama[2] = (size >> (8*0)) & 0xff;
    strcpy((char*) &trama[3], header);
    printf("Type: %d\n", trama[0]);
    printf("Header Length: %d%d\n", trama[1], trama[2]);
    printf("Header: %s\n", &trama[3]);
    printf("Data: %s\n", &trama[3+size]);
    if(configBowman != NULL){
        char * data = (char*)malloc(sizeof(char)*256-3-size);
        strcpy(data, configBowman->nom);
        int sizeData = strlen(data);
        //add data to trama
        memcpy(&trama[3+size], data, sizeData);
        //rellenar lo que falta de data con 0
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
        free(data);
    }else if(port != NULL){
        printf("port 2: %s\n", port);
        char * data = (char*)malloc(sizeof(char)*256-3-size);
        strcpy(data, port);
        strcat(data, "&");
        int sizeData = strlen(data);
        //add data to trama
        memcpy(&trama[3+size], data, sizeData);
        //rellenar lo que falta de data con 0
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
        free(data);
    }else{
        printF(ERR_TRAMA);
    }
    return trama;
    free(trama);
}

int reciveTrama(int socketFD){
    int response = 0;
    unsigned char op2 [256];
    int size = read(socketFD, op2, 256);
        if(size==256){
            uint8_t type = op2[0];
            uint16_t header_length = (op2[1] << (8*1)) + op2[2];
            char * header = (char*)malloc(sizeof(char)*(header_length+1));
            memcpy(header, &op2[3], header_length);
            header[header_length]='\0';
            printF("\nTrama recibida\n");
            printf("Type: %d\n", type);
            printf("Header Length: %u\n", (unsigned int)header_length);
            printf("Header: %s\n", header);
            if(strcmp(header, CON_OK)==0){
                return 1;
            }else{
                char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
                memcpy(data, &op2[3+header_length], 256-header_length-3);
                printf("Data: %s\n", data); 

                if(strcmp(header, SONGS_RESPONSE)==0){
                    write(1, "\n", strlen("\n"));
                    write(1, "====================\n", strlen("====================\n"));
                    write(1, "Songs:\n", strlen("Songs:\n"));
                    int contador = 0;
                    
                    for(int i = 0; i < (int) strlen(data); i++){
                        if(data[i]=='&' ){
                            contador++;
                            write(1, "\n", strlen("\n"));
                        }else{
                            write(1, &data[i], 1);
                        }
                    }
                }else if(strcmp(header, PLAYLISTS_RESPONSE)==0){
                    write(1, "\n", strlen("\n"));
                    write(1, "====================\n", strlen("====================\n"));
                    write(1, "Playlists:\n", strlen("Playlists:\n"));
                    int contador = 0;
                    for(int i = 0; i < (int) strlen(data); i++){
                        if(data[i]=='&' || data[i]=='#'){
                            contador++;
                            write(1, "\n", strlen("\n"));
                        }else{
                            write(1, &data[i], 1);
                        }
                    }

                }else{
                    printF(ERR_TRAMA_HEADER);
                }

                response = 1;
                free(data);
            }
            free(header);
        }else{
            printF(ERR_RECIVE);
            printaInt(3);
            return 0;
        }
        
    return response;
}
int connect_Poole(char * data, ConfigBowman * configBowman){
    struct sockaddr_in servidor;
    int response = 0;
    //int cont_ERR_RECIVE = 0;

    signal(SIGINT, intHandler2);

    char * nom = strtok(data, "&");
    char * ip = strtok(NULL, "&");
    port = strtok(NULL, "&");

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
            perror(ERR_CONNECT);
            return 1;
        }else{
            //envia trama a Poole
            unsigned char* trama = generateTrama(NEW_BOWMAN, configBowman, NULL);
            if(write(socketFD, trama, 256)<0){
                perror(ERR_SEND);
                return 1;
            }
            free(trama);
            //lee la trama desde Poole
            unsigned char op [256];
            read(socketFD, op, 256);
            printf("sizeof op: %lu\n", sizeof(op));
            if(sizeof(op)==256){
                uint16_t header_length = (op[1] << (8*1)) + op[2];
                char * header = (char*)malloc(sizeof(char)*(header_length+1));
                memcpy(header, &op[3], header_length);
                header[header_length] = '\0';
                char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
                memcpy(data, &op[3+header_length], 256-header_length-3);
                
                printF("\nTrama recibida\n");
                printf("Header Length: %u\n", header_length);
                printf("Header: %s\n", header);
                printf("Data: %s\n", data);
            }else{
                printF(ERR_RECIVE);
                printaInt(4);
                return 1;
            }
            
            
            //close(socketFD);
        }
    }else{
        printF(ERR_RECIVE);
        printaInt(2);
        return 1;
    }
    return response;
}

int create_connection(ConfigBowman * configBowman, int flag){

    int cont_ERR_RECIVE = 0;
    int response = 0;
    struct sockaddr_in servidor;

    signal(SIGINT, intHandler2);
    int project_id = getpid() % 255;
    key_t key = ftok("thread.c", project_id);
    int mq_id = msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1) {
        printa(ERR_COLA);
        exit(EXIT_FAILURE);
    }

    if( (socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(configBowman->port);
    if(inet_pton(AF_INET, configBowman->ipServer, &servidor.sin_addr) < 0){
        printF("Error configurant IP\n");
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        perror(ERR_SEND);
        return 1;
    }else{
        if(flag == 0){
        
        //envia trama a Discovery
        printf("Enviada trama a Discovery\n");
        unsigned char* trama = generateTrama(NEW_BOWMAN, configBowman, NULL);
        write(socketFD, trama, 256);

        //lee la trama desde Discovery
        unsigned char op [256];
        read(socketFD, op, 256);
        
        printf("sizeof op: %lu\n", sizeof(op));
        if(sizeof(op)==256){
            uint16_t header_length = (op[1] << (8*1)) + op[2];
            char * header = (char*)malloc(sizeof(char)*(header_length+1));
            memcpy(header, &op[3], header_length);
            header[header_length] = '\0';
            if(strcmp(header, CHECK_OK)){
                char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
                memcpy(data, &op[3+header_length], 256-header_length-3);
                
                printF("\nTrama recibida\n");
                printf("Header Length: %u\n", header_length);
                printf("Header: %s\n", header);
                printf("Data: %s\n", data);
                response = connect_Poole(data, configBowman);
                if(response==1){
                    return 1;
                }
                
                //sendsocket(mq_id, socketFD);
                if(strcmp(header, CON_OK) != 0){
                    perror(ERR_CONNECT);
                    raise(SIGKILL);
                }
                create_hilos(socketFD);
                int opcio = 0;
                char *input;
                int n_espais = 0;
                char option[MAX_INPUT];
                printaInt(socketFD);
                while(!opcio){
                    sleep(1);
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
                    printf("cont_ERR_RECIVE: %d\n", cont_ERR_RECIVE);
                    if(cont_ERR_RECIVE < 2){
                        if(strcmp(option, LOGOUT) == 0){           //LOGOUT
                            printa("\nEntra en Logout\n");
                            unsigned char* trama = generateTrama(EXIT, NULL, port);
                            write(socketFD, trama, 256);
                            free(trama);
                            if(reciveTrama(socketFD)==1){
                                close(socketFD);
                                create_connection(configBowman, 1);
                                close(socketFD);
                            }else{
                                cont_ERR_RECIVE++;
                            }
                            connected = 1;
                            opcio = 1; 
                        }else if(strcmp(option, LIST) == 0 && n_espais >= 1 && strcmp(input, SONGS)==0){            //LIST
                            printa("\nEntra en List Songs\n");
                            unsigned char* trama = generateTrama(LIST_SONGS, NULL, port);
                            write(socketFD, trama, 256);
                            free(trama);
                            printf("fd: %d\n", socketFD);
                            /*if(reciveTrama(socketFD)!=1){
                                cont_ERR_RECIVE++;
                            }*/
                            
                        }else if(strcmp(option, LIST) == 0 && n_espais >= 1 && strcmp(input, PLAYLIST)==0){            //LIST
                            printa("\nEntra en List Playlists\n");
                            unsigned char* trama = generateTrama(LIST_PLAYLISTS, NULL, port);
                            write(socketFD, trama, 256);
                            free(trama);
                            /*if(reciveTrama(socketFD)!=1){
                                cont_ERR_RECIVE++;
                            }*/
                        }else{
                            perror(ERR_INVALID_INPUT);
                            break;
                        }
                    }else{
                        printf("cont_ERR_RECIVE: %d\n", cont_ERR_RECIVE);
                        close(socketFD);
                        opcio = 1;
                        connected = 1;
                    }
                    free(input);
                }
            }else{
                printF(ERR_COMUNICATION);
                return 1;
            }
        }else{
            perror(ERR_RECIVE);
            printaInt(1);
            return 1;
        }
        //close(socketFD);
        }else if(flag == 1){
            if(port != NULL){
                unsigned char* trama = generateTrama(DISCONECT_BOWMAN, NULL, port);
                write(socketFD, trama, 256);
            }
            
        }else{
            close(socketFD);
            printF(ERR_FLAG);
            return 1;
        }
        close(socketFD);
    }
    return response;
}
#include "socket.h"

#define MAX_CHAR 50
#define printF(x) write(1, x, strlen(x))


void create_connection(ConfigBowman * configBowman){

    int socketFD;
    struct sockaddr_in servidor;
    char op[MAX_CHAR];
    long nRead;

    if( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(configBowman->port);
    printf("port : %d\n", configBowman->port);
    if(inet_pton(AF_INET, configBowman->ipServer, &servidor.sin_addr) < 0){
        printF("Error configurant IP\n");
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF("Error fent el connect\n");
    }else{
        write(socketFD, configBowman, sizeof(ConfigBowman));
        /*while(op[0]!='\0' && strcmp(&op[0],"KO")!=0){
            nRead = read(socketFD,op,sizeof(op));
            op[nRead-1] = '\0';
            write(1,op,strlen(op));
            //valor = passwordManager(socketFD);
        }
        close(socketFD);*/
    }
}
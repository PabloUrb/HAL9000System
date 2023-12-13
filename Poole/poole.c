#include "poole.h"

#define MAX_CHAR 50
#define printF(x) write(1, x, strlen(x))

Config *config;

void intHandler(){
    freeAllMem(config);
    raise(SIGKILL);
}

int main(int argc, char *argv[]){
    int socketFD;
    struct sockaddr_in servidor;
    char op[MAX_CHAR];
    long nRead;

    signal(SIGINT, intHandler);

    if (argc != 2)
    {
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    config = llegirFitxer(argv[1]);
    
    if (config == NULL)
    {
        perror("ERROR2: Input invalid\n");
        return 0;
    }

    if( (socketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(config->portDiscovery);

    if(inet_pton(AF_INET, config->ipServerDiscovery, &servidor.sin_addr) < 0){
        printF("Error configurant IP\n");
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF("Error fent el connect\n");
    }
        while(op[0]!='\0' && strcmp(&op[0],"KO\n")!=0){
            nRead = read(socketFD,op,sizeof(op));
            op[nRead-1] = '\0';
            write(1,op,strlen(op));
            //valor = passwordManager(socketFD);
        }

    freeAllMem(config);
    exit(0);
}
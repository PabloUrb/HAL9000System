#include "poole.h"

#define MAX_CHAR 50
#define printF(x) write(1, x, strlen(x))

ConfigPoole *configPoole;
int socketFD;
struct sockaddr_in servidor;

void intHandler(){
    close(socketFD);
    freeAllMemPoole(configPoole);
    raise(SIGKILL);
}
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
    printa("\n");
    printf("data : %s",data);
    int sizeData = strlen(data);
    //add data to trama
    memcpy(&trama[3+size], data, sizeData);
    //rellenar lo que falta de data con 0
    bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    
    printf("data : %s",data);
    printa("\n");
    return trama;
}

int main(int argc, char *argv[]){
    
    char op[MAX_CHAR];
    long nRead;
    
    signal(SIGINT, intHandler);

    if (argc != 2)
    {
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    configPoole = llegirFitxerPoole(argv[1]);
    if (configPoole == NULL)
    {
        perror("ERROR2: Input invalid\n");
        return 0;
    }

    if( (socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printF("Error creant el socket\n");
    }

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(configPoole->portDiscovery);

    if(inet_pton(AF_INET, configPoole->ipServerDiscovery, &servidor.sin_addr) < 0){
        printF("Error configurant IP\n");
    }

    if(connect(socketFD, (struct sockaddr*) &servidor, sizeof(servidor)) < 0){
        printF("Error fent el connect\n");
    }else{
        printf("Conectado con %s:%d\n",inet_ntoa(servidor.sin_addr),htons(servidor.sin_port));
        unsigned char* trama = generateTrama("NEW_POOLE", configPoole);
        if(write(socketFD, trama, 256)<0){
            printF("Error fent el send\n");
        }
        while(op[0]!='\0' && strcmp(&op[0],"KO")!=0){
            nRead = read(socketFD,op,sizeof(op));
            op[nRead-1] = '\0';
            write(1,op,strlen(op));
            //valor = passwordManager(socketFD);
        }
    }
    close(socketFD);
    freeAllMemPoole(configPoole);
    exit(0);
}
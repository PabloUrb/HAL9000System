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
    printa("Sigint\n");
    //write(newsockfd, "KO\n", myStrlen("KO\n"));
    shutdown(newsockfd, SHUT_RDWR);
    shutdown(sockfd, SHUT_RDWR);
    raise(SIGKILL);
}


void reciveTrama(){
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
        printa("\n");
        
        free(trama1.header);
        free(trama1.data);
    }
    
    
    
}

int launch_server(int port, char * ip, ConfigDiscovery * configDiscovery) {
    
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    signal(SIGINT, intHandler2);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    tramas = (Trama*)malloc(sizeof(Trama)*100);
    if (sockfd < 0) {
        printa("Error creant el socket\n");
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);
        
        if (bind(sockfd, (struct sockaddr *) &serv_addr,
                 sizeof(serv_addr)) < 0){
            perror("ERROR on binding");
            return 2;
        }

    if(listen(sockfd, 10) < 0){
        printa("Error fent el listen\n");
    }else{
        clilen = sizeof(cli_addr);
        while (1)
        {
            
            printa("Esperant nova conexio...\n");
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if(newsockfd<0){
                printa("ERROR on accept");
                return 3;
            }

            if(port == configDiscovery->portPoole){
                printa("Nova connexio Poole entrant\n");
                reciveTrama();
                printa("Envia KO\n"); 
                write(newsockfd, "KO\n", myStrlen("KO\n"));
            }else if(port == configDiscovery->portBowman){
                printa("Nova connexio Bowman entrant\n");
                //write(newsockfd, "eres tonto2\n", myStrlen("eres tonto2\n"));
            }else{
                printa("ERROR: Input invalid\n");
                return 0;
            }
            printa("Connexio cerrada\n");
            close(newsockfd);
        }
        
    }
    return 0;
}
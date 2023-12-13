#include "socket.h"

int sockfd, newsockfd;

void initSockets(){
    printa("Init Sockets\n");
}

void intHandler2(){
    printa("Sigint\n");
    close(newsockfd);
    close(sockfd);
    raise(SIGKILL);
}

int launch_server(int port, char * ip) {
    
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    signal(SIGINT, intHandler2);

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
            }
            printa("Nova connexio entrant\n");
            write(newsockfd, "eres tonto\n", myStrlen("eres tonto\n"));
        }
        
    }
    return 0;
}
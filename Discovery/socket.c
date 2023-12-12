#include "socket.h"

void initSockets(){
    printa("Init Sockets\n");
}

int launch_server(int port, char * ip, int * socket_fd) {
    struct sockaddr_in s_addr;

    *socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*socket_fd < 0) {
        printa("Error creant el socket\n");
        return -1;
    }

    bzero(&s_addr, sizeof (s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (port);
    s_addr.sin_addr.s_addr = inet_addr(ip);
    if (bind (*socket_fd, (void *) &s_addr, sizeof (s_addr)) < 0) {
        printa("Error fent el bind\n");
        return -1;
    }

    if(listen(*socket_fd, 10) < 0){
        printa("Error fent el listen\n");
    }
    return 0;
}
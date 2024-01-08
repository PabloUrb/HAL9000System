#define _POSIX_C_SOURCE 200112L
#include "socket.h"

int socketFD;
struct sockaddr_in servidor;
struct epoll_event events[MAXEVENTS];
extern ConfigPoole *configPoole;




int reciveTrama(unsigned char * trama){
    int response = 0;
    unsigned char op [256];
    read(socketFD, op, 256);
        if(sizeof(op)==256){
            uint16_t header_length = (trama[1] << (8*1)) + trama[2];
            char * header = (char*)malloc(sizeof(char)*header_length);
            memcpy(header, &trama[3], header_length);
            if(strcmp(header, CON_OK)){
                response = 1;
            }else{
                printF(ERR_RECIVE);
            }
        }else{
            printF(ERR_RECIVE);
        }
    return response;
}

unsigned char* generateTrama(char * header, ConfigPoole *configPoole){
    unsigned char* trama = (unsigned char*)malloc(sizeof(unsigned char)*256);
    trama[0] = 0x01;
    uint16_t size = strlen(header);
    trama[1] = (size >> (8*1)) & 0xff;
    trama[2] = (size >> (8*0)) & 0xff;
    strcpy((char*) &trama[3], header);
    
    if(configPoole != NULL){
        char * data = (char*)malloc(sizeof(char)*256-3-size);
        strcpy(data, configPoole->nomServidor);
        strcat(data, "&");
        strcat(data, configPoole->ipServerPoole);
        strcat(data, "&");
        char buffer[20];
        sprintf(buffer, "%d", configPoole->portPoole);
        strcat(data, buffer);
        int sizeData = strlen(data);
        //add data to trama
        memcpy(&trama[3+size], data, sizeData);
        //rellenar lo que falta de data con 0
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    }else{
        bzero(&trama[3+size], 256-3-size);
    }
    printf("    header: %s\n", header);
    return trama;
    free(trama);
}
void listSongs(int mq_id, int fd, char * header){

    Message msg;
        msg.fd = fd;
        msg.mtype = 1;
        if(strcmp(header, LIST_SONGS)==0){
            snprintf(msg.header, 60, LIST_SONGS);
        }else if(strcmp(header, LIST_PLAYLISTS)==0){
            snprintf(msg.header, 60, LIST_PLAYLISTS);
        }else{
            snprintf(msg.header, 60, ERR_TRAMA_HEADER);
        }
        printf("    header: %s\n", msg.header);
    if (msgsnd(mq_id, &msg, (2*sizeof(int))+(60*sizeof(char)), 0) == -1) {
        perror("msgsnd");
    }
}

void reciveTramaPoole(char trama[256], int fd, int mq_id){
    printa("    Recive Trama\n");
    int error = 1;
    unsigned char* trama2 = (unsigned char*)malloc(sizeof(unsigned char)*256);
 
        uint8_t type = trama[0];
        uint16_t header_length = (trama[1] << (8*1)) + trama[2];
        char * header = (char*)malloc(sizeof(char)*(header_length+1));
        memcpy(header, &trama[3], header_length);
        header[header_length]='\0';
        char * data = (char*)malloc(sizeof(char)*(256-header_length-3));
        memcpy(data, &trama[3+header_length], 256-header_length-3);
        //data[256-header_length-3]
        printf("    Type: %d\n", type);
        printf("    Header Length: %u\n", header_length);
        printf("    Header: %s\n", header);
        
        if(strcmp(header, NEW_BOWMAN)==0){
            trama2 = generateTrama(CON_OK, NULL);
        }else if(strcmp(header, LIST_SONGS)==0){
            listSongs(mq_id, fd, LIST_SONGS);
        }else if(strcmp(header, LIST_PLAYLISTS)==0){
            listSongs(mq_id, fd, LIST_PLAYLISTS);
        }else if(strcmp(header, EXIT)==0){
            trama2 = generateTrama(CON_OK, NULL);
        }else{
            printF(ERR_TRAMA_HEADER);
            trama2 = generateTrama(CON_KO, NULL);
            error = 0;
        }
        free(data);
        
    if(strcmp(header, NEW_BOWMAN)==0 || error == 0){
        if(write(fd, trama2, 256)<0){
            printF(ERR_SEND);
        }else{
            printf("    trama enviada\n");
        }
    }
    free(header);
    free(trama2);
}



void intHandler2(){
    launch_server(configPoole, 1);
    close(socketFD);
    intHandler();
}

int launch_server(ConfigPoole * configPoole, int flag){
    
    char buffer[MAX_INPUT];
    int response = 0;
    signal(SIGINT, intHandler2);
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
        unsigned char* trama;
        if(flag == 0){
            sprintf(buffer,"\nConnecting %s Server to the system...\n", configPoole->nomServidor);
            write(1, buffer, strlen(buffer));
            trama = generateTrama(NEW_POOLE, configPoole);
            if(write(socketFD, trama, 256)<0){
                printF(ERR_SEND);
            }
            if(reciveTrama(trama)==1){
                response = 1;
            }
        }else if(flag == 1){
            sprintf(buffer, "Notifying the system of our disconnection...\n");
            write(1, buffer, strlen(buffer));
            trama = generateTrama(DISCONECT_POOLE, configPoole);
            if(write(socketFD, trama, 256)<0){
                printF(ERR_SEND);
            }
        }
    }
    close(socketFD);

    return response;
}
static void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl()\n");
		exit(1);
	}
}

static void set_sockaddr(struct sockaddr_in *addr, int port)
{
	bzero((char *)addr, sizeof(struct sockaddr_in));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons(port);
}

static int setnonblocking(int sockfd)
{
	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) ==
	    -1) {
		return -1;
	}
	return 0;
}

void launch_Poole(ConfigPoole *configPoole){
    int i;
	int n;
	int epfd;
	int nfds;
	int listen_sock;
	int conn_sock;
	socklen_t  socklen;
	char buf[256];
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;

    key_t key = ftok("thread.c", 1);
    int mq_id = msgget(key, IPC_CREAT | 0666);
    if (mq_id == -1) {
        printF(ERR_COLA);
        exit(EXIT_FAILURE);
    }
	

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    char * test = (char*)malloc(sizeof(char)*4);
    sprintf(test,"%d",configPoole->portPoole);

	set_sockaddr(&srv_addr, atoi(test));
	bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

	setnonblocking(listen_sock);
	listen(listen_sock, MAX_CONN);

	epfd = epoll_create(1);
	epoll_ctl_add(epfd, listen_sock, EPOLLIN | EPOLLOUT | EPOLLET);

	socklen = sizeof(cli_addr);
	for (;;) {
		nfds = epoll_wait(epfd, events, MAXEVENTS, -1);
		for (i = 0; i < nfds; i++) {
			if (events[i].data.fd == listen_sock) {
				/* handle new connection */
				conn_sock =
				    accept(listen_sock,(struct sockaddr *) &cli_addr, &socklen);

				inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr),
					  buf, sizeof(cli_addr));
				printf("[+] connected with %s:%d\n", buf,
				       ntohs(cli_addr.sin_port));

				setnonblocking(conn_sock);
				epoll_ctl_add(epfd, conn_sock,
					      EPOLLIN | EPOLLET | EPOLLRDHUP |
					      EPOLLHUP);
			} else if (events[i].events & EPOLLIN) {
				/* handle EPOLLIN event */
				while(1) {
					bzero(buf, sizeof(buf));
					n = read(events[i].data.fd, buf, sizeof(buf));
					if (n <= 0 ) {
						break;
					} else {
						printf("\n\n[+] data: %s\n", buf);
                        reciveTramaPoole(buf, events[i].data.fd, mq_id);
					}
                }
			} else {
				printf("\n[+] unexpected\n");
			}
			/* check if the connection is closing */
			if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				printf("\n[+] connection closed\n");
				epoll_ctl(epfd, EPOLL_CTL_DEL,
					  events[i].data.fd, NULL);
				close(events[i].data.fd);
				continue;
			}
		}
	}
    
}
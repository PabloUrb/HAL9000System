#define _POSIX_C_SOURCE 200112L
#include "socket.h"

int socketFD;
struct sockaddr_in servidor;
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
        printf("data: %s\n", data);
        //rellenar lo que falta de data con 0
        bzero(&trama[3+size+sizeData], 256-3-size-sizeData);
    }else{
        bzero(&trama[3+size], 256-3-size);
    }
    
    
    return trama;
}

void intHandler2(){
    launch_server(configPoole, 1);
    close(socketFD);
    intHandler();
}

int launch_server(ConfigPoole * configPoole, int flag){
    
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
        unsigned char* trama;
        if(flag == 0){
            sprintf(buffer,"Connecting %s Server to the system...\n", configPoole->nomServidor);
            write(1, buffer, strlen(buffer));
            trama = generateTrama(NEW_POOLE, configPoole);
            if(write(socketFD, trama, 256)<0){
                printF(ERR_SEND);
            }
        }
        if(flag == 1){
            sprintf(buffer,"Notifying %s Server, disconect system...\n", configPoole->nomServidor);
            write(1, buffer, strlen(buffer));
            trama = generateTrama(DISCONECT, configPoole);
            write(socketFD, trama, 256);
        }
        if(reciveTrama(trama)==1){
                response = 1;
            }
    }
    close(socketFD);

    return response;
}
static int make_socket_non_blocking (int sfd){
    int flags, s;
    
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
        {
        perror ("fcntl");
        return -1;
        }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
        {
        perror ("fcntl");
        return -1;
        }

    return 0;
}
static int create_and_bind (char * port){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
        {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return -1;
        }

    for (rp = result; rp != NULL; rp = rp->ai_next)
        {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
            {
            /* We managed to bind successfully! */
            break;
            }

        close (sfd);
        }

    if (rp == NULL)
        {
        fprintf (stderr, "Could not bind\n");
        return -1;
        }

    freeaddrinfo (result);

    return sfd;
}
void launch_Poole(ConfigPoole *configPoole){
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;

    signal(SIGINT, intHandler2);

    char * test = (char*)malloc(sizeof(char)*4);
    sprintf(test,"%d",configPoole->portPoole);

    sfd = create_and_bind (test);
    if(sfd == -1){
        abort();
    }

    s = make_socket_non_blocking(sfd);
    if(s == -1){
        abort();
    }

    s = listen(sfd, BACKLOG);
    if(s == -1){
        perror("listen");
        abort();
    }
    signal(SIGINT, intHandler2);
    printf("server: waiting for connections...\n");

    efd = epoll_create1(0);
    if(efd == -1){
        perror("epoll_create");
        abort();
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if(s == -1){
        perror("epoll_ctl");
        abort();
    }

    /* Buffer where events are returned */
    events = calloc (MAXEVENTS, sizeof event);

    /* The event loop */
    while (1){
        int n, i;

        n = epoll_wait (efd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++)
        {
            if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
                /* An error has occured on this fd, or the socket is not
                ready for reading (why were we notified then?) */
                fprintf (stderr, "epoll error\n");
                close (events[i].data.fd);
                continue;
            }else if(sfd == events[i].data.fd){
                /* We have a notification on the listening socket, which
                means one or more incoming connections. */
                while (1){
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                    in_len = sizeof in_addr;
                    infd = accept (sfd, &in_addr, &in_len);
                    if (infd == -1){
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                            /* We have processed all incoming
                            connections. */
                            break;
                        }else{
                            perror ("accept");
                            break;
                        }
                    }

                    s = getnameinfo (&in_addr, in_len,
                                    hbuf, sizeof hbuf,
                                    sbuf, sizeof sbuf,
                                    NI_NUMERICHOST | NI_NUMERICSERV);
                    if (s == 0){
                        printf("Accepted connection on descriptor %d "
                                "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                    /* Make the incoming socket non-blocking and add it to the
                    list of fds to monitor. */
                    s = make_socket_non_blocking (infd);
                    if (s == -1)
                        abort ();

                    event.data.fd = infd;
                    event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1){
                        perror ("epoll_ctl");
                        abort ();
                    }
                }
                continue;
            }else{
                /* We have data on the fd waiting to be read. Read and
                display it. We must read whatever data is available
                completely, as we are running in edge-triggered mode
                and won't get a notification again for the same
                data. */
                int done = 0;

                while (1){
                    ssize_t count;
                    char buf[512];

                    count = read (events[i].data.fd, buf, sizeof buf);
                    if (count == -1){
                        /* If errno == EAGAIN, that means we have read all
                        data. So go back to the main loop. */
                        if (errno != EAGAIN){
                            perror ("read");
                            done = 1;
                        }
                        break;
                    }else if (count == 0){
                        /* End of file. The remote has closed the
                        connection. */
                        done = 1;
                        break;
                    }

                    /* Write the buffer to standard output */
                    s = write (1, buf, count);
                    if (s == -1){
                        perror ("write");
                        abort ();
                    }
                }

                if (done){
                    printf ("Closed connection on descriptor %d\n",
                            events[i].data.fd);

                    /* Closing the descriptor will make epoll remove it
                    from the set of descriptors which are monitored. */
                    close (events[i].data.fd);
                }
            }
        }
        
    }
    free(events);
    close(sfd);
    
}
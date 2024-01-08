#ifndef _socket_h_
#define _socket_h_

#include "poole.h"
#include "../tools.h"
#include "../defines.h"
#include "thread.h"

#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <strings.h>
#include <signal.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>

void initSockets();

int launch_server(ConfigPoole * configPoole, int port);
void launch_Poole(ConfigPoole *configPoole);

#endif
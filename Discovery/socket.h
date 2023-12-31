#ifndef _socket_h_
#define _socket_h_

#include "discovery.h"
#include "../tools.h"
#include "../defines.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>

void initSockets();

int launch_server(int port, char * ip, ConfigDiscovery * configDiscovery);

#endif
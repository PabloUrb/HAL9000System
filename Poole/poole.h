#ifndef _poole_h_
#define _poole_h_

#include "tools.h"

#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <dirent.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>

#define FD_READER 0
#define FD_SPEAKER 1

#define CONNECT "connect\0"
#endif
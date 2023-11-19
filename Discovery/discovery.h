#ifndef _discovery_h_
#define _discovery_h_

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

#define FD_READER 0
#define FD_SPEAKER 1

#define CONNECT "connect\0"
#endif
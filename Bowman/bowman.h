#ifndef _bowman_h_
#define _bowman_h_

#include "tools.h"
#include "mainFunctions.h"

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
#define LOGOUT "logout\0"
#define LIST "list\0"
#define DOWNLOAD "download\0"
#define CHECK "check\0"
#define CLEAR "clear\0"
#define DOWNLOADS "downloads\0"

#endif
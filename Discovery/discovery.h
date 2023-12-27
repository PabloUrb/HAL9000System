#ifndef _discovery_h_
#define _discovery_h_

#include "../defines.h"
#include "socket.h"
#include "../tools.h"

#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>

void intHandler();

#endif
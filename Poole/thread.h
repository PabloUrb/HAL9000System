#ifndef _thread_h_
#define _thread_h_
#define  _GNU_SOURCE

#include "poole.h"
#include "../tools.h"
#include "../defines.h"
#include "socket.h"

#define _OPEN_SYS
#define _OPEN_THREADS
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/msg.h>
#include<math.h>

void create_hilos();
char * leer_canciones();

#endif
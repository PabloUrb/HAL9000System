#ifndef _tools_h_
#define _tools_h_

#include "bowman.h"
#include "mainFunctions.h"
#include "socket.h"

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

#define MAX_INPUT 100

typedef struct
{
    char * nom;
    char *nom_carpeta;
    char *ipServer;
    int port;
} Config;

char *read_until(int fd, char delimiter);
char *readUntil(int fd, char delimiter);
void printa(char * cadena);
void printaInt(int i);
int myStrlen(char * cadena);
int myStrcmp(char * cad1, char * cad2);
int myStrcasecmp(char * cad1, char * cad2);
void freeAllMem(Config * config);
Config *llegirFitxerBowman(char * fileConfig);
int prepareData(char * input, char option[MAX_INPUT]);

#endif
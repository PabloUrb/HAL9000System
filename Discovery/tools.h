#ifndef _tools_h_
#define _tools_h_

#include "discovery.h"

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
    char *ipServerPoole;
    int portPoole;
    char *ipServerBowman;
    int portBowman;
} Config;

char *read_until(int fd, char delimiter);
void printa(char * cadena);
void printaInt(int i);
Config *llegirFitxer(char * fileConfig);
void freeAllMem(Config * config);
int myStrlen(char * cadena);
int myStrcmp(char * cad1, char * cad2);
int prepareData(char * input, char option[MAX_INPUT]);
int extreuCadena (char * origen, int posicio, char limit, char option[MAX_INPUT]);
void myToLowerCase(char * input);
#endif
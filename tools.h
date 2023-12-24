#ifndef _tools_h_
#define _tools_h_

#include "defines.h"

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


typedef struct{
    char *ipServerPoole;
    int portPoole;
    char *ipServerBowman;
    int portBowman;
} ConfigDiscovery;

typedef struct{
    char * nom;
    char *nom_carpeta;
    char *ipServer;
    int port;
} ConfigBowman;

typedef struct
{
    char * nomServidor;
    char *nom_carpeta;
    char *ipServerDiscovery;
    int portDiscovery;
    char *ipServerPoole;
    int portPoole;
    
} ConfigPoole;




char *read_until(int fd, char delimiter);
char *readUntil(int fd, char delimiter);
void printa(char * cadena);
void printaInt(int i);
int myStrlen(char * cadena);
int myStrcmp(char * cad1, char * cad2);
int myStrcasecmp(char * cad1, char * cad2);
int prepareData(char * input, char option[MAX_INPUT]);
void freeAllMemBowman(ConfigBowman * configBowman);
ConfigBowman *llegirFitxerBowman(char * fileConfig);
void freeAllMemPoole(ConfigPoole * config);
ConfigPoole *llegirFitxerPoole(char * fileConfig);
void freeAllMemDiscovery(ConfigDiscovery * config);
ConfigDiscovery *llegirFitxerDiscovery(char * fileConfig);

#endif
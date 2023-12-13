#define _POSIX_C_SOURCE 200809L
#include "discovery.h"
#include "socket.h"

#define MAX_CHAR 50

Config *config;
pthread_t thread_pare;

void intHandler(){
    void * res;
    printa("Sigint\n");
    pthread_kill(thread_pare, 9);
    pthread_join (thread_pare,&res);
    freeAllMem(config);
    raise(SIGKILL);
}
static void *threadFunc (void *arg){
    char *s = (char *) arg;
    printf ("%s", s);
    return (void *) strlen (s);
}

int main(int argc, char *argv[]){
    void *res;
    int s;

    signal(SIGINT, intHandler);

    if (argc != 2)
    {
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    config = llegirFitxer(argv[1]);
    
    if (config == NULL)
    {
        perror("ERROR2: Input invalid\n");
        return 0;
    }
    
    s = pthread_create (&thread_pare, NULL, threadFunc, "Hello world\n");
    if (s != 0) printa("Error thread not created");
    printf ("Missatge des del main()\n");

    launch_server(config->portPoole, config->ipServerPoole);

    pthread_join (thread_pare,&res);
    printf ("Thread retorna %ld\n", (long)res);

    freeAllMem(config);
    exit(0);
}
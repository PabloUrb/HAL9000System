#define _POSIX_C_SOURCE 200809L
#include "discovery.h"
#include "socket.h"

Config *config;
pthread_t thread1, thread2;


void intHandler(){
    printa("Sigint\n");
    //cerrar threads
    close(thread1);
    close(thread2);
    freeAllMem(config);
    raise(SIGKILL);
}
void *threadFunc (void *arg){
    int i = (uintptr_t)arg;
    printaInt(i);
    if(i==1){
        launch_server(config->portPoole, config->ipServerPoole);
    }else if(i==2){
        launch_server(config->portBowman, config->ipServerBowman);
    }else{
        printa("ERROR: Input invalid\n");
        return NULL;
    }
    return NULL;
}
int main(int argc, char *argv[]){

    signal(SIGINT, intHandler);

    if(argc != 2){
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    config = llegirFitxer(argv[1]);
    
    if(config == NULL){
        perror("ERROR2: Input invalid\n");
        return 0;
    }
    
    printf ("Missatge des del main()\n");

    //crear dos fils
    
    int s;
    void *res;
    s = pthread_create(&thread1, NULL, threadFunc, (void*)(uintptr_t)1);
    if (s != 0){
        perror("ERROR3: Input invalid\n");
        return 0;
    }
    s = pthread_create(&thread2, NULL, threadFunc, (void*)(uintptr_t)2);
    if (s != 0){
        perror("ERROR4: Input invalid\n");
        return 0;
    }
    s = pthread_join(thread1, &res);
    if (s != 0){
        perror("ERROR5: Input invalid\n");
        return 0;
    }
    s = pthread_join(thread2, &res);
    if (s != 0){
        perror("ERROR6: Input invalid\n");
        return 0;
    }
    free(res);
    close(thread1);
    close(thread2);
    freeAllMem(config);
    exit(0);
}
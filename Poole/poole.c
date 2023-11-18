#include "poole.h"

#define MAX_CHAR 50

Config *config;

void intHandler(){
    freeAllMem(config);
    raise(SIGKILL);
}

int main(int argc, char *argv[]){

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
    freeAllMem(config);
    exit(0);
}
#include "poole.h"

ConfigPoole *configPoole;

void intHandler(){
    freeAllMemPoole(configPoole);
    raise(SIGKILL);
}

int main(int argc, char *argv[]){
    
    signal(SIGINT, intHandler);

    if (argc != 2)
    {
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    configPoole = llegirFitxerPoole(argv[1]);
    if (configPoole == NULL)
    {
        perror("ERROR2: Input invalid\n");
        return 0;
    }
    if(launch_server(configPoole)==1){
        printF(CONNECTED);
    }else{
        printF(ERR_COMUNICATION);
    }
    
    
    freeAllMemPoole(configPoole);
    exit(0);
}
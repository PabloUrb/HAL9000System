#include "bowman.h"

#define MAX_CHAR 50

Config *config;

void intHandler(){
    freeAllMem(config);
    raise(SIGKILL);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, intHandler);
    int opcio = 0;
    char *input;
    char option[MAX_INPUT];
    int n_espais = 0;

    if (argc != 2)
    {
        perror("ERROR1: Input invalid\n");
        return 0;
    }
    config = llegirFitxer(argv[1]);
    while(!opcio){
        input = readUntil(FD_READER, '\n');
        n_espais = prepareData(input, option);
        /*printa("\nInput: --");
        printa(input);
        printa("--\n");

        printa("\nOption: ");
        printa(option);
        printa("\nEspais: "); 
        printaInt(n_espais);*/
        if(myStrcmp(option, CONNECT) == 1){               //CONNECT
            printa("\nEntra en Connect\n");
        }else if(myStrcmp(option, LOGOUT) == 1){           //LOGOUT
            printa("\nEntra en Logout\n");
            opcio = 1; 
        }else if(myStrcmp(option, LIST) == 1 && n_espais == 1){            //LIST
            printa("\nEntra en List\n"); 
        }else if(myStrcmp(option, DOWNLOAD) == 1 && n_espais == 1){            //DOWNLOAD
            printa("\nEntra en Download\n"); 
        }else if(myStrcmp(option, CHECK) == 1 && (myStrcmp(input, DOWNLOADS) == 1)){            //CHECK DOWNLOADS
            checkDownloads(n_espais, option, input);
        }else if(myStrcmp(option, CLEAR) == 1 && (myStrcmp(input, DOWNLOADS) == 1)){            //CLEAR DOWNLOADS
            clearDownloads(n_espais, option, input);
        }else{
            printa("Comanda KO. Comanda no correcte\n");
        }
        free(input);
    }
    freeAllMem(config);
    exit(0);
}
#include "bowman.h"

#define MAX_CHAR 50

ConfigBowman *configBowman;

void intHandler(){
    printf("\n\nSaliendo del programa\n");
    freeAllMemBowman(configBowman);
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
    configBowman = llegirFitxerBowman(argv[1]);
    while(!opcio){ 
        printa("\n$ ");
        input = readUntil(FD_READER, '\n');
        n_espais = prepareData(input, option);
        printa("\nInput: --");
        printa(input);
        printa("--\n");

        printa("\nOption: ");
        printa(option);
        printa("\nEspais: ");
        if(myStrcmp(option, CONNECT) == 1){               //CONNECT
            //connectServer(config);
            int response = connectServer(configBowman);
            if(response == 1){
                continue;
            }
        }else if(myStrcmp(option, LOGOUT) == 1){           //LOGOUT
            printa("\nEntra en Logout\n");
            opcio = 1; 
        }else if(myStrcmp(option, LIST) == 1 && n_espais >= 1){            //LIST
            printa("\nEntra en List\n"); 
        }else if(myStrcmp(option, DOWNLOAD) == 1 && n_espais >= 1){            //DOWNLOAD
            printa("\nEntra en Download\n"); 
        }else if(myStrcmp(option, CHECK) == 1 && n_espais >= 1){            //CHECK DOWNLOADS
            if((myStrcmp(input, DOWNLOADS) == 1)){
                checkDownloads(n_espais, option, input);
            }else{
                printa("Comanda KO. Quizás querias decir Downloads?\n");
            }
        }else if(myStrcmp(option, CLEAR) == 1 && n_espais >= 1){        //CLEAR DOWNLOADS
            if(myStrcmp(input, DOWNLOADS) == 1){
                    clearDownloads(n_espais, option, input);
            }else{
                printa("Comanda KO. Quizás querias decir Downloads?\n");
            }          
            
        }else{
            printa("Comanda KO. Comanda no correcte\n");
        }
        free(input);
    }
    freeAllMemBowman(configBowman);
    exit(0);
}
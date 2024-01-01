#include "mainFunctions.h"

void initMainFunctions(){
    printa("Init Main Functions\n");
}
int connectServer(ConfigBowman * configBowman){
    printa("\nEntra en Connect\n");
    return create_connection(configBowman, 0);
}
void checkDownloads(){
    printa("\nEntra en Check Downloads\n");
}
void clearDownloads(){
    printa("\nEntra en Check Downloads\n");
}
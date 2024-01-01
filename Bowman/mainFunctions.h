#ifndef _mainFunctions_h_
#define _mainFunctions_h_

#include "socket.h"
#include "../tools.h"

void initMainFunctions();
int connectServer(ConfigBowman * configBowman);
void checkDownloads();
void clearDownloads();

#endif
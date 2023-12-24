#ifndef _defines_h_
#define _defines_h_

#define READING_FILE "Reading configuration file"
#define WAITING_CONNECTIONS "Waiting for connections..."
#define ERR_INVALID_INPUT "ERROR: Input invalid\n"
#define SIGINT "SIGINT received\n"

#define FD_READER 0
#define FD_SPEAKER 1
#define MAX_INPUT 100

#define printF(x) write(1, x, strlen(x))

//Sockets
#define ERR_SEND "Error fent el send\n"
#define ERR_BIND "Error fent el bind\n"
#define ERR_LISTEN "Error fent el listen\n"
#define ERR_SOCKET "Error creant el socket\n"
#define ERR_CONFIG_IP "Error configurant IP\n"
#define ERR_CONNECT "Error fent el connect\n"
#define ERR_ACCEPT "Error fent el accept\n"
#define WAITING_CONNECTIONS "Waiting for connections..."
#define NEW_BOWMAN_CONNECTION "New Bowman connection entering\n"
#define NEW_POOLE_CONNECTION "New Poole connection entering\n"

//Tramas
#define ERR_TRAMA "Error en la trama\n"
#define ERR_TRAMA_SIZE "Error en la trama, size incorrecte\n"
#define ERR_TRAMA_HEADER "Error en la trama, header incorrecte\n"
#define ERR_TRAMA_DATA "Error en la trama, data incorrecte\n"
#define ERR_TRAMA_CONTADOR "Error en la trama, contador incorrecte\n"
#define NEW_POOLE "NEW_POOLE"
#define CON_OK "CON_OK"
#define CON_KO "CON_KO"
#define NEW_BOWMAN "NEW_BOWMAN"
#define LIST_SONGS "LIST_SONGS"
#define SONGS_RESPONSE "SONGS_RESPONSE"
#define LIST_PLAYLISTS "LIST_PLAYLISTS"
#define PLAYLISTS_RESPONSE "PLAYLISTS_RESPONSE"
#define DOWNLOAD_SONG "DOWNLOAD_SONG"
#define DOWNLOAD_LIST "DOWNLOAD_LIST"
#define NEW_FILE "NEW_FILE"
#define FILE_DATA "FILE_DATA"
#define CHECK_OK "CHECK_OK"
#define CHECK_KO "CHECK_KO"
#define EXIT "EXIT"
#define CONOK "CONOK"
#define CONKO "CONKO"
#define UNKNOWN "UNKNOWN"
#define OK "OK\n"
#define KO "KO\n"

#endif
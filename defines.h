#ifndef _defines_h_
#define _defines_h_

#define READING_FILE "Reading configuration file"
#define WAITING_CONNECTIONS "Waiting for connections..."
#define ERR_INVALID_INPUT "ERROR: Input invalid\n"
#define SIGINT1 "SIGINT received\n"
#define SONGS "songs"
#define PLAYLIST "playlist"
#define ERR_FLAG "ERROR: Flag invalid\n"

#define FD_READER       0
#define FD_SPEAKER      1
#define MAX_INPUT       200
#define MAXEVENTS       100
#define MAX_CONN        26
#define BACKLOG         200
#define NI_MAXHOST      1025
#define NI_MAXSERV      32
#define NI_THREADS      1

#define printF(x) write(1, x, strlen(x))

//Bowman
#define SERVER_DOWN "El servidor ha cerrado la conexión\n"
#define ERR_READ "Error fent el read\n"   

//Poole
#define CONNECTED   "Connected to HAL 9000 System, ready to listen to Bowmans petitions\n"
#define ERR_COLA    "Error creando la cola\n"
#define ERR_MSG     "Error al recibir el msg\n"

//Sockets
#define ERR_SEND "Error fent el send\n"
#define ERR_BIND "Error fent el bind\n"
#define ERR_LISTEN "Error fent el listen\n"
#define ERR_SOCKET "Error creant el socket\n"
#define ERR_CONFIG_IP "Error configurant IP\n"
#define ERR_CONNECT "Error fent el connect\n"
#define ERR_ACCEPT "Error fent el accept\n"
#define WAITING_CONNECTIONS "Waiting for connections..."
#define NEW_BOWMAN_CONNECTION "\nNew Bowman connection entering\n"
#define NEW_POOLE_CONNECTION "\nNew Poole connection entering\n"
#define ERR_RECIVE "Error fent el recive\n"
#define ERR_COMUNICATION "Error en la comunicació\n"

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
#define DISCONECT_POOLE "DISCONECTP"
#define DISCONECT_BOWMAN "DISCONECTB"
#define OK "OK\n"
#define KO "KO\n"
#define DATA_START_MARKER '$' 

#endif
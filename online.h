#define ONLINE_CONFIG_FILE  "config.ini"

void getOnlineID(int* id);
const char* getIPAdress(Uint16* port);// permet de connaitre les informations de connexion de l'adversaire
//UDPsocket initializeConnection(IPaddress* ip, Uint16* port, int* channel);
void vertexOwnerMessage(char* str, int player, int vertex);
void removePawnMessage(char* str, int vertex);
void sendMessage(char* message, int len); // envoie un packet UDP à l'adversaire
int receiveMessage(char* message, int len, int timeout);

#include "online.c"


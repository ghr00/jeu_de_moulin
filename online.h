/**
Auteur : Amine YAHEMDI (GHR00)
Description : Les fonctions implementant le mode LAN devait �tre declar� dans ce fichier, mais le scripting du mode f�t
integr� directement au main.c, de ce fait, ce fichier est � moiti� inutilisale pour le moment.
*/

#ifndef ONLINE_H
#define ONLINE_H

#define ONLINE_CONFIG_FILE  "config.ini"

void getOnlineID(int* id);
const char* getIPAdress(Uint16* port);// permet de connaitre les informations de connexion de l'adversaire
//UDPsocket initializeConnection(IPaddress* ip, Uint16* port, int* channel);
void vertexOwnerMessage(char* str, int player, int vertex);
void removePawnMessage(char* str, int vertex);
void sendMessage(char* message, int len); // envoie un packet UDP � l'adversaire
int receiveMessage(char* message, int len, int timeout);

#include "source/online.c"

#endif /* ONLINE_H */

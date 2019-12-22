void getIPAdress(IPaddress* ip, Uint16* port)
{
    const char* _host;
    const char* _port;

    ini_t* file = NULL;

    file = ini_load(ONLINE_CONFIG_FILE);

    printf("file %p", file);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    _port = ini_get(file, "Opponent", "Port");
    _host = ini_get(file, "Opponent", "IP");

    //*port = (Uint16)_port; // à corriger !

    int nbrPort = atoi(_port);

    printf("Poort%d\n", nbrPort);
    *port = (Uint16)nbrPort;

    SDLNet_ResolveHost(ip, _host, *port);

    ini_free(file);
}

UDPsocket initializeConnection(IPaddress* ip, Uint16* port, int* channel)
{
    UDPsocket udpsock;

    getIPAdress(ip, port);

    udpsock = SDLNet_UDP_Open(*port);

    if(!udpsock)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    }

    *channel = SDLNet_UDP_Bind(udpsock, -1, ip);

    if(*channel==-1)
    {
        printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
        // do something because we failed to bind
    }

    return udpsock;
}

// Cette fonction sert à créer un message qui indique au joueur adverse qu'un sommet a changé de propriétaire.
// Le message devrait par la suite être envoyé avec sendMessage()
void vertexOwnerMessage(char* str, int player, int vertex)
{
    //le premier caractere indique le type de message, le '-' est séparateur entre l'id du nouveau propriétaire et l'id du sommet.
    sprintf(str, "o%d-%d", player, vertex);
}

// Cette fonction sert à créer un message qui indique au joueur adverse qu'un pion doit être supprimé.
// Le message devrait par la suite être envoyé avec sendMessage()
void removePawnMessage(char* str, int vertex)
{
    //le premier caractere indique le type de message.
    sprintf(str, "r%d", vertex);
}

// Cette fonction est utilisée pour envoyer des paquets UDP au joueur adverse, chaque paquet contient un message generé par
// une fonction de type xMessage(char* str, ...) . A la reception, le message est decodé et l'état du jeu adverse est mis à jour.
void sendMessage(char* message, int len)
{
    IPaddress ip;
    Uint16 port;
    int channel;

    UDPsocket udpsock = initializeConnection(&ip, &port, &channel);

    UDPpacket* packet = SDLNet_AllocPacket(len);

    packet->address = ip;
    packet->channel = channel;
    packet->data    = (Uint8*)message;

    if(!SDLNet_UDP_Send(udpsock, packet->channel, packet))
        printf("[Online] Erreur dans l'envoie.");

    SDLNet_FreePacket(packet);

    SDLNet_UDP_Close(udpsock);
}

// message : une chaine vide contenant le message à recevoir.
// len : la taille maximal des messages pouvant être reçu.
int receiveMessage(char* message, int len, int timeout)
{
    IPaddress ip;
    Uint16 port;
    int channel;

    UDPsocket udpsock = initializeConnection(&ip, &port, &channel);

    printf("Port:%d", port);

    UDPpacket* packet = SDLNet_AllocPacket(len);

    int numrcv;

    int T = 0; // permet d'estimer le temps correspand à un timeout

    do
    {
        numrcv=SDLNet_UDP_Recv(udpsock, packet);
        if(numrcv == -1)
        {
            printf("SDLNet_GetError: %s\n", SDLNet_GetError());
            return -1;
        }

        printf("\tAucun message reçu..\n");
        T++;
    }
    while(numrcv == 0 || T < timeout); // tant qu'aucun paquet n'est reçu...

    if(T >= timeout)
    {
        printf("\t[Timeout] Delai d'attente surpasse pour la reception d'un message..");
        return 0;
    }

    printf("\t[Receive] Reception d'un message!");

    sprintf(message, "%s", (char*)packet->data);

    SDLNet_FreePacket(packet);

    SDLNet_UDP_Close(udpsock);
    return 1;
}

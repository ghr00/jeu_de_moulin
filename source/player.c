#ifndef STR_IMPL_
    #define STR_IMPL_(x) #x      //stringify argument
#endif // STR_IMPL_

#ifdef PLAYER_ID
    undef PLAYER_ID
#endif // PLAYER_ID

#ifdef GET_PSEUDO
    undef GET_PSEUDO
#endif // GET_PSEUDO

// Cette macro (GET_PSEUDO) sert à charger les pseudos des joueurs dans la fonction getPlayersUsernames
#define PLAYER_ID(x) STR_IMPL_(x)  //indirection to expand argument macros

#define GET_PSEUDO(id)  _pseudo = "Erreur"PLAYER_ID(id)""; \
                        _pseudo = ini_get(file, "Game", "Player" PLAYER_ID(id) "");  \
                        if(_pseudo != NULL) \
                            strcpy(pseudo##id, _pseudo);

void getPlayersUsernames(char pseudo1[MAX_USERNAME_LENGTH], char pseudo2[MAX_USERNAME_LENGTH])
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _pseudo;

    GET_PSEUDO(1);
    GET_PSEUDO(2);

    ini_free(file);
}

void initializePlayer(Player* player, const char pseudo[MAX_USERNAME_LENGTH], SDL_Color color)
{
    strcpy(player->pseudo, pseudo);

    player->color = color;

    player->pawns = PLAYER_INITIAL_PAWNS;

    player->activePawns = 0;

    player->moulin = 0;

    for(int k = 0; k < 4; k++) player->moulinID[k] = -1;
}

int setPlayerAI(Player* player, int type)
{
    if(player->ai[type] != NULL)
    {
        player->activeAI = player->ai[type];
        return 1;
    }

    return 0;
}

int isPlayerAI(Player player)
{
    if(player.activeAI != NULL) return 1;

    return 0;
}

void destroyPlayer(Player* player)
{
    if(player == NULL) return;

    for(int i = 0; i < MAX_PLAYER_AI; i++)
        if(player->ai[i] != NULL) free(player->ai[i]);
}


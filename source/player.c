
void initializePlayer(Player* player, const char pseudo[MAX_USERNAME_LENGTH], SDL_Color color)
{
    strcpy(player->pseudo, pseudo);

    player->color = color;

    player->pawns = PLAYER_INITIAL_PAWNS;

    player->activePawns = 0;

    player->moulin = 0;

    for(int k = 0; k < 4; k++) player->moulinID[k] = -1;
}

void getPlayersUsernames(char pseudo1[MAX_USERNAME_LENGTH], char pseudo2[MAX_USERNAME_LENGTH])
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _pseudo;

    _pseudo = "Erreur1"; // par défaut le pseudo 'Erreur' indique que le chargement a été mal éxecuté.
    _pseudo = ini_get(file, "Game", "Player1");
     printf("pseudo:%s",_pseudo);
    if(_pseudo != NULL && _pseudo[0] != '/0') strcpy(pseudo1, _pseudo);



    _pseudo = "Erreur2"; // par défaut le pseudo 'Erreur' indique que le chargement a été mal éxecuté.
    _pseudo = ini_get(file, "Game", "Player2");
    //strcpy(pseudo2, _pseudo);

    ini_free(file);
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


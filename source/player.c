
void initializePlayer(Player* player, char* pseudo, SDL_Color color)
{
    player->pseudo = pseudo;

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


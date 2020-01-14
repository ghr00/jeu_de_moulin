int initializeGame(Game* game, enum gameType type, int optionnal_rule)
{
    if(game == NULL) return 0;

    game->active = 0;
    game->turn = 0;
    game->type = type;
    game->hidingTurn = 0;
    game->optionnel = optionnal_rule;
    game->frame = 0;
    game->cap = 1;
    return 1;
}

int convertTurn(int turn)
{
    if(turn % 2 == 0) return 0;

    return 1;
}

int getRendererFlags()
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    printf("file2 %p\n", file);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _id = ini_get(file, "Graphics", "Debug");

    int flags = atoi(_id);

    printf("DEBUG%d\n", flags);

    ini_free(file);

    if(flags == 1)
        return SDL_RENDERER_SOFTWARE;
    else
        return SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

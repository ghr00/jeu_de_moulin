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

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _id = ini_get(file, "Graphics", "Debug");

    int flags = atoi(_id);

    printf("[Graphics] Mode:%d\n", flags);

    ini_free(file);

    if(flags == 1)
        return SDL_RENDERER_SOFTWARE;
    else
        return SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

const char* getFont()
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _font = ini_get(file, "Graphics", "Font");

    printf("[Graphics] Font : %s\n", _font);

    if(_font == NULL)
    {
        printf("Erreur dans le chargement du font depuis config.ini");
        return NULL;
    }

    return _font;
}

int getFontSize()
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _size = ini_get(file, "Graphics", "FontSize");

    if(_size == NULL)
    {
        printf("Erreur dans le chargement de la taille du font depuis config.ini");
        return DEFAULT_FONT_SIZE;
    }

    int size = atoi(_size);

    printf("[Graphics] FontSize:%d\n", size);

    ini_free(file);

    return size;
}

const char* getMusicFile()
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _music = ini_get(file, "Audio", "Music");

    printf("[Audio] Musique : %s\n", _music);

    if(_music == NULL)
    {
        printf("Erreur dans le chargement du font depuis config.ini");
        return "audio/music.mp3";
    }

    return _music;
}

int getMusicVolume()
{
    ini_t* file = NULL;

    file = ini_load(GAME_CONFIG_FILE);

    if(file == NULL)
        printf("Echec du chargement du fichier ONLINE_CONFIG_FILE\n");

    const char* _volume = ini_get(file, "Audio", "Volume");

    if(_volume == NULL)
    {
        printf("Erreur dans le chargement de la taille du font depuis config.ini");
        return MIX_MAX_VOLUME;
    }

    int volume = atoi(_volume);

    printf("[Audio] Volume de la musique:%d\n", volume);

    ini_free(file);

    return volume;
}

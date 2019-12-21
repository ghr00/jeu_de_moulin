int initializeGame(Game* game, enum gameType type, int optionnal_rule)
{
    if(game != NULL) return 0;

    game->active = 0;
    game->turn = 0;
    game->type = type;
    game->hidingTurn = 0;
    game->optionnel = optionnal_rule;

    return 1;
}

int convertTurn(int turn)
{
    if(turn % 2 == 0) return 0;

    return 1;
}

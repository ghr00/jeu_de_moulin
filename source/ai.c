AI* createAI(int type)
{
	AI* ai = (AI*)malloc(sizeof(AI));

	if(ai == NULL) return NULL;

	ai->type = type;
	ai->freeze = 0;

	ai->firstPlacement = 0;
	return ai;
}

int createAIForPlayer(int type, Player* player)
{
	if(player == NULL) return -1;

	player->ai[type] = createAI(type);

	if(player->ai[type] != NULL) return 1;

	return 0;
}

int setAIStateForPlayer(Player* player, int state)
{
	if(player->activeAI != NULL)
	{
		player->activeAI->state = state;
		return 1;
	}

	return 0;
}

int getAIStateForPlayer(Player* player)
{
    if(player->activeAI == NULL) return -1;

    return player->activeAI->state;
}

int getAITypeForPlayer(Player* player)
{
    if(player->activeAI == NULL) return -1;

    return player->activeAI->type;
}

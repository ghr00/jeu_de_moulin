const int A_EVAL = 10;
const int B_EVAL = 100;

enum aiType // flags
{
	AI_TYPE_NONE,
	AI_TYPE_EASY_RANDOM,
	AI_TYPE_MEDIUM,
	AI_TYPE_DIFFICULT // minimax
};

enum aiState
{
    AI_STATE_INACTIVE,
    AI_STATE_ACTIVE
};

AI* createAI(int type);
int createAIForPlayer(int type, Player* player);
int setAIStateForPlayer(Player* player, int state);
int getAIStateForPlayer(Player* player);
int getAITypeForPlayer(Player* player);
int destroyAI(AI* ai);
#include "source/ai.c"

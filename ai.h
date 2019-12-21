const int A_EVAL = 10;
const int B_EVAL = 100;

enum aiType // flags
{
	AI_TYPE_NONE,
	AI_TYPE_EASY_RANDOM,
	AI_TYPE_MEDIUM
};

enum aiState
{
    AI_STATE_INACTIVE,
    AI_STATE_ACTIVE
};

AI* createAI(int type);
int createAIForPlayer(int type, Player* person);
int setAIStateForPlayer(Player* person, int state);
int getAIStateForPlayer(Player* person);
int getAITypeForPlayer(Player* person);
int destroyAI(AI* ai);
#include "ai.c"

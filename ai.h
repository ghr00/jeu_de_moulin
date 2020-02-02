/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier définit tous ce qui concenre l'IA, il est indépendant du jeu et peut être utilisé dans n'importe
                quel projet utilisant la SDL2 .*/

#ifndef AI_H
#define AI_H

// Constantes pour l'algorithme alpha/beta, inutilisables pour le moment.
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

void ai_positionment(int ai, int placed[MAX_PLAYERS]);
void ai_movement(int ai, int placed[MAX_PLAYERS]);
void ai_saut(int ai, int placed[MAX_PLAYERS]);
void ai_moulin(int ai, int moulinID);

int searchOpportunity(int ai, int* u, int* v);

#endif

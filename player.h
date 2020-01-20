/**
Auteur : Amine YAHEMDI (GHR00)
Description : Tout ce qui concerne le joueur est declaré ici.
*/

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_INITIAL_PAWNS 9

#define MAX_PLAYERS  2
#define MAX_PLAYER_AI 3 // Nombre de AI par joueur  (facile, moyen, difficile)

typedef struct ai
{
	int state;
	int type;
    int freeze;

    int firstPlacement;
}AI;

typedef struct player
{
    char* pseudo;

    SDL_Color color;

    AI* ai[MAX_PLAYER_AI];

    AI* activeAI;

    int activePawns; // nombre de pion actives sur le plateau du jeu
    int pawns; // nombre de pion restant
    int moulin; // si 1, alors le joueur pourrait supprimer n'importe quel pion de l'adversaire
    int moulinID[4]; // l'id de la ligne moulin qui a donné au joueur la possibilité de supprimer un moulin
}Player;

void initializePlayer(Player* player, char* pseudo, SDL_Color color);
int setPlayerAI(Player* player, int type);
int isPlayerAI(Player player);
void destroyPlayer(Player* player);

#include "source/player.c"

#endif

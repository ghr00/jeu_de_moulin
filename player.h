/**
Auteur : Amine YAHEMDI (GHR00)
Description : Tout ce qui concerne le joueur est declaré ici.
*/

#ifndef PLAYER_H
#define PLAYER_H

#ifndef GAME_CONFIG_FILE
    #define GAME_CONFIG_FILE  "config.ini"
#endif // GAME_CONFIG_FILE

#define PLAYER_INITIAL_PAWNS 9

#define MAX_PLAYERS  2
#define MAX_PLAYER_AI 3 // Nombre de AI par joueur  (facile, moyen, difficile)

#define MAX_USERNAME_LENGTH 24

typedef struct ai
{
	int state;
	int type;
    int freeze;

    int firstPlacement;
}AI;

typedef struct player
{
    char pseudo[MAX_USERNAME_LENGTH];

    SDL_Color color;

    AI* ai[MAX_PLAYER_AI];

    AI* activeAI;

    int activePawns; // nombre de pion actives sur le plateau du jeu
    int pawns; // nombre de pion restant
    int moulin; // si 1, alors le joueur pourrait supprimer n'importe quel pion de l'adversaire
    int moulinID[4]; // l'id de la ligne moulin qui a donné au joueur la possibilité de supprimer un moulin
}Player;

void initializePlayer(Player* player, const char pseudo[MAX_USERNAME_LENGTH], SDL_Color color);
void getPlayersUsernames(char pseudo1[MAX_USERNAME_LENGTH], char pseudo2[MAX_USERNAME_LENGTH]); // charge les pseudos des joueurs depuis le fichier de configuration
void setPlayerUsername(Player* player, const char* username);
int setPlayerAI(Player* player, int type);
int isPlayerAI(Player player);
void destroyPlayer(Player* player);

#include "source/player.c"

#endif

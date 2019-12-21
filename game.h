#define MAX_GAME_TYPES 3
#define MAX_SQUARES 3

const int SCREEN_FPS        =   20;
const int SCREEN_WIDTH      =   860;
const int SCREEN_HEIGHT     =   620;
//const int SCREEN_TICKS_PER_FRAME    = 1000 / SCREEN_FPS;

enum gameType
{
    GAME_TYPE_PvP,
    GAME_TYPE_PvAI,
    GAME_TYPE_PvP_ONLINE
};

char gameType[MAX_GAME_TYPES][32] = { {"Joueur contre Joueur"}, {"Joueur contre Ordinateur"}, {"Joueur contre Joueur (en ligne)"} };

typedef struct game
{
    Player players[MAX_PLAYERS]; // Ce tableau stock les variables des joueurs

    int active; // Si 1, la partie est en cours, sinon, la partie est terminé

    int turn;   // comme il y aura 2 joueurs, si le tour est pair : c'est au tour du de game.players[0]
                // sinon, c'est au tour de game.players[1] . Une fonction qui permet de savoir si
                // c'est au tour du premier joueur ou du second est convertTurn(int turn).

    int hidingTurn; // Le nombre de tour affiché aux joueurs à l'écran : turn - hidingTurn
                    // hidingTurn est incrementé lors des phases de moulin, en effet, cette phase est techniquement
                    // considéré comme se deroulant en deux tours.

    enum gameType type;   // prend une valeur de enum gameType.

    enum aiType difficulty; // difficulté de l'ia

    int optionnel;  // Si 1, la régle optionnelle est pris en compte.
                    // Ainsi, si il reste pour un joueur moins de trois pions, il peut deplacer ses pions vers n'importe quel
                    // case non occupée.

    int frame;

    int cap;

    int theme;
}Game;

int initializeGame(Game* game, enum gameType type, int optionnal_rule);
int convertTurn(int turn); // convertit le tour en identifiant des joueurs

#include "game.c"

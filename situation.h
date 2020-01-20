/**
Auteur : Amine YAHEMDI (GHR00)
Description : situation.h et situation.c devait implementer l'algorithme minimax mais l'ia ne fonctionne pas comme prévu,
tout ce qui est declaré dans ce fichier est inutilisable pour le moment.
*/

#ifndef SITUATION_H
#define SITUATION_H

typedef struct situation
{
    Game game;

    Vertex* vertices[MAX_VERTICES];

    int value; // calculé avec eval()

    int moulin; // id du pion choisi à supprimer

    struct situation* parent;

    struct situation* previous;
}Situation;

// * Situation* situationsPile: queue de la pile des situations produites à partir de la situation courante.
Situation* situationsPile = NULL;

const int initialDepth = 3;

Situation* generateSituation(Situation* parent, int moulin, int cible);
Situation* generateInitialeSituation(Game* game, Vertex* vertices[], int moulin, int cible);
Situation* searchParent(Situation* initiale, int id);
void countMoulins(Situation* situation, int* moulin1, int id1, int* moulin2, int id2);
int eval(Situation* situation, int id);
void nextSituations(Situation* situation, int depth, int moulin, int cible);
Situation* empilerSituation(Situation* newElement);
Situation* depilerSituation();
Situation* minimax(Situation* situation, int depth, int id, int moulin, int cible);
int isPlayerBlocked(Game* game, Vertex* vertices[], int id);
int isGameFinished(Situation* situation);
int freeSituationPile();
int countSituations();
void getActivePawns(Situation* situation, int* p1, int id1, int* p2, int id2);


#include "source/situation.c"

#endif // SITUATION_H

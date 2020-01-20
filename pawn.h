/**
Auteur      :   Amine Yahemdi.
Description :   Voir 'vertex.h' .
*/

#ifndef PAWN_H
#define PAWN_H

void movePawn(Player* player, Vertex* vertex, Vertex* destination);
int isPawnMovable(Vertex* vertex, Vertex* vertices[MAX_VERTICES]);
SDL_Rect getPawnRectangle(Vertex* vertex);
void deletePawn(Vertex* vertex);

#include "source/pawn.c"

#endif

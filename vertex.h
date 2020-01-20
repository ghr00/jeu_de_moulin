/**
Auteur      :   Amine Yahemdi.
Description :   Vertex signifie ici 'sommet', ce sont les 24 points de forme carr�s affich�s sur le plateau du jeu et
                sur lequelles il est possible de poser un pion.
                Il est possible de d�finir un pion comme "un sommet control� par un joueur" et un sommet comme
                "un pion invisible sur le plateau du jeu", de ce fait j'ai consid�r� que les sommets et les pions sont la m�me
                chose et j'ai declar� tous ce qui les concernent dans 'vertex.h' et 'pawn.h'. Quand une fonction, un parametre ou une
                variable sont relatifs � un sommet ils portent le nom de 'vertex', quand ils sont relatifs � un pion ils portent
                le nom de 'pawn'.
*/

#ifndef VERTEX_H
#define VERTEX_H

#define MAX_VERTICES 24

/* Line signifie trois sommets adjacents � travrers lequelles une ligne verticale ou horizentale passe, il existe 16 lignes
de ce type. Si une ligne est active alors il y a un moulin. */
#define MAX_LINES 16

enum lineState
{
    LINE_INACTIVE, // la ligne est inactive
    LINE_ACTIVE, // la ligne est active, mais le bonus moulin n'est pas encoe utilis�
    LINE_USED, // l'effet moulin est utilis�
};

#define VERTEX_FILE "sprites/vertex.bmp"
#define PAWN_FILE "sprites/pawn.bmp"

typedef struct vertex
{
	Sprite sprite;

	Sprite pawn;

    SDL_Rect rect;

	Player* owner;

    //int moulin; // si �gale 1, alors le pion est en formation moulin

    //int moulinCount; // nombre de moulin dont fait partie le pion

    int list[4]; // Ce tableau contient les identifiants des sommets adjacents � ce vertex
    int id; // identifiant re�u � l'initlisation d'un vertice, le m�me utilsi� dans le tableau vertices[] de main.c
	//List* list;
}Vertex;

Vertex* initializeVertex(SDL_Renderer* renderer, char* fileImage, char* pawnImage);
void setVertexPosition(Vertex* vertex, int x, int y);
void setVertexPosition2(Vertex* vertex, SDL_Point point);
void moveVertexBy(Vertex* vertex, int x, int y);
void changeVertexColor(Vertex* vertex, SDL_Color color);
void setPawnVisibilityState(Vertex* vertex, int visible);
SDL_Rect getVertexRectangle(Vertex* vertex);
void setVertexOwner(Vertex* vertex, Player* player);
void setVertexList(Vertex* vertex, int matrix[24][24]);
int isVertexAdjacentToLine(Vertex* vertex, int line, int* adj, Vertex* vertices[], int Lines[][4], int Adjacency[][MAX_VERTICES]);

#include "source/vertex.c"

#endif // VERTEX_H

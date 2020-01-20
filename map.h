/**
Auteur : Amine YAHEMDI (GHR00)
Description : Tout ce qui concerne le plateau du jeu est declaré ici.
*/

#ifndef MAP_H
#define MAP_H

/** Matrice d'adjacence du plateau du jeu
    Propriétés  :   1. Elle est symetrique.
                    2. Si Adjacency[i][j] = 1, alors il existe un chemin entre le sommet i et sommet j.
                    3. Si Adjacency[i][j] = 0, alors il n'existe aucun chemin entre le sommet i et sommet j. */
int Adjacency[24][24] = {
    {0, 1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {1, 0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {1, 0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  1},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1},
    {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  1,  0}
};
// Ce tableau à deux dimensions stocke toutes les possibilités de la formation des moulins.
// Il y a 16 possibilités. Ce tableau sera utilisé pour verifier si trois pions forment un moulin.
// Il suffit pour cela de vérifier si trois pions adjacents appartiennent à element du tableau.
// Le quatriéme élément permet de verifier si le moulin est déjà formé ou non;
int Lines[MAX_LINES][4] =
{
    { 0, 1, 2, 0 },
    { 2, 3, 4, 0 },
    { 4, 5, 6, 0 },
    { 6, 7, 0, 0 },

    { 8,  9, 10, 0 },
    { 10, 11, 12, 0 },
    { 12, 13, 14, 0 },
    { 14, 15, 8, 0 },

    { 16, 17, 18, 0 },
    { 18, 19, 20, 0 },
    { 20, 21, 22, 0 },
    { 22, 23, 16, 0 },

    { 1, 9, 17 , 0 },
    { 3, 11, 19, 0 },
    { 5, 13, 21, 0 },
    { 7, 15, 23, 0 }

};

/* La dessin du plateau du jeu est composé de trois carrés, chacun contenu dans l'autre. Ces carrés sont modelisés dans
le tableau squares[].*/
SDL_Rect squares[MAX_SQUARES];

/* Les sommets qui peuvent contenir des pions sont modelisés dans le tableau de vertices[], ils sont de nombre de 24.*/
Vertex* vertices[MAX_VERTICES];

#include "source/map.c"

#endif

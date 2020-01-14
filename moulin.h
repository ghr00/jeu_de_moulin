/** Auteur : Amine YAHEMDI (GHR00)
    Description : Tout ce qui concerne les moulins est implementé dans ce fichier.

    searchMoulin :  vérifie pour chaque pion existant si il appartient à un moulin, si c'est le cas, un moulin est créé.
                    les ids des moulins seront par la suite stockés dans le tableau moulinID[].

    isVertexInMoulin : vérifie si un pion fait partie d'uu moins un moulin, retourne 0 si ce n'est pas le cas.

    deleteMoulins : supprime l'état "dans un moulin" pour un sommet, utilisé principalement lorsqu'un pion bouge d'un moulin
                    vers une ligne qui ne contient pas de moulin.
*/
void searchMoulin(Player* player, Vertex* vertices[MAX_VERTICES], int Lines[][4], int moulinID[]);
int isVertexInMoulin(Vertex* vertex, int Lines[][4]);
void deleteMoulins(Vertex* vertex, int Lines[][4]);

#include "source/moulin.c"

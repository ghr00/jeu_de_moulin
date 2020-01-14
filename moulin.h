/** Auteur : Amine YAHEMDI (GHR00)
    Description : Tout ce qui concerne les moulins est implement� dans ce fichier.

    searchMoulin :  v�rifie pour chaque pion existant si il appartient � un moulin, si c'est le cas, un moulin est cr��.
                    les ids des moulins seront par la suite stock�s dans le tableau moulinID[].

    isVertexInMoulin : v�rifie si un pion fait partie d'uu moins un moulin, retourne 0 si ce n'est pas le cas.

    deleteMoulins : supprime l'�tat "dans un moulin" pour un sommet, utilis� principalement lorsqu'un pion bouge d'un moulin
                    vers une ligne qui ne contient pas de moulin.
*/
void searchMoulin(Player* player, Vertex* vertices[MAX_VERTICES], int Lines[][4], int moulinID[]);
int isVertexInMoulin(Vertex* vertex, int Lines[][4]);
void deleteMoulins(Vertex* vertex, int Lines[][4]);

#include "source/moulin.c"

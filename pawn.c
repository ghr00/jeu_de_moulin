
void setPawnVisibilityState(Vertex* vertex, int visible)
{
    setVisibilityState(&(vertex->pawn), visible);
}

void movePawn(Player* player, Vertex* vertex, Vertex* destination)
{
    if(player == NULL)
    {
        printf("[movePawn] la valeur de player est nulle\n");
    }

    setVertexOwner(destination, player );
    setVertexOwner(vertex, NULL);

    setPawnVisibilityState(destination, 1);
    setPawnVisibilityState(vertex, 0);

    SDL_SetTextureAlphaMod(vertex->pawn.texture, 255);
}

///
// Retourne le degrée de liberté d'un pion, si il est égale à 0, il n'est pas movable
///
int isPawnMovable(Vertex* vertex, Vertex* vertices[MAX_VERTICES])
{
    int cmp = 0;

    int i;
    for(int u = 0; u < 4; u++) // le degré de liberté maximal est de 4
    {
        if(vertex->list[u] != -1)
        {
            i = vertex->list[u];

            if(vertices[i]->owner == NULL) cmp++;
        }
    }

    return cmp;
}

void deletePawn(Vertex* vertex)
{
    setPawnVisibilityState(vertex, 0);

    vertex->owner = NULL;

    vertex = NULL;
}

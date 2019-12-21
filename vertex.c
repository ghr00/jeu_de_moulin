
Vertex* initializeVertex(SDL_Renderer* renderer, char* fileImage, char* pawnImage)
{

    Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));

    vertex->owner = NULL;

    vertex->sprite.renderer     = renderer;
    vertex->sprite.imageFile    = fileImage;

    vertex->pawn.renderer   = renderer;
    vertex->pawn.imageFile  = pawnImage;

    /*vertex->moulin          = 0;
    vertex->moulinCount    = 0;*/

    loadSprite( &(vertex->sprite)   );
    loadSprite( &(vertex->pawn)     );

    setPawnVisibilityState(vertex, 0);
    //road->state = ROAD_STATE_ACTIVE;

    for(int u = 0; u < 4; u++)
    {
        vertex->list[u] = -1;

    }

    return vertex;
}

void getVertexPosition(Vertex* vertex, int* x, int* y)
{
    *x = (int)(vertex->sprite.rect.x);
    *y = (int)(vertex->sprite.rect.y);
}

void setVertexPosition(Vertex* vertex, int x, int y)
{
    setSpritePosition(&(vertex->sprite), x, y);
    setSpritePosition(&(vertex->pawn), x, y);
}

void setVertexPosition2(Vertex* vertex, SDL_Point point)
{
    setSpritePosition(&(vertex->sprite), point.x, point.y);
    setSpritePosition(&(vertex->pawn), point.x, point.y);
}

void moveVertexBy(Vertex* vertex, int x, int y)
{
    moveSpriteBy(&(vertex->sprite), x, y);
    moveSpriteBy(&(vertex->pawn), x, y);
}
void changeVertexColor(Vertex* vertex, SDL_Color color)
{
    changeColorSprite(&(vertex->pawn), color);
}

void setVertexOwner(Vertex* vertex, Player* player)
{
    vertex->owner = player;


    if(player != NULL) changeVertexColor(vertex, player->color);

    else
    {
        SDL_Color black = {0, 0, 0, 255};

        changeVertexColor(vertex, black);
    }
}

SDL_Rect getVertexRectangle(Vertex* vertex)
{
    /// PAWN ----> VERTEX_SPRITE

    return vertex->sprite.rect;
}

SDL_Rect getPawnRectangle(Vertex* vertex)
{
    /// PAWN ----> VERTEX_SPRITE

    return vertex->pawn.rect;
}

void drawVertex(Vertex* vertex)
{
    //updateVertex(vertex);

    drawSprite( &(vertex->sprite) );
    drawSprite( &(vertex->pawn) );
}

void setVertexList(Vertex* vertex, int matrix[24][24])
{
    int id = vertex->id;

    for(int i = 0; i < MAX_VERTICES; i++)
    {
        if(matrix[id][i] == 1)
        {
            for(int u = 0; u < 4; u++) // le degré de liberté maximal est de 4
            {
                if(vertex->list[u] == -1)
                {
                    vertex->list[u] = i;

                    printf("[%d] [%d]\n", id, i);
                    break;
                }
            }
        }
    }
}

int isVertexAdjacentToLine(Vertex* vertex, int line, int* adj, Vertex* vertices[], int Lines[][4], int Adjacency[][MAX_VERTICES])
{
    int a;
    for(int p = 0; p < 3; p++)
    {
        a = Lines[line][p];

        if(a != vertex->id && Adjacency[a][vertex->id] == 1)
        {
            *adj = a;
            return 1;
        }
    }

    return 0;
}

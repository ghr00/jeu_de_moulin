
void drawMap(SDL_Renderer* renderer)
{
    for(int i = 0; i < MAX_SQUARES; i++)
        SDL_RenderDrawRect(renderer, & (squares[i]) );

    SDL_RenderDrawLine(renderer, vertices[1]->sprite.rect.x, vertices[1]->sprite.rect.y, vertices[17]->sprite.rect.x, vertices[17]->sprite.rect.y);
    SDL_RenderDrawLine(renderer, vertices[3]->sprite.rect.x, vertices[3]->sprite.rect.y, vertices[19]->sprite.rect.x, vertices[19]->sprite.rect.y);
    SDL_RenderDrawLine(renderer, vertices[5]->sprite.rect.x, vertices[5]->sprite.rect.y, vertices[21]->sprite.rect.x, vertices[21]->sprite.rect.y);
    SDL_RenderDrawLine(renderer, vertices[7]->sprite.rect.x, vertices[7]->sprite.rect.y, vertices[23]->sprite.rect.x, vertices[23]->sprite.rect.y);

    for(int i = 0; i < MAX_VERTICES; i++)
        drawVertex(vertices[i]);
}

void initializeSquares(SDL_Renderer* renderer)
{
    squares[0].h = 400;
    squares[0].w = 400;
    squares[0].x = 150 + 20;
    squares[0].y = 150 - 75;

    squares[1].h = 300;
    squares[1].w = 300;
    squares[1].x = 200 + 20;
    squares[1].y = 200 - 75;

    squares[2].h = 200;
    squares[2].w = 200;
    squares[2].x = 250 + 20;
    squares[2].y = 250 - 75;

    int x, y;

    int a, b, h, w;

    for(int i = 0; i < 3; i++)
    {
        x = squares[i].x;
        y = squares[i].y;
        h = squares[i].h;
        w = squares[i].w;

        a = x;
        b = y;
        vertices[0 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[0 + 8*i], a - 7, b - 7);
        vertices[0 + 8*i]->id = 0 + 8*i;

        a = x + w/2;
        b = y;
        vertices[1 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[1 + 8*i], a - 7, b - 7);
        vertices[1 + 8*i]->id = 1 + 8*i;

        a = x + w;
        b = y;
        vertices[2 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[2 + 8*i], a - 7, b - 7);
        vertices[2 + 8*i]->id = 2 + 8*i;

        a = x + w;
        b = y + h/2;
        vertices[3 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[3 + 8*i], a - 7, b - 7);
        vertices[3 + 8*i]->id = 3 + 8*i;

        a = x + w;
        b = y + h;
        vertices[4 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[4 + 8*i], a - 7, b - 7);
        vertices[4 + 8*i]->id = 4 + 8*i;

        a = x + w/2;
        b = y + h;
        vertices[5 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[5 + 8*i], a - 7, b - 7);
        vertices[5 + 8*i]->id = 5 + 8*i;

        a = x;
        b = y + h;
        vertices[6 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[6 + 8*i], a - 7, b - 7);
        vertices[6 + 8*i]->id = 6 + 8*i;

        a = x;
        b = y + h/2;
        vertices[7 + 8*i] = initializeVertex(renderer, VERTEX_FILE, PAWN_FILE);
        setVertexPosition(vertices[7 + 8*i], a - 7, b - 7);
        vertices[7 + 8*i]->id = 7 + 8*i;
    }
}

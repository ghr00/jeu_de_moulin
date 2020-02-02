
void searchMoulin(Player* player, Vertex* vertices[MAX_VERTICES], int Lines[][4], int moulinID[])
{
    int count, k;
    for(int i = 0; i < MAX_LINES; i++)
    {
        if(Lines[i][3] != LINE_INACTIVE) continue;

        count = 0;

        for(int p = 0; p < 3; p++)
        {
            k = Lines[i][p];

            //printf("[K] %d (p%d)\n", k, p);
            if(vertices[k]->owner == player)
            {
                count++;
            }
        }

        if(count > 2)
        {
            Lines[i][3] = LINE_ACTIVE;

            for(int k = 0; k < 4; k++)
            {
                if(moulinID[k] == -1)
                {
                    moulinID[k] = i;

                    printf("(( moulinID[k] = %d ))", i);
                    break;
                }

            }
        }
    }
}

int isVertexInMoulin(Vertex* vertex, int Lines[][4])
{
    int u = -1;

    for(int i = 0; i < MAX_LINES; i++)
    {
        if(Lines[i][3] == LINE_INACTIVE) continue;

        for(int p = 0; p < 3; p++)
        {
            if(vertex->id == Lines[i][p])
            {
                u = i;
            }
        }

        if(u != -1)
            return u;
    }

    return u;
}

int moulinCanBeFormed(int id, int line, Vertex* vertices[], int Lines[][4])
{
    int u, cmp;
    cmp = 0;

    for(int p = 0; p < 3; p++)
    {
        u = Lines[line][p];

        if(vertices[u]->owner == &(game.players[id]))
            cmp++;

        else if(vertices[u]->owner == &(game.players[1-id]))
            cmp--;
    }

    if(cmp == 2)
        return 1;

    return 0;
}

void deleteMoulins(Vertex* vertex, int Lines[][4])
{
    for(int i = 0; i < MAX_LINES; i++)
    {
        if(Lines[i][3] == LINE_INACTIVE) continue;

        for(int p = 0; p < 3; p++)
        {
            if(vertex->id == Lines[i][p])
            {
                Lines[i][3] = LINE_INACTIVE;

                printf("\t[Moulin] %d supprime\n",i);
            }
        }
    }

}

AI* createAI(int type)
{
	AI* ai = (AI*)malloc(sizeof(AI));

	if(ai == NULL) return NULL;

	ai->type = type;
	ai->freeze = 0;

	ai->firstPlacement = 0;
	return ai;
}

int createAIForPlayer(int type, Player* player)
{
	if(player == NULL) return -1;

	player->ai[type] = createAI(type);

	if(player->ai[type] != NULL) return 1;

	return 0;
}

int setAIStateForPlayer(Player* player, int state)
{
	if(player->activeAI != NULL)
	{
		player->activeAI->state = state;
		return 1;
	}

	return 0;
}

int getAIStateForPlayer(Player* player)
{
    if(player->activeAI == NULL) return -1;

    return player->activeAI->state;
}

int getAITypeForPlayer(Player* player)
{
    if(player->activeAI == NULL) return -1;

    return player->activeAI->type;
}

void ai_positionment(int ai, int placed[MAX_PLAYERS])
{
    int id = 1 - ai;

    int u = -1;

    //int cmp = 0;

    if(game.players[ai].activeAI == NULL)
        printf("[IA] IA nulle\n");

    printf("diff %d\n", getAITypeForPlayer( &game.players[ai] ));

    if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_DIFFICULT )
    {
        system("CLS");

        Situation* courante = generateInitialeSituation(&game, vertices, 0, -1);

        printf("courante %p\n", courante);

        printf("\n\n\n\n\n\n\n\n\t______________ MINIMAX _______________\n");

        if(situationsPile != NULL)
        {
            free(situationsPile);

            printf("Liberation de la pile des situations\n");

            situationsPile = NULL;
        }


        Situation* decision = minimax(courante, initialDepth, ai, 0, -1);

        printf("Resultat de minimax: %p de valeur %d\n", decision, eval(decision, ai));

        free(courante);

        for(int i = 0; i < MAX_VERTICES; i++)
        {
            if(decision->vertices[i]->owner == NULL) //&game.players[ai])
            {
                printf("\tDECISION %d\n", i);
            }

            else
            {
                printf("\tDECISION %d, owner %s\n", i, decision->vertices[i]->owner->pseudo);

            }
        }

        for(int i = 0; i < MAX_VERTICES; i++)
        {
            if(vertices[i]->owner != decision->vertices[i]->owner)
            {
                const char* s1 = decision->vertices[i]->owner->pseudo;
                const char* s2 = game.players[ai].pseudo;

                printf("[%d] \tS1 %s - S2 %s\n", i, s1, s2);

                if(!strcmp(s1, s2))
                {
                    printf("[IA-Minimax] Elle a pose un pion au vertice %d\n", i);

                    setVertexOwner(vertices[i], &game.players[ai]);
                    setPawnVisibilityState(vertices[i], 1);

                    placed[ai] = i;

                    game.players[ai].pawns--;

                    game.players[ai].activePawns++;
                }
            }
        }

        free(decision);
    }

    else if( getAITypeForPlayer( &game.players[ai] ) != AI_TYPE_NONE )
    {
        /* Vérification si un moulin peut être former */

        int k, focus = -1, focusID = -1;
        int cmp[2]; /*  *Cas 1: Il est possible de former un moulin
                        *Cas 2: Il est possible de contrer la formation du moulin du joueur adverse
                    */

        cmp[0] = 0;
        cmp[1] = 0;

        if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM )
        {
            for(int i = 0; i < 16; i++)
            {
                if(Lines[i][3] != LINE_INACTIVE) continue;

                cmp[0] = 0;
                cmp[1] = 0;

                int choices[3];

                for(int a = 0; a < 3; a++) choices[a] = 0;

                for(int a = 0; a < 3; a++)
                {
                    k = Lines[i][a];

                    if(vertices[k]->owner == &(game.players[ai]))
                    {
                        choices[a] = 1;

                        cmp[0]++;

                        cmp[1]--;
                    }

                    else if(vertices[k]->owner == &(game.players[id]))
                    {
                        choices[a] = -1;

                        cmp[0]--;

                        cmp[1]++;
                    }
                }

                if(cmp[0] == 2)
                {
                    focus = i;

                    for(int a = 0; a < 3; a++)
                    {
                        if(choices[a] == 0) u = Lines[i][a];
                    }

                    focusID = 0;
                    break;
                }

                else if(cmp[1] == 2)
                {
                    focus = i;

                    for(int a = 0; a < 3; a++)
                    {
                        if(choices[a] == 0) u = Lines[i][a];
                    }

                    focusID = 1;
                    break;
                }
            }
        }

        if(cmp[0] != 2 && cmp[1] != 2)
        {
            /* Recherche aléatoire d'une case vide */
            do
            {
                u = Random(0, MAX_VERTICES-1);
            }
            while( vertices[u]->owner == &(game.players[ai]) || vertices[u]->owner == &(game.players[id]) );
        }

        else printf("[IA] Moulin %d doit remplie ! [cas:%d]\n", focus, focusID);

        printf("[IA] Elle a pose un pion au vertice %d\n", u);

        if(u != -1)
        {
            setVertexOwner(vertices[u], &game.players[ai]);
            setPawnVisibilityState(vertices[u], 1);

            placed[ai] = u;

            game.players[ai].pawns--;

            game.players[ai].activePawns++;
        }
    }

    game.turn++;
    game.hidingTurn++;
}


void ai_movement(int ai, int placed[MAX_PLAYERS])
{
    /* Seul le joueur est capable de cliquer sur l'écran */
    /* Il faut donc passer le tour du deuxieme joueur qui sera joué par l'ia ici.*/
    game.turn++;
    game.hidingTurn++;

    int u = -1, v =-1;

    printf("AI : %d\n", getAITypeForPlayer( &(game.players[ai]) ) );
    // Dans le cas de l'ai facile aléatoire ou moyenne, le mouvement est aléatoire.
    // Toute fois une ia moyenne se deplace plus intelligement en recherchant avant chaque tour des opportunités (voir searchOpportunity())
    if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_EASY_RANDOM || getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM )
    {
        int cmp = 0;
        // Opportunité : l'ia peut deplacer un pion pour former un moulin

        if(getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM && searchOpportunity(ai, &u, &v) != 0)
            printf("[IA] Il a trouve une opportunite 1 et a decide de deplacer le pion %d vers le sommet %d\n", u, v);

        else
        {
            cmp = 0;
            u = -1;
            v = -1;

            do
            {
                u = Random(0, MAX_VERTICES-1);

                cmp++;
            }
            while( !(vertices[u]->owner == &game.players[ai] && isPawnMovable(vertices[u], vertices) != 0) );

            printf("[IA] Elle a choisit de deplacer le pion %d\n", u);

            // Une fois un pion aléatoire trouvé, on le fait bouger
            // Pour cela, on aura besoin d'une case vide, on recherche une nouvelle fois!

            cmp = 0;

            do
            {
                v = Random(0, MAX_VERTICES-1);

                //if(cmp > MAX_VERTICES*2) break;

                cmp++;
            }
            while( !(vertices[v]->owner == NULL
                            && Adjacency[u][v] == 1) ) ;

            printf("[IA] Elle a choisit de se deplacer vers le sommet %d\n", v);
        }

    }

    //On le fait bouger ici
    //u étant l'id du sommet qui contient le pion à bouger
    //v étant l'id du sommet vide où sera bouger le pion

    printf("[AI] Mouvement de %d vers %d\n", u, v);

    if(isVertexInMoulin(vertices[u], Lines) != -1)
        deleteMoulins(vertices[u], Lines);

    if(Adjacency[u][v] == 0)
        printf("[AI] Il n'y a pas d'adjacence entre %d et %d\n", u, v);

    movePawn(&game.players[ai], vertices[u], vertices[v]);
    placed[ai] = u;
}

void ai_saut(int ai, int placed[MAX_PLAYERS])
{
    /* Seul le joueur est capable de cliquer sur l'écran */
    /* Il faut donc passer le tour du deuxieme joueur qui sera joué par l'ia ici.*/
    game.turn++;
    game.hidingTurn++;

    int u = -1, v =-1;

    printf("[AI, Saut] %d\n", getAITypeForPlayer( &(game.players[ai]) ) );
    // Dans le cas de l'ai facile aléatoire ou moyenne, le mouvement est aléatoire.
    // Toute fois une ia moyenne se deplace plus intelligement en recherchant avant chaque tour des opportunités (voir searchOpportunity())
    if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_EASY_RANDOM || getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM )
    {
        int cmp = 0;

        do
        {
            u = Random(0, MAX_VERTICES-1);

            cmp++;
        }
        while( !(vertices[u]->owner == &game.players[ai] && isPawnMovable(vertices[u], vertices) != 0) );

        printf("[AI, Saut] Elle a choisit de deplacer le pion %d\n", u);

        // Une fois un pion aléatoire trouvé, on le fait bouger
        // Pour cela, on aura besoin d'une case vide, on recherche une nouvelle fois!

        cmp = 0;

        do
        {
            v = Random(0, MAX_VERTICES-1);

            //if(cmp > MAX_VERTICES*2) break;

            cmp++;
        }
        while( !(vertices[v]->owner == NULL) ) ;

        printf("[AI, Saut] Elle a choisit de se deplacer vers le sommet %d\n", v);
    }

    //On le fait bouger ici
    //u étant l'id du sommet qui contient le pion à bouger
    //v étant l'id du sommet vide où sera bouger le pion

    printf("[AI, Saut] Mouvement de %d vers %d\n", u, v);

    if(isVertexInMoulin(vertices[u], Lines) != -1)
        deleteMoulins(vertices[u], Lines);

    movePawn(&game.players[ai], vertices[u], vertices[v]);
    placed[ai] = u;
}

void ai_moulin(int ai, int moulinID)
{
    int id = 1 - ai;

    int u = -1;

    if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_EASY_RANDOM || getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM )
    {
        /* Vérification si un moulin peut être former par le joueur */

        int k, focus, focusID;
        int cmp; /*  Il est possible de contrer la formation du moulin du joueur adverse */

        for(int i = 0; i < 16; i++)
        {
            if(Lines[i][3] != LINE_INACTIVE) continue;

            cmp = 0;

            int choices[3];

            for(int a = 0; a < 3; a++) choices[a] = 0;

            for(int a = 0; a < 3; a++)
            {
                k = Lines[i][a];
                if(vertices[k]->owner == &(game.players[id]))
                {
                    choices[a] = -1;

                    cmp++;
                }

                else if(vertices[k]->owner == &(game.players[ai]))
                {
                    choices[a] = 1;

                    cmp--;
                }
            }

            if(cmp == 2)
            {
                focus = i;

                for(int a = 0; a < 3; a++)
                {
                    if(choices[a] == -1) u = Lines[i][a];
                }

                focusID = 1;
                break;
            }
        }

        if(cmp != 2)
        {
            do
            {
                u = Random(0, MAX_VERTICES-1);
                printf("[IA] %d\n", u);
            }
            while(vertices[u]->owner != &(game.players[id]) || isVertexInMoulin(vertices[u], Lines) != -1);
        }

        else printf("[IA, ai_moulin] Moulin %d est sur le point d'etre former ! [cas:%d]\n", focus, focusID);

    }

    /*else if( getAITypeForPlayer( &game.players[ai] ) == AI_TYPE_MEDIUM )
    {
        Situation* courante = generateSituation(NULL, &game, vertices, 1, 1-ai);

        printf("courante %p\n", courante);

        printf("\n\n\n\n\n\n\n\n\t______________ MINIMAX-MOULIN _______________\n");

        initialDepth = 2;

        if(situationsPile != NULL)
        {
            free(situationsPile);

            printf("Liberation de la pile des situations\n");

            situationsPile = NULL;
        }

        Situation* decision = minimax(courante, initialDepth, ai, 1, 1-ai);

        printf("Resultat de minimax: %p, %d\n", decision, decision->moulin);

        free(courante);

        u = decision->moulin;

        free(decision);
    }*/

    // Suppression du pion par l'IA
    printf("[IA] Le pion %d a ete supprime par l'IA.\n",u);

    if(u != -1)
    {
        deletePawn(vertices[u]);

        Lines[ moulinID ][3]    =   LINE_USED;

        game.players[1-ai].activePawns--;
    }
}

int searchOpportunity(int ai, int* u, int* v)
{
    for(int i = 0; i < MAX_LINES; i++)
    {
        if(Lines[i][3] != LINE_INACTIVE) continue;

        if(moulinCanBeFormed(ai, i, vertices, Lines))
        {
            printf("\t[CanBeFormed] %d\n", i);

            for(int k = 0; k < MAX_VERTICES; k++) if(vertices[k]->owner == &game.players[ai])
            {
                if(isVertexAdjacentToLine(vertices[k], i, v, vertices, Lines, Adjacency))
                    if(k != *v && vertices[*v]->owner == NULL && Adjacency[*v][k] == 1)
                    {
                        *u = k;
                        return 1;
                    }
            }
        }
    }

    return 0;
}

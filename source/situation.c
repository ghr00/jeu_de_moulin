

/* Cette fonction calcule le nombre de moulins pour chaque joueur.*/
void countMoulins(Situation* situation, int* moulin1, int id1, int* moulin2, int id2)
{
    int u;

    int cmp[2];

    *moulin1 = 0;
    *moulin2 = 0;

    for(int i = 0; i < MAX_LINES; i++)
    {
        if(Lines[i][3] != LINE_INACTIVE) continue;

        cmp[0] = 0;
        cmp[1] = 0;

        const char* s1;
        const char* s2;

        for(int p = 0; p < 3; p++)
        {
            u = Lines[i][p];

            if(situation->vertices[u]->owner != NULL)
            {
                s1 = situation->vertices[u]->owner->pseudo;
                s2 = situation->game.players[id1].pseudo;

                if(!strcmp(s1, s2))
                {
                    cmp[0]++;
                    //printf("[%d, %d] CMP0 LA ZUP\n", i, u);
                }

                s2 = situation->game.players[id2].pseudo;

                if(!strcmp(s1, s2))
                {
                    cmp[1]++;
                    //printf("[%d, %d] CMP1 LA ZUP\n", i, u);
                }
            }


        }

        if(cmp[0] == 3)
            (*moulin1)++;

        if(cmp[1] == 3)
            (*moulin2)++;

        printf("\t\t[Moulin%d] %d\t[Moulin%d] %d\n", id1, cmp[0], id2, cmp[1]);
    }
}

/* La fonction d'évaluation prend en parametre une situation et l'id du joueur courant et calcule à partir de vertices[] la difference
    entre le nombre de pions  du joueur et et de l'ia et entre le nombre de moulins du joueur et de l'ia. */
int eval(Situation* situation, int id)
{
    int value = 0;
    int absValue = 0;

    int a = 0; int b = 0;

    int moulins[MAX_PLAYERS]; // nombre de moulins par joueur
    int actives[MAX_PLAYERS]; // nombre de pions par joueur

    int ai = 1;

    if(id == 1)
    {
        id = 0;
    }

    countMoulins(situation, &(moulins[id]), id, &(moulins[ai]), ai);
    getActivePawns(situation, &(actives[id]), id, &(actives[ai]), ai);

    a = A_EVAL*(actives[ai] - actives[id]);

    b = B_EVAL*(moulins[ai] - moulins[id]);

    value = a + b;
    absValue = abs(value);

    printf("[eval] countMoulins:%d %d, activePawns: %d %d, value: %d, absValue: %d\n", moulins[id], moulins[1-id], situation->game.players[id].activePawns, situation->game.players[1-id].activePawns, value, absValue);

    return value;//absValue;
}

Situation* empilerSituation(Situation* newElement)
{
    if(situationsPile == NULL)
    {
        printf("[Pile] La pile n'existe pas.\n");
        return NULL;
    }

    if(newElement == NULL)
    {
        printf("\n[Pile] Erreur, on cherche à empiler une situation vide.\n");
        return NULL;
    }

    // Pour ajouter un element à la pile il suffit de permutter ce nouveau element avec la queue de la pile.
    Situation* oldQueue = situationsPile;
    situationsPile = newElement;
    newElement->previous = oldQueue;

    return situationsPile;
}

// On enleve la queue de la pile et on la retourne, de ce fait la pile devient defini par l'element précédent à l'ancienne queue.
Situation* depilerSituation()
{
    if(situationsPile == NULL)
        return NULL;

    Situation* oldQueue = situationsPile;
    Situation* newQueue = situationsPile->previous;
    situationsPile = newQueue;

    return oldQueue;
}

int countSituations()
{
    Situation* element = situationsPile;

    printf("queue suivant : %p, queue: %p\n", element, element->previous);

    int count = 0;
    while(element->previous != NULL)
    {
        //printf("\tcount %d %p", count, element);
        element = element->previous;

        //printf(" %p\n", element);
        count++;
    }

    return count;
}

int isGameFinished(Situation* situation)
{
    if(situation->game.active && situation->game.players[0].pawns < 1 && isPlayerBlocked(&(situation->game),situation->vertices, 0))
        return 1;

    if(situation->game.active && situation->game.players[1].pawns < 1 && isPlayerBlocked(&(situation->game),situation->vertices, 1))
        return 1;

    if(situation->game.active && situation->game.players[0].pawns < 1 && situation->game.players[0].activePawns < 3) // le joueur 1 a gagné
        return 1;

    if(situation->game.active && situation->game.players[1].pawns < 1 && situation->game.players[1].activePawns < 3) // le joueur 2 a gagné
        return 1;

    return 0;
}

Situation* searchParent(Situation* initiale, int id)
{
    Situation* element = initiale;

    while(element->parent != NULL)
    {
        printf("[[%d]]\t", eval(element, id ));
        element = element->parent;
    }

    //printf("\n");
    return element;
}

Situation* generateSituation(Situation* parent, int moulin, int cible)
{
    Situation* newElement = (Situation*)malloc(sizeof(Situation));

    newElement->game.turn = parent->game.turn;
    newElement->game.players[0].activePawns = parent->game.players[0].activePawns;
    newElement->game.players[1].activePawns = parent->game.players[1].activePawns;
    newElement->game.players[0].pawns = parent->game.players[0].pawns;
    newElement->game.players[1].pawns = parent->game.players[1].pawns;
    strcpy(newElement->game.players[0].pseudo, parent->game.players[0].pseudo);
    strcpy(newElement->game.players[1].pseudo, parent->game.players[1].pseudo);
    newElement->previous = NULL;
    newElement->parent = NULL;
    if(parent != NULL) newElement->parent = parent;
    newElement->value = 0;

    //int id = convertTurn(newElement->game.turn);

    //printf("active[0] = %d\n", game->players[0].activePawns);
    //printf("active[1] = %d\n", game->players[1].activePawns);

    const char* s1;
    const char* s2;

    for(int k = 0; k < MAX_VERTICES; k++)
    {
        newElement->vertices[k] = (Vertex*)malloc(sizeof(Vertex));
        newElement->vertices[k]->id = k;

        if(moulin == 1)
        {
            if(parent->vertices[k]->owner != NULL)//&(game->players[cible]))
            {
                s1 = parent->vertices[k]->owner->pseudo;
                s2 = parent->game.players[cible].pseudo;

                if(!strcmp(s1, s2))
                {
                    newElement->vertices[k]->owner = NULL;
                    newElement->moulin = k;
                }

                else newElement->vertices[k]->owner = parent->vertices[k]->owner;
            }
        }

        else
        {
            if(parent->vertices[k]->owner != NULL)
            {
                newElement->vertices[k]->owner = parent->vertices[k]->owner;
            }

            else newElement->vertices[k]->owner = NULL;//&(parent->game.players[id]);
        }

        for(int p = 0; p < 4; p++)
            newElement->vertices[k]->list[p] = parent->vertices[k]->list[p];
    }

    return newElement;
}

Situation* generateInitialeSituation(Game* game, Vertex* vertices[], int moulin, int cible)
{
    Situation* newElement = (Situation*)malloc(sizeof(Situation));

    newElement->game.turn = game->turn;
    newElement->game.players[0].activePawns = game->players[0].activePawns;
    newElement->game.players[1].activePawns = game->players[1].activePawns;
    newElement->game.players[0].pawns = game->players[0].pawns;
    newElement->game.players[1].pawns = game->players[1].pawns;
    strcpy(newElement->game.players[0].pseudo, game->players[0].pseudo);
    strcpy(newElement->game.players[1].pseudo, game->players[1].pseudo);
    newElement->previous = NULL;
    newElement->parent = NULL;
    newElement->value = 0;

    //printf("active[0] = %d\n", game->players[0].activePawns);
    //printf("active[1] = %d\n", game->players[1].activePawns);

    const char* s1;
    const char* s2;

    for(int k = 0; k < MAX_VERTICES; k++)
    {
        newElement->vertices[k] = (Vertex*)malloc(sizeof(Vertex));
        newElement->vertices[k]->id = k;

        if(moulin == 1)
        {
            if(vertices[k]->owner != NULL)//&(game->players[cible]))
            {
                s1 = vertices[k]->owner->pseudo;
                s2 = game->players[cible].pseudo;

                if(!strcmp(s1, s2))
                {
                    newElement->vertices[k]->owner = NULL;
                    newElement->moulin = k;
                }

                else newElement->vertices[k]->owner = vertices[k]->owner;
            }
        }

        else
        {
            if(vertices[k]->owner != NULL)
            {
                newElement->vertices[k]->owner = vertices[k]->owner;
            }

            else
            {
                newElement->vertices[k]->owner = NULL;
            }
        }

        for(int p = 0; p < 4; p++)
            newElement->vertices[k]->list[p] = vertices[k]->list[p];
    }

    return newElement;
}

// * const Situation situation: la situation courante.
void nextSituations(Situation* situation, int depth, int moulin, int cible)
{
    int id = convertTurn(situation->game.turn);

    printf("  __________ nextSituations (tour :%d, id:%d, cible:%d, pawns:%d) ___________\n", situation->game.turn, id, cible, situation->game.players[id].pawns);

    if(moulin == 1)
    {
        for(int k = 0; k < MAX_VERTICES; k++)
        {
            if(situation->vertices[k]->owner == NULL) // Lors de la phase de positionnement, chaque sommet vide genere une situation possible
            {
                Situation* element;

                if(depth == initialDepth)
                {
                    element = generateInitialeSituation(&(situation->game),  situation->vertices, moulin, cible);

                    printf("[Processus] Generation initiale (cible : %d)", cible);
                }

                else
                    element = generateSituation(situation, moulin, cible);

                element->game.players[cible].activePawns--;

                element->moulin = k;

                element->game.turn++;

                empilerSituation(element); //ajoute la nouvelle situation à la pile

                printf("[%d] Generation (%p, %p, %s)\n", k, element, element->previous, element->vertices[k]->owner->pseudo);
            }
        }
    }

    else if(situation->game.players[id].pawns > 0)// si le joueur courant est en phase de postionnement
    {
        printf("\t____________ NextSituations - Positionnement __________________\n");
        for(int k = 0; k < MAX_VERTICES; k++)
        {
            if(situation->vertices[k]->owner == NULL) // Lors de la phase de positionnement, chaque sommet vide genere une situation possible
            {
                Situation* element = NULL;

                if(depth == initialDepth)
                {
                    element = generateInitialeSituation(&(situation->game),  situation->vertices, moulin, cible);

                    printf("[Processus] Generation initiale");
                }

                else
                    element = generateSituation(situation, moulin, cible);

                element->vertices[k]->owner = &(element->game.players[id]);

                element->game.players[id].pawns--;
                element->game.players[id].activePawns++;

                element->game.turn++;

                empilerSituation(element); //ajoute la nouvelle situation à la pile

                printf("[%d] Generation (%p, %p, %s, T%d)\n", k, element, element->previous, element->vertices[k]->owner->pseudo, element->game.turn);
            }
        }
    }

    else // si le joueur courant est en phase de mouvement
    {

    }
}

Situation* minimax(Situation* situation, int depth, int id, int moulin, int cible)
{
    printf("-------MINIMAX, DEPTH:%d, ID:%d, TURN:%d-----------\n", depth, id, situation->game.turn);

    printf("---Owners---\n");
    printf("Joueur[0] : %s\n", situation->game.players[0].pseudo );
    printf("Joueur[1] : %s\n", situation->game.players[1].pseudo );

    for(int k = 0; k < MAX_VERTICES; k++)
        if(situation->vertices[k]->owner != NULL)
            printf("\tSommet %d: %s\n", k, situation->vertices[k]->owner->pseudo);
        else
            printf("\tSommet %d: null\n", k);

    if(isGameFinished(situation) || depth == 0)
    {
        //Situation* decision = generateSituation(situation, &(situation->game), situation->vertices);
        printf("________________ Recherche du parent ____________________\n");
        Situation* decision = searchParent(situation, id);
        printf("_________________________________________________________\n");
        int coeff = 1;

        /*if(id == 1)//(isPlayerAI( situation->game.players[id] ))
        {
            coeff = 1;
        }

        else
        {
            coeff = -1;
        }*/

        decision->value = eval(situation, id) * coeff;


        return decision;
    }

    printf("SituationPile:%p\n", situationsPile);
    if(situationsPile == NULL)
    {
        situationsPile = (Situation*)malloc(sizeof(Situation));
        situationsPile->previous = NULL;
        situationsPile->value = 0;
        printf("[Minimax] Creation de la pile des situations.\n");
    }

    // On part d'une situation courante pour produire une pile de situations possibles..
    nextSituations(situation, depth, moulin, cible);

    printf("\t[MINIMAX]nombre de situations generes :%d\n", countSituations());

    int e;

    //if(isPlayerAI(situation->game.players[id])) // maximizer
    if(id == 1) // maximizer
    {
        int max = eval(situation, id);
        Situation* decision = situation;

        Situation* pile; // le curseur qui va permettre de parcourir toute la pile des situations

        pile = depilerSituation(situationsPile); // on retire le dernier element de la queue

        while( (pile->previous)->previous != NULL) // ..tant qu'on est pas arrivé à la tete de la pile
        {
            e = eval(pile, id);

            if(e >= max) // on verifie si cette situation est la plus valeureuse, si c'est le cas, elle devient le max.
            {
                max = e;

                decision            =   pile;
                decision->value     =   max;
            }

            pile = depilerSituation(situationsPile); // on retire le dernier element de la queue

            printf("\t[Pile] on vient de depiler : %p, son precedent est %p, sa valeur est %d\n", pile, pile->previous, e);

            for(int k = 0; k < MAX_VERTICES; k++)
                if(situation->vertices[k]->owner != NULL)
                    printf("\tSommet %d: %s\n", k, situation->vertices[k]->owner->pseudo);
                else
                    printf("\tSommet %d: null\n", k);
        }

        //freeSituations(decision);

        printf("La decision prise pour une profondeur %d est %p de valeur (%d)\n", depth, decision, max);
        printf("Le nombre de tour: %d, le joueur: %d, countSituations: \n", decision->game.turn, id);

        return minimax(decision, depth-1, 1-id, moulin, cible);
    }

    else // minimizer
    {
        int min = eval(situation, id);
        Situation* decision = situation;

        Situation* pile; // le curseur qui va permettre de parcourir toute la pile des situations

        pile = depilerSituation(situationsPile); // on retire le dernier element de la queue

        int k = 0;
        while( (pile->previous)->previous != NULL) // ..tant qu'on est pas arrivé à la tete de la pile
        {
            e = eval(pile, id);

            if(e <= min) // on verifie si cette situation est la plus valeureuse, si c'est le cas, elle devient le min.
            {
                min = e;

                decision            =   pile;
                decision->value     =   min;
            }

            pile = depilerSituation(situationsPile); // on retire le dernier element de la queue

            printf("\t[Pile] on vient de depiler : %p, son precedent est %p, sa valeur est %d\n", pile, pile->previous, e);

            for(k = 0; k < MAX_VERTICES; k++)
                if(situation->vertices[k]->owner != NULL)
                    printf("\tSommet %d: %s\n", k, situation->vertices[k]->owner->pseudo);
                else
                    printf("\tSommet %d: null\n", k);
        }

        printf("La decision prise pour une profondeur %d est %p de valeur (%d)\n", depth, decision, min);
        printf("Le nombre de tour: %d, le joueur: %d, countSituations() : \n", decision->game.turn, id);

        return minimax(decision, depth-1, 1-id, moulin, cible);
    }
}

int isPlayerBlocked(Game* game, Vertex* vertices[], int id)
{
    if(game->players[id].activePawns == 0)
        return 0;

    int state = 1;

    for(int i = 0; i < MAX_VERTICES; i++)
    {
        if(vertices[i]->owner != &(game->players[id])) continue;

            if(isPawnMovable(vertices[i], vertices))
            {
                state = 0;
                break;
            }

    }

    return state;
}

void getActivePawns(Situation* situation, int* p1, int id1, int* p2, int id2)
{
    if(situation == NULL)
    {
        printf("\n\n\aSituation nulle");
        return;
    }

    *p1 = 0; *p2 = 0;

    const char* s1;
    const char* s2;
    const char* s3;

    for(int i = 0; i < MAX_VERTICES; i++)
    {
        if(situation->vertices[i]->owner == NULL) continue;

        s1 = situation->vertices[i]->owner->pseudo;
        s2 = situation->game.players[id1].pseudo;
        s3 = situation->game.players[id2].pseudo;

        if(!strcmp(s1, s2))
        {
            (*p1)++;

            situation->game.players[id1].activePawns = *p1;
        }

        else if(!strcmp(s1, s3))
        {
            (*p2)++;

            situation->game.players[id2].activePawns = *p2;
        }
    }
}

/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier devait servir à définir les décomptes, comme le décompte actuel du jeu a été directement
                integré au main.c, ce fichier reste inutilisable pour le moment.*/

#ifndef COUNTDOWN_H
#define COUNTDOWN_H

typedef struct countdown
{
    Text* text;

    char str[16];

    int value;
    int initialValue;
    int maximum;
    int minimum;

    int visible;

    int step;
}Countdown;

Countdown* initializeCountdown(SDL_Renderer* renderer, SDL_Color color, int visible, int initialValue, int minimum, int maximum, int step);
//Uint32 updateCountdown(Uint32 intervalle, void* _countdown);
void drawCountdown(Countdown* countdown);
void destroyCountdown(Countdown* countdown);
#include "source/countdown.c"

#endif

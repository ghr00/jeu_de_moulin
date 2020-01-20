/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier d�finie un �cran d'affichage, ils sont de deux types : l'�cran du jeu (SCREEN_TYPE_GAME) est le plateau
                du jeu et l'�cran de menu (SCREEN_TYPE_MENU) est un interface d'utilisateur (un menu avec des widgets).
                'Screen' et 'Widget' sont ind�pendants du jeu et peuvent �tre utilis�s dans n'importe quel projet bas� sur la SDL2.
*/

#ifndef SCREEN_H
#define SCREEN_H

#define MAX_WIDGETS 18

// Un �cran contient les widgets, si un �cran est actif tous ses widgets sont affich�s
enum screenType
{
    SCREEN_TYPE_NONE,
    SCREEN_TYPE_MENU,
    SCREEN_TYPE_GAME
};
typedef struct screen
{
	Widget** widgets;

	int state; // 1 : actif, 0 : inactif

	int type;
}Screen;

Screen* initializeScreen(int type);
int addWidgetToScreen(Screen* screen, char* fileImage, enum widgetType type, int x, int y, SDL_Renderer* renderer);
int showScreen(Screen* screen);
int hideScreen(Screen* screen);
int getClickedWidget(Screen* gameScreen, SDL_Rect* MS, Widget* widget);
int destroyScreen(Screen* screen);
#include "source/screen.c"

#endif

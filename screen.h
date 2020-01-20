/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier définie un écran d'affichage, ils sont de deux types : l'écran du jeu (SCREEN_TYPE_GAME) est le plateau
                du jeu et l'écran de menu (SCREEN_TYPE_MENU) est un interface d'utilisateur (un menu avec des widgets).
                'Screen' et 'Widget' sont indépendants du jeu et peuvent être utilisés dans n'importe quel projet basé sur la SDL2.
*/

#ifndef SCREEN_H
#define SCREEN_H

#define MAX_WIDGETS 18

// Un écran contient les widgets, si un écran est actif tous ses widgets sont affichés
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

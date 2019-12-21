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

	int state;

	int type; // si SCREEN_TYPE_GAME, alors les peds/vehicles seront dessinés si le screen est actif
}Screen;

Screen* initializeScreen(int type);
int addWidgetToScreen(Screen* screen, char* fileImage, enum widgetType type, int x, int y, SDL_Renderer* renderer);
int showScreen(Screen* screen);
int hideScreen(Screen* screen);
int getClickedWidget(Screen* gameScreen, SDL_Rect* MS, Widget* widget);
int destroyScreen(Screen* screen);
#include "screen.c"

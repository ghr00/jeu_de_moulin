
/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier facilite la création de nouveaux composants d'un menu (widget), il est indépendant du jeu et peut être
                utilisé dans n'importe quel projet basé sur la SDL2.
                Un ensemble de widget est un 'screen' (voir screen.h).
*/

#ifndef WIDGET_H
#define WIDGET_H

enum widgetType
{
	WIDGET_TYPE_BUTTON,
	WIDGET_TYPE_IMAGE
};

enum widgetClickable
{
    WIDGET_NOT_CLICKABLE=0,
    WIDGET_CLICKABLE=1
};
typedef struct widget
{
	enum widgetType type;

	int clickable;

	int clicked;

	Sprite sprite;
}Widget;

Widget* createWidget(char* fileImage, enum widgetType type, int x, int y, SDL_Renderer* renderer);
int drawWidget(Widget* widget);
int destroyWidget(Widget* widget);
int isMouseOnWidget(Widget* widget, SDL_Rect* MS);
int isWidgetVisible(Widget* widget);
int setWidgetColor(Widget* widget, SDL_Color color);
int setWidgetVisible(Widget* widget, int state);
int setWidgetClickable(Widget* widget, int clickable);
#include "source/widget.c"

#endif // WIDGET_H

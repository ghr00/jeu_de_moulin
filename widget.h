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
#include "widget.c"

int drawWidget(Widget* widget)
{
	if(widget == NULL) return 0;

	if(&(widget->sprite) == NULL) return -1;


    if(widget->clickable && widget->clicked)
    {
        SDL_Color red = {255, 127, 40, 255};

        setWidgetColor(widget, red);
    }
	drawSprite( &(widget->sprite) );
	return 1;
}

Widget* createWidget(char* fileImage, enum widgetType type, int x, int y, SDL_Renderer* renderer)
{
	Widget* widget = (Widget*)malloc(sizeof(Widget));

	widget->sprite.renderer= renderer;
    widget->sprite.imageFile = fileImage;

    loadSprite(&(widget->sprite));

    setSpritePosition(&(widget->sprite), x, y);

	widget->type = type;

	widget->clickable = 1;
	widget->clicked = 0;

	if(type == WIDGET_TYPE_BUTTON) widget->clickable = 1;

	return widget;
}

int destroyWidget(Widget* widget)
{
	if(widget == NULL) return 0;

	unloadSprite(&(widget->sprite));

	free(widget);
	return 1;
}

int isMouseOnWidget(Widget* widget, SDL_Rect* MS)
{
	if(SDL_HasIntersection(MS, &(widget->sprite.rect) ) == SDL_TRUE)
	{
		return 1;
	}

	return 0;
}

int isWidgetVisible(Widget* widget)
{
	if(widget == NULL) return -1;

	if(&(widget->sprite) == NULL) return -2;

	return isVisible(&(widget->sprite));
}

int setWidgetColor(Widget* widget, SDL_Color color)
{
    if(widget == NULL) return -1;

	if(&(widget->sprite) == NULL) return -2;

	changeColorSprite(&(widget->sprite), color);
	return 1;
}

int setWidgetVisible(Widget* widget, int state)
{
    if(widget == NULL) return -1;

	if(&(widget->sprite) == NULL) return -2;

	setVisibilityState(&(widget->sprite), state);
	return 1;
}

int setWidgetClickable(Widget* widget, int clickable)
{
    if(widget == NULL) return -1;

    widget->clickable = clickable;
    return 1;
}
// ______________________________________________

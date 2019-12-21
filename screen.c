Screen* initializeScreen(int type)
{
	Screen* screen = (Screen*)malloc(sizeof(Screen));

	if(screen == NULL) printf("Erreur dans la création d'un screen\n");

    screen->widgets = (Widget**)malloc(sizeof(Screen) * MAX_WIDGETS);
	screen->state = 0;
    screen->type = type;
	return screen;
}

int addWidgetToScreen(Screen* screen, char* fileImage, enum widgetType type, int x, int y, SDL_Renderer* renderer)
{
    if(screen == NULL) return -1;

    if(renderer == NULL) return -2;

	for(int i = 2; i < MAX_WIDGETS; i++)
	{
		if( (screen->widgets[i]) == NULL)
		{
			screen->widgets[i] = createWidget(fileImage, type, x, y, renderer);

            printf("CR %d\n", i);
			if(screen->widgets[i] != NULL) return 1;

			return -1;
		}
	}

	return 0;
}

int showScreen(Screen* screen)
{
	if(screen == NULL) return 0;

	screen->state = 1;

	for(int i = 2; i < MAX_WIDGETS; i++)
	{
		if(screen->widgets[i] != NULL)
		{
		    if(&(screen->widgets[i]->sprite) == NULL) continue;

			setVisibilityState(&(screen->widgets[i]->sprite), 1);
		}
	}

	return 1;
}

int hideScreen(Screen* screen)
{
	if(screen == NULL) return 0;

	screen->state = 0;

	for(int i = 2; i < MAX_WIDGETS; i++)
	{
		if(screen->widgets[i] != NULL)
		{
			setVisibilityState(&(screen->widgets[i]->sprite), 0);
		}
	}

	return 1;
}

int drawScreen(Screen* gameScreen)
{
	for(int i = 2; i < MAX_WIDGETS; i++)
	{
		if(gameScreen->widgets[i] != NULL)
            printf("draw %d", drawWidget(gameScreen->widgets[i]));
	}

	return 1;
}

int getClickedWidget(Screen* gameScreen, SDL_Rect* MS, Widget* widget)
{
	for(int i = 2; i < MAX_WIDGETS; i++)
	{
		if(gameScreen->widgets[i] == NULL) continue;

		if(gameScreen->widgets[i]->clickable == 0) continue;

        if(gameScreen->widgets[i]->type == WIDGET_TYPE_IMAGE) continue;

		if(isMouseOnWidget(gameScreen->widgets[i], MS))
		{
			widget =  gameScreen->widgets[i];
			return i;
		}
	}

	return -1;
}

int destroyScreen(Screen* screen)
{
    if(screen == NULL) return 0;

    for(int i = 0; i < MAX_WIDGETS; i++)
    {
        if(screen->widgets[i] != NULL) free(screen->widgets[i]);
    }

    return 1;
}

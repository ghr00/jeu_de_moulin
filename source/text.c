Text* createText(SDL_Renderer* renderer, const char* fontFile, int fontSize, const char* value, int x, int y, SDL_Color color, int visible)
{
    Text* text = (Text*)malloc(sizeof(Text));

    text->renderer = renderer;
    text->text = value;
    text->visibile = visible;
    text->color = color;

    text->font = TTF_OpenFont(fontFile, fontSize);

    if(text->font == NULL)
    {
        printf("Erreur createText() : %s ", TTF_GetError());
        return NULL;
    }

    text->image = TTF_RenderText_Solid(text->font, value, color);

    if(text->image == NULL)
    {
        printf("Erreur createText() : %s ", TTF_GetError());
        return NULL;
    }

    text->texture = SDL_CreateTextureFromSurface(renderer, text->image);

    if(text->image == NULL)
    {
        printf("Erreur createText() : %s ", SDL_GetError());
        return NULL;
    }

    text->rect.x = x;
    text->rect.y = y;
    text->rect.w = 0;
    text->rect.h = 0;

    text->newText = NULL;

    SDL_QueryTexture(text->texture, NULL, NULL, &text->rect.w, &text->rect.h);
    return text;
}

void setTextPosition(Text* text, int x, int y)
{
    text->rect.x = x;
    text->rect.y = y;
}

void changeTextValue(Text* text, const char* newText)
{
    text->newText = newText;
}

void changeTextColor(Text* text, SDL_Color color)
{
    text->newColor = color;
}

void updateText(Text* text)
{
    if(text->newText == NULL) return;

    text->text = text->newText;

    text->newText = NULL;

    unloadText(text);

    text->image = TTF_RenderText_Solid(text->font, text->text, text->color);
    text->texture = SDL_CreateTextureFromSurface(text->renderer, text->image);

    SDL_QueryTexture(text->texture, NULL, NULL, &text->rect.w, &text->rect.h);
}

void drawText(Text* text)
{
    if(text->visibile == 0) return;

    SDL_Rect dstrect = { text->rect.x, text->rect.y, text->rect.w, text->rect.h };

    SDL_RenderCopy(text->renderer, text->texture, NULL, &dstrect);
}

void setTextVisible(Text* text, int visible)
{
    text->visibile = visible;
}

void unloadText(Text* text)
{
    if(text->texture != NULL)   SDL_DestroyTexture(text->texture);
    if(text->image != NULL)     SDL_FreeSurface(text->image);
}

void destroyText(Text* text)
{
    unloadText(text);
    TTF_CloseFont(text->font);
    free(text);
}

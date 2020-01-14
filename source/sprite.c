
void loadSprite(Sprite* sprite)
{
    sprite->image = IMG_Load(sprite->imageFile);

    if(sprite->image == NULL)
    {
        fprintf(stderr, "Erreur loadSprite() : %s", SDL_GetError());
        return;
    }

    SDL_SetColorKey(sprite->image, SDL_TRUE, SDL_MapRGB(sprite->image->format, 0, 255, 0));

    sprite->texture = SDL_CreateTextureFromSurface(sprite->renderer, sprite->image);

    SDL_FreeSurface(sprite->image);

    if(sprite->texture == NULL)
    {
        fprintf(stderr, "Erreur loadSprite() : %s", SDL_GetError());
        return;
    }

    setVisibilityState(sprite, 1);

    sprite->rect.x = 0; sprite->rect.y = 0; sprite->angle = 0;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &(sprite->rect.w), &(sprite->rect.h));
}

void changeColorSprite(Sprite* sprite, SDL_Color newColor)
{
    SDL_SetTextureColorMod(sprite->texture, newColor.r, newColor.g, newColor.b);

    sprite->color = newColor;
}

void setSpritePosition(Sprite* sprite, int x, int y)
{
    sprite->rect.x = x;
    sprite->rect.y = y;
}

void moveSpriteBy(Sprite* sprite, int x, int y)
{
    sprite->rect.x += x;
    sprite->rect.y += y;
}

void rotateSpriteBy(Sprite* sprite, double a)
{
    sprite->angle += a;
}

void drawSprite(Sprite* sprite)
{
    if(sprite->visibile)
        SDL_RenderCopyEx(sprite->renderer, sprite->texture, NULL, &(sprite->rect), sprite->angle, &(sprite->center), SDL_FLIP_NONE );
}

void setVisibilityState(Sprite* sprite, int state)
{
    sprite->visibile = state;
}

int isVisible(Sprite* sprite)
{
    if(sprite->visibile == 0) return 0;

    else return 1;
}

void unloadSprite(Sprite* sprite)
{
    SDL_FreeSurface(sprite->image);

    SDL_DestroyTexture(sprite->texture);

    free(sprite);
}


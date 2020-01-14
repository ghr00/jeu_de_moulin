/**
Auteur      :   Amine Yahemdi.
Description :   ce fichier facilite la création de nouveaux sprites, il est indépendant du jeu et peut être utilisé dans n'importe quel
                projet basé sur la SDL2.
*/
typedef struct sprite
{
    SDL_Surface* image;
    SDL_Texture* texture;
    SDL_Color color;
    SDL_Renderer* renderer;

    int visibile;

    double angle;

    SDL_Point center;

    char* imageFile;

    SDL_Rect rect;
}Sprite;


void loadSprite(Sprite* sprite);
void changeColorSprite(Sprite* sprite, SDL_Color newColor);
void drawSprite(Sprite* sprite);
void setSpritePosition(Sprite* sprite, int x, int y);
void moveSpriteBy(Sprite* sprite, int x, int y);
void rotateSpriteBy(Sprite* sprite, double a);
void unloadSprite(Sprite* sprite);
int isVisible(Sprite* sprite);
void setVisibilityState(Sprite* sprite, int state);
#include "source/sprite.c"

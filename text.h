
typedef struct text
{
    SDL_Surface* image;
    SDL_Texture* texture;
    SDL_Color color;
    SDL_Color newColor;
    SDL_Renderer* renderer;

    int visibile;

    TTF_Font* font;

    char* text;
    char* newText;

    SDL_Rect rect;
}Text;

Text* createText(SDL_Renderer* renderer, char* fontFile, int fontSize, char* value, int x, int y, SDL_Color color, int visible);
void setTextPosition(Text* text, int x, int y);
void drawText(Text* text);
void setTextVisible(Text* text, int visible);
void unloadText(Text* text);
void destroyText(Text* text);

#include "text.c"

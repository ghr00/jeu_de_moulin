/**
Auteur : Amine YAHEMDI (GHR00)
Description :   La structure text permet de gérer plus facilement les textes qui apparaissent sur l'écran
                Pour afficher un texte à l'écran, il faut le créer avec createText puis le dessiner avec drawText
                dans la boucle principale du jeu, tant que la valeur du champs 'visible' est 1, le texte sera alors
                affiché à l'écran - la fonction setTextVisible permet de changer la valeur du champs 'visible'.
                La fonction unlaodText permet de supprimer la texture. Enfin,il  ne faut pas désallouer le texte de la mémoire
                avec destroyText à la fin du programme.
                Les fichiers text.h et text.c sont indépendants du jeu et peuvent être utilisés dans n'importe quel projet
                qui utilise SDL2 et SDL2_TTF.
*/
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

#include "source/text.c"

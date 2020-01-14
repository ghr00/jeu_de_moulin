/**
Auteur : Amine YAHEMDI (GHR00)
Description :   La structure text permet de g�rer plus facilement les textes qui apparaissent sur l'�cran
                Pour afficher un texte � l'�cran, il faut le cr�er avec createText puis le dessiner avec drawText
                dans la boucle principale du jeu, tant que la valeur du champs 'visible' est 1, le texte sera alors
                affich� � l'�cran - la fonction setTextVisible permet de changer la valeur du champs 'visible'.
                La fonction unlaodText permet de supprimer la texture. Enfin,il  ne faut pas d�sallouer le texte de la m�moire
                avec destroyText � la fin du programme.
                Les fichiers text.h et text.c sont ind�pendants du jeu et peuvent �tre utilis�s dans n'importe quel projet
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

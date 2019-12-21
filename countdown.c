
Countdown* initializeCountdown(SDL_Renderer* renderer, SDL_Color color, int visible, int initialValue, int minimum, int maximum, int step)
{
    Countdown* countdown = (Countdown*)malloc( sizeof(Countdown) );

    if(countdown == NULL)
    {
        printf("Erreur dans la creation du countdown.");
        return NULL;
    }

    countdown->initialValue = initialValue;
    countdown->maximum = maximum;
    countdown->minimum = minimum; // le step peut aussi prendre des valeurs négatives
    countdown->step = step;
    countdown->value = initialValue;
    countdown->visible = visible;
    countdown->text = createText(renderer, "coolvetica.ttf", 24, "0s", 0, 300, color, visible);

    printf("cd text %p", countdown->text);
    sprintf(countdown->str, "%ds", countdown->value);
    return countdown;
}
/*
Uint32 updateCountdown(Uint32 intervalle, void* _countdown)
{
    Countdown* countdown = (Countdown*)_countdown;

    sprintf(countdown->str, "%ds", countdown->value);
    changeTextValue(countdown->text, countdown->str);

    // Un maximum négatif signifie que le decompte ne possede pas de maximum.
    if( countdown->maximum < 0 ||
        (countdown->minimum <= countdown->value && countdown->value < countdown->maximum) )
        countdown->value += countdown->step;

    return intervalle;
}*/

void drawCountdown(Countdown* countdown)
{
    if(countdown->text != NULL)
        drawText(countdown->text);
}

void destroyCountdown(Countdown* countdown)
{
    free(countdown->text);
    free(countdown);
}

/** Auteur      : Amine YAHEMDI
  * Version     : 0.2.3C
  * Date de fin : 21/01/2020, 12:30
  * Lien Github : github.com/ghr00/jeu_de_moulin
  **/

/************************************************************************************
 *                                  Expat License                                   *
 *  ------------------------------------------------------------------------------  *
 * Copyright 2020 Amine Yahemdi/GHR00                                               *
 *                                                                                  *
 * Permission is hereby granted, free of charge, to any person obtaining a copy     *
 * of this software and associated documentation files (the "Software"), to deal    *
 * in the Software without restriction, including without limitation the rights     *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
 * copies of the Software, and to permit persons to whom the Software is            *
 * furnished to do so, subject to the following conditions:                         *
 *                                                                                  *
 * The above copyright notice and this permission notice shall be included in       *
 * all copies or substantial portions of the Software.                              *
 *                                                                                  *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    *
 * SOFTWARE.                                                                        *
 *                                                                                  *
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#include "ini.h"
#include "sprite.h"
#include "widget.h"
#include "screen.h"
#include "player.h"
#include "ai.h"
#include "vertex.h"
#include "pawn.h"
#include "moulin.h"
#include "game.h"
#include "text.h"
#include "countdown.h"
#include "map.h"
#include "situation.h"
#include "online.h"

#define MAX_SCREENS 3

#define     BOARD_THEME_0   2+0
#define     BOARD_THEME_1   2+1
#define     BOARD_THEME_2   2+2

// Informations de connexion à l'opposant par défaut
#define     HOSTNAME    "172.16.136.93"
#define     PORT        49152

int Random(int min, int max);

void resetTexts();

Uint32 updateCountdown(Uint32 intervalle, void* Count);
Uint32 removeErrorScreen(Uint32 intervalle, void* params);

static int onlineFunction(void* params);
static int drawingFunction(void* params);

// Permet d'entrer en mode listining UDP indépendament du programme
SDL_Thread* onlineThread = NULL;
SDL_Thread* drawingThread = NULL;
SDL_Thread* eventsThread = NULL;

int activeOnlineThread = 0;
int activeDrawingThread = 0;
int activeEventsThread = 0;

static void init(void);
static void end(void);

void WidgetsManager();
void VerticesManager(int id);
void MouvementPhase(int id, int i);
void PlacementPhase(int id, int i);
void MoulinPhase(int id, int i);

void LoadPawns();
void DrawWidgets();
void DrawConfigTexts();

// ------------------------------------- displayErrorScreen :
// Colore temporairement la fenetre si le joueur fait une mauvaise action
// id : id du joueur (0 ou 1)
void displayErrorScreen(int id);

enum menuID
{
    MENU_START,
    MENU_GAME,
    MENU_SETTINGS
};

Game game; // strucutre qui stock tous les informations relatives au jeu (joueurs, tour du jeu actuel)

Screen* screen[MAX_SCREENS];

#define INFO_PLACEMENT  8
#define INFO_MOUVEMENT  9
#define INFO_SAUT       10

#define ERROR_SCREEN    9+2
#define ERROR_TIME      500

SDL_Color orange = {255, 127, 40, 255};
SDL_Color red   = {255, 10, 10, 255};
SDL_Color green = {10, 255, 10, 255};
SDL_Color blue  = {10, 10, 255, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};
SDL_Color grey  = {128, 128, 128, 255};

Text* turnText = NULL;
Text* playerText = NULL;
Text* moulinText = NULL;
Text* gameText = NULL;
Text* countdownText = NULL;
Text* pawnText = NULL;
Text* randomPlayerText = NULL;
Text* gameConfigText[4];
Mix_Music* music;

int partyState = 0; // phase actuel de la partie (placement, mouvement, saut)

int count = 1; // temps du jeu en seconde

int pass = 0;

int lastTime = 0;

SDL_TimerID timer1s = -1; // un timer de 1 seconde
SDL_TimerID timerError = -1; // un timer pour l'ecran d'erreur temporaire

Vertex* focusedVertex[MAX_PLAYERS];

SDL_Renderer* renderer = NULL;

Sprite sprites[PLAYER_INITIAL_PAWNS][MAX_PLAYERS];

int placed[MAX_PLAYERS]; // si le joueur a bien placé un pion

Mix_Chunk* pawnSound;

SDL_Event event;
SDL_bool quit = SDL_FALSE;

SDL_Point MS; // position de la souris sur l'écran

int opponent = -1; // id de l'adversaire dans le cas d'une partie LAN

int menu = MENU_START; // id du screen actuel

char turnChar[32];
char playerChar[32];
char moulinChar[64];
char gameChar[64];
char countdownChar[16];
char randomPlayerChar[32];

int randomPlayer = -1; // l'id du joueur qui a été choisi aléatoirement pour commencer une partie

int main(int argc, char *argv[])
{
    // initialisation des sockets pour la platforme WINDOWS
    init();

    SDL_Window* window = NULL;

    //Countdown* countdown = NULL;

    const char* FONT = getFont();
    const int FONT_SIZE = getFontSize();

    const char* MUSIC_FILE = getMusicFile();
    const int MUSIC_VOLUME = getMusicVolume();

    srand(time(NULL));

    if(0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    /*if(0 != SDLNet_Init())
    {
        fprintf(stderr, "Erreur SDLNet_Init : %s", SDLNet_GetError());
        return EXIT_FAILURE;
    }*/

    if(0 != TTF_Init())
    {
        fprintf(stderr, "Erreur TTF_Init : %s", TTF_GetError());
        return EXIT_FAILURE;
    }

    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted=IMG_Init(flags);

    if((initted&flags) != flags)
    {
        fprintf(stderr, "Erreur IMG_Init(PNG) : %s", IMG_GetError());
        return EXIT_FAILURE;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) //Initialisation de l'API Mixer
    {
        fprintf(stderr, "Erreur Mix_GetError() : %s", Mix_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Jeu de moulin par Amine Yahemdi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 860, 620, SDL_WINDOW_SHOWN);
    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    flags = getRendererFlags();
    renderer = SDL_CreateRenderer(window, -1, flags);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialisation du décompte
    //countdown = initializeCountdown(renderer, black, 0, 1, 0, -1, 1);

    /*loadingThread = SDL_CreateThread(loadingFunction, "loadingFunction", NULL);
    SDL_DetachThread(loadingThread);*/

    LoadPawns();

    // Initilisation des deux écrans : écran du menu de demarrage et écran du plateau du jeu
    screen[0] = initializeScreen(SCREEN_TYPE_MENU); // menuScreen
    screen[1] = initializeScreen(SCREEN_TYPE_GAME); // gameScreen
    screen[2] = initializeScreen(SCREEN_TYPE_MENU); // configScreen

    addWidgetToScreen(screen[0], "interface/main_theme.jpg", WIDGET_TYPE_IMAGE, 0, 0, renderer);
    addWidgetToScreen(screen[0], "interface/button_play.png", WIDGET_TYPE_BUTTON, 190, 210, renderer);
    addWidgetToScreen(screen[0], "interface/button_options.png", WIDGET_TYPE_BUTTON, 180, 310, renderer);
    addWidgetToScreen(screen[0], "interface/button_quit.png", WIDGET_TYPE_BUTTON, 180, 410, renderer);
    addWidgetToScreen(screen[0], "interface/banniere.png", WIDGET_TYPE_IMAGE, 100, 50, renderer);
    addWidgetToScreen(screen[0], "interface/morris.jpg", WIDGET_TYPE_IMAGE, 425, 180, renderer);
    addWidgetToScreen(screen[0], "interface/logo_resized.png", WIDGET_TYPE_IMAGE, 0, 510, renderer);

    addWidgetToScreen(screen[1], "interface/theme_0.jpg", WIDGET_TYPE_IMAGE, 0, 0, renderer); // BOARD_THEME_0 = 0
    addWidgetToScreen(screen[1], "interface/theme_1.jpg", WIDGET_TYPE_IMAGE, 0, 0, renderer); // BOARD_THEME_1 = 1
    addWidgetToScreen(screen[1], "interface/theme_2.jpg", WIDGET_TYPE_IMAGE, 0, 0, renderer); // BOARD_THEME_2 = 2
    addWidgetToScreen(screen[1], "interface/button_remake.png", WIDGET_TYPE_BUTTON, 610, 360, renderer);
    addWidgetToScreen(screen[1], "interface/button_turn.png", WIDGET_TYPE_BUTTON, 610, 400, renderer);
    addWidgetToScreen(screen[1], "interface/button_quit_2.png", WIDGET_TYPE_BUTTON, 635, 440, renderer);
    addWidgetToScreen(screen[1], "interface/info_placement.png", WIDGET_TYPE_IMAGE, 620, 60, renderer);
    addWidgetToScreen(screen[1], "interface/info_mouvement.png", WIDGET_TYPE_IMAGE, 620, 60, renderer);
    addWidgetToScreen(screen[1], "interface/info_saut.png", WIDGET_TYPE_IMAGE, 620, 60, renderer);
    addWidgetToScreen(screen[1], "interface/error_screen.png", WIDGET_TYPE_IMAGE, 0, 0, renderer); // ERROR_SCREEN = 9+2

    setWidgetVisible(screen[1]->widgets[BOARD_THEME_0], 1);
    setWidgetVisible(screen[1]->widgets[BOARD_THEME_1], 1);
    setWidgetVisible(screen[1]->widgets[BOARD_THEME_2], 1);
    setWidgetVisible(screen[1]->widgets[ERROR_SCREEN], 0);

    addWidgetToScreen(screen[2], "interface/main_theme.jpg", WIDGET_TYPE_IMAGE, 0, 0, renderer);
    addWidgetToScreen(screen[2], "interface/button_pvp.png", WIDGET_TYPE_BUTTON, 300, 80, renderer);
    addWidgetToScreen(screen[2], "interface/button_pvai.png", WIDGET_TYPE_BUTTON, 400, 80, renderer);
    addWidgetToScreen(screen[2], "interface/button_lan.png", WIDGET_TYPE_BUTTON, 510, 80, renderer);
    addWidgetToScreen(screen[2], "interface/button_easy.png", WIDGET_TYPE_BUTTON, 300, 180, renderer);
    addWidgetToScreen(screen[2], "interface/button_medium.png", WIDGET_TYPE_BUTTON, 400, 180, renderer);
    addWidgetToScreen(screen[2], "interface/button_no.png", WIDGET_TYPE_BUTTON, 370, 260, renderer);
    addWidgetToScreen(screen[2], "interface/button_yes.png", WIDGET_TYPE_BUTTON, 485, 260, renderer);
    addWidgetToScreen(screen[2], "interface/button_theme_0.jpg", WIDGET_TYPE_BUTTON, 300, 380, renderer);
    addWidgetToScreen(screen[2], "interface/button_theme_1.jpg", WIDGET_TYPE_BUTTON, 420, 380, renderer);
    addWidgetToScreen(screen[2], "interface/button_theme_2.jpg", WIDGET_TYPE_BUTTON, 540, 380, renderer);
    addWidgetToScreen(screen[2], "interface/button_play.png", WIDGET_TYPE_BUTTON, 400, 520, renderer);
    addWidgetToScreen(screen[2], "interface/button_quit.png", WIDGET_TYPE_BUTTON, 520, 520, renderer);

    // Configuration initiale du menu de configuration de la partie
    setWidgetColor(screen[2]->widgets[1+2], green);
    setWidgetColor(screen[2]->widgets[4+2], grey);
    setWidgetColor(screen[2]->widgets[5+2], grey);
    setWidgetColor(screen[2]->widgets[6+2], green);
    setWidgetColor(screen[2]->widgets[7+2], black);
    setWidgetClickable(screen[2]->widgets[4+2], WIDGET_NOT_CLICKABLE);
    setWidgetClickable(screen[2]->widgets[5+2], WIDGET_NOT_CLICKABLE);

    // Initialisation des outils pour la gestion des textes
    turnText        =   createText(renderer, FONT, FONT_SIZE, "Tour:", 15, 0, black, 1);
    playerText      =   createText(renderer, FONT, FONT_SIZE, "Joueur:", 330, 0, black, 1);
    moulinText      =   createText(renderer, FONT, FONT_SIZE, "Moulin!", 175, 500, black, 0);
    gameText        =   createText(renderer, FONT, FONT_SIZE, "Etat du jeu", 175, 500, black, 0);
    countdownText   =   createText(renderer, FONT, FONT_SIZE, "0s", 680, 0, black, 1);
    pawnText        =   createText(renderer, FONT, FONT_SIZE, "Les pions", 15, 65, black, 1);
    randomPlayerText=   createText(renderer, FONT, FONT_SIZE, "randomPlayer", 125, 500, black, 1);

    gameConfigText[0]  =   createText(renderer, FONT, 22, "Type de la partie", 80, 80, grey, 0);
    gameConfigText[1]  =   createText(renderer, FONT, 22, "Niveau de l'IA", 80, 180, grey, 0);
    gameConfigText[2]  =   createText(renderer, FONT, 22, "Activer la regle optionnelle", 80, 280, grey, 0);
    gameConfigText[3]  =   createText(renderer, FONT, 22, "Theme du plateau", 80, 380, grey, 0);

    // Initilisation de la partie & joueurs
    char pseudo[MAX_PLAYERS][MAX_USERNAME_LENGTH];
    getPlayersUsernames(pseudo[0], pseudo[1]);
    initializePlayer( &(game.players[0]), pseudo[0], blue); // blue est la couleur du pion du premier joueur
    initializePlayer( &(game.players[1]), pseudo[1], red); // red est la couleur du pion du second joueur

    //createAIForPlayer(AI_TYPE_EASY_RANDOM, &(game.players[0]));
    createAIForPlayer(AI_TYPE_EASY_RANDOM, &(game.players[1])); // Le joueur 2 sera l'IA
    createAIForPlayer(AI_TYPE_MEDIUM, &(game.players[1])); // Le joueur 2 sera l'IA

    //setPlayerAI(&(game.players[0]), AI_TYPE_EASY_RANDOM);
    setPlayerAI(&(game.players[1]), AI_TYPE_EASY_RANDOM); // par défaut l'IA est facile-aléatoire

    // Ce pointeur stock l'adresse du sommet selectionné par le joueur utilisé par la suite lors de la phase du mouvement.
    // L'IA n'utilise pas cette variable.
    focusedVertex[0] = NULL;
    focusedVertex[1] = NULL;

    // Initialisation des parametres de la partie
    initializeGame(&game, GAME_TYPE_PvP, 1);

    // Initilisation des trois carrés formant le plateau du jeu
    initializeSquares(renderer);

    // Il est possible de déduire la liste d'adjacence du graph 'plateau du jeu' à partir de sa matrice d'adjacence.
    for(int u = 0; u < 24; u++) setVertexList(vertices[u], Adjacency);

    // Initilisations des musiques et des sons
    music = Mix_LoadMUS(MUSIC_FILE);
    pawnSound = Mix_LoadWAV("audio/pawn.wav");

    Mix_Volume(1, MUSIC_VOLUME);

    if(music == NULL)
        fprintf(stderr, "Echec du chargement de la musique.");

    if(pawnSound == NULL)
        fprintf(stderr, "Echec du chargement du PAWN_SOUND.");

    Mix_AllocateChannels(2); // nombre de canaux alloués pour le jeu de son courts

    if(0 != SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_RenderClear : %s",SDL_GetError());
        return EXIT_FAILURE;
    }

    while(!quit)
    {
        SDL_GetMouseState(&(MS.x), &(MS.y));

        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
                quit = SDL_TRUE;

            // Le clavier ne sera pas utilisé dans le jeu
            /*if(event.type == SDL_KEYDOWN)
            {
               if(menu == MENU_START)
               {
                   menu = MENU_GAME;
               }

               else if(screen[menu]->type == SCREEN_TYPE_GAME)
               {

               }
            }*/
            if(event.type == SDL_MOUSEBUTTONUP)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                    WidgetsManager();
            }

            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                Widget wid;

                SDL_Rect RMS;
                RMS.h = 2; RMS.w = 2; RMS.x = MS.x; RMS.y = MS.y;

                // On peut optimiser ceci en vérifiant d'abords que le clique est dans le rectangle formé pa les trois bouttons.
                int w = getClickedWidget(screen[menu], &RMS, &wid);

                if(w != -1) screen[menu]->widgets[w]->clicked = 1;

                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    // Une fois en jeu, si le joueur fait un clique gauche dans le plateau du jeu alors..
                    if (menu == MENU_GAME)
                    {
                        if(!game.active) continue;

                        Mix_PlayChannel(1, pawnSound, 0);

                        int id; // id du joueur qui joue actuellement

                        id = convertTurn(game.turn); // [voir le fichier game.h]

                        printf("\n\n[Tour] %d\n", game.turn);

                        placed[0] = -1;
                        placed[1] = -1;

                        /* Le gestionnaire des pions permet de savoir si le joueur est en phase de :
                            - Positionnement.
                            - Mouvement.
                            - Moulin.
                                        */
                        VerticesManager(id);

                        /* Par la suite, la variable placed[id] permet de verifier si le joueur a bien placé/fait bouger
                            un pion sur le plateau. Si c'est le cas, on verifie si le positionnement/le deplacement
                            d'un pion a generé un moulin..*/


                        if(placed[id] != -1)
                        {
                            //printf("\t[LAN] (%d, %d)\n", id, opponent);

                            if(game.type != GAME_TYPE_PvP_ONLINE ||
                               (game.type == GAME_TYPE_PvP_ONLINE && id != opponent) )
                            {

                                int moulinID[4];

                                for(int k = 0; k < 4; k++) moulinID[k] = -1;

                                searchMoulin(&game.players[id], vertices, Lines, moulinID);

                                for(int k = 0; k < 4; k++)
                                    if(moulinID[k] != -1)
                                    {
                                        printf("[Joueur] moulinID[%d](id:%d), %d\n", k, id, moulinID[k]);

                                        game.players[id].moulin++;
                                        game.players[id].moulinID[k]    =   moulinID[k];

                                        printf("\n\t[Joueur] MOULLLIIIN (%d) POUR %s(%d): (%d, %d)\n", k,
                                                game.players[id].pseudo, id, moulinID[k], game.players[id].moulin);
                                    }

                                if(game.players[id].moulin > 0)
                                {
                                    if(game.type != GAME_TYPE_PvAI)
                                    {
                                        //game.hidingTurn++;
                                        game.turn ++;
                                    }
                                }
                            }
                        }

                        /* Même chose que precedemment mais pour l'IA */
                        int ai = 1 - id;

                        // Si l'ia a bien placé/fait bouger un pion sur le plateau..
                        if(game.type == GAME_TYPE_PvAI && placed[ai] != -1)
                        {
                            int moulinID[4];

                            for(int k = 0; k < 4; k++) moulinID[k] = -1;

                            searchMoulin(&game.players[ai], vertices, Lines, moulinID);

                            for(int k = 0; k < 4; k++)
                            {
                                printf("[IA] moulinID(id:%d), %d\n", ai, moulinID[k]);

                                if(moulinID[k] != -1)
                                {
                                    printf("\n\t[IA] MOULLLIIIN POUR %d: (%d)\n", ai, moulinID[k]);

                                    ai_moulin(ai, moulinID[k]);
                                }

                            }
                        }
                    }

                }
            }
        }
        // Cette fonction dessine les widgets (bouttons, images du IU) selon le screen active
        DrawWidgets();

        // Si l'écran de configuration de la partie est actif, cette fonction dessine l'interface
        DrawConfigTexts();

        if(screen[menu] != NULL) if(menu == MENU_GAME)
        {
            ////////////////////// Le drawing du jeu est ici

            //if(timer == -1) timer = SDL_AddTimer(30, moveSprites, ped); /* Démarrage du timer */

            // [LAN]
            if(game.type == GAME_TYPE_PvP_ONLINE)
            {
                //printf("_______________ LAN _________________ \n");
                int id = convertTurn( game.turn );

                if(opponent == -1)
                    getOnlineID(&opponent);

                if(id != 2)
                    if(activeOnlineThread == 0)
                    {
                        activeOnlineThread = 1;

                        onlineThread = SDL_CreateThread(onlineFunction, "onlineFunction", NULL);

                        //SDL_WaitThread(onlineThread, NULL);
                        SDL_DetachThread(onlineThread);

                        onlineThread = NULL;

                        printf("Creation et detachement du thread online\n");
                    }

            }

            if(isPlayerAI(game.players[0]))
                setPlayerAI(&(game.players[0]), game.difficulty);

            if(isPlayerAI(game.players[1]))
                setPlayerAI(&(game.players[1]), game.difficulty);

            SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);

            if(activeDrawingThread == 0)
                drawingFunction(NULL);

            if(partyState != 0)
            {
                if(partyState == INFO_PLACEMENT)
                {
                    setWidgetVisible(screen[1]->widgets[INFO_PLACEMENT], 1);
                    setWidgetVisible(screen[1]->widgets[INFO_MOUVEMENT], 0);
                    setWidgetVisible(screen[1]->widgets[INFO_SAUT], 0);
                }

                if(partyState == INFO_MOUVEMENT)
                {
                    setWidgetVisible(screen[1]->widgets[INFO_PLACEMENT], 0);
                    setWidgetVisible(screen[1]->widgets[INFO_MOUVEMENT], 1);
                    setWidgetVisible(screen[1]->widgets[INFO_SAUT], 0);
                }

                if(partyState == INFO_SAUT)
                {
                    setWidgetVisible(screen[1]->widgets[INFO_PLACEMENT], 0);
                    setWidgetVisible(screen[1]->widgets[INFO_MOUVEMENT], 0);
                    setWidgetVisible(screen[1]->widgets[INFO_SAUT], 1);
                }

            }

            else
            {
                setWidgetVisible(screen[1]->widgets[INFO_PLACEMENT], 0);
                setWidgetVisible(screen[1]->widgets[INFO_MOUVEMENT], 0);
                setWidgetVisible(screen[1]->widgets[INFO_SAUT], 0);
            }

            /*if(countdown != NULL)
                drawCountdown(countdown);*/

            if(countdownText != NULL)
            {
                sprintf(countdownChar, "Compteur:%ds", count);

                changeTextValue(countdownText, countdownChar);

                updateText(countdownText);
                drawText(countdownText);
            }

            if(turnText != NULL)
            {
                int T = game.turn - game.hidingTurn;
                sprintf(turnChar, "Tour:%d", T);

                changeTextValue(turnText, turnChar);

                updateText(turnText);
                drawText(turnText);
            }

            int id = convertTurn(game.turn);

            if(playerText != NULL)
            {
                sprintf(playerChar, "Joueur:%s (%d)", game.players[id].pseudo, game.players[id].pawns);
                playerText->color = game.players[id].color;

                changeTextValue(playerText, playerChar);

                updateText(playerText);
                drawText(playerText);
            }

            if(moulinText != NULL)
            {
                if(game.players[id].moulin != 0) setTextVisible(moulinText, 1);
                else setTextVisible(moulinText, 0);

                sprintf(moulinChar, "Moulin! Vous pouvez supprimer un pion ennemi.\n");

                moulinText->color = game.players[id].color;

                changeTextValue(moulinText, moulinChar);


                updateText(moulinText);
                drawText(moulinText);
            }

            if(pawnText != NULL)
            {
                updateText(pawnText);
                drawText(pawnText);
            }

            // Les conditions pour vérifier la victoire des joueurs sont ici.

            if(game.active != 0 && game.players[0].pawns < 1 && isPlayerBlocked(&game, vertices, 0))
            {
                game.players[0].activePawns = 0;

                printf("\t[Blocked] Le joueur 0 est bloque.\n");
            }

            if(game.active != 0 && game.players[1].pawns < 1 && isPlayerBlocked(&game, vertices, 1))
            {
                game.players[1].activePawns = 0;

                printf("\t[Blocked] Le joueur 1 est bloque.\n");
            }

            /* Ceci permet de vérifier si le jeu vient tout juste d'être lancé, dans ce cas on affiche aux joueurs un message
                comme quoi que le premier joueur qui va placer un pion sur le plateau a été choisi de maniére aléatoire. */
            int onGameStart =(  (game.players[0].activePawns == 0 && game.players[0].pawns > 0) ||
                                (game.players[1].activePawns == 0 && game.players[1].pawns > 0)     );

            if(game.active != 0 && game.type == GAME_TYPE_PvP && gameText != NULL)
            {
                setTextVisible(randomPlayerText, onGameStart);

                if( onGameStart ) // si on est toujours aux premiers tours du jeu
                {
                    if(randomPlayer == -1)
                    {
                        randomPlayer = Random(0, 1);

                        if(randomPlayer == 1)
                        {
                            game.turn++;
                            game.hidingTurn++;

                            id = convertTurn(game.turn);
                        }
                        printf("[RandomPlayer] Aleatoirement, c'est %d qui fut choisi.\n", randomPlayer);
                    }

                    sprintf(randomPlayerChar, "%s a ete choisi aleatoirement pour commencer le jeu",
                            game.players[randomPlayer].pseudo);

                    randomPlayerText->color = game.players[randomPlayer].color;

                    changeTextValue(randomPlayerText, randomPlayerChar);

                    updateText(randomPlayerText);
                    drawText(randomPlayerText);
                }
            }

            if(/*game.active && */game.players[0].pawns < 1 && game.players[0].activePawns < 3) // le joueur 1 a gagné
            {
                game.active = 0;

                if(gameText != NULL)
                {
                    setTextVisible(gameText, !game.active);

                    sprintf(gameChar, "%s a gagne le jeu !", game.players[1].pseudo);

                    gameText->color = game.players[1].color;

                    changeTextValue(gameText, gameChar);

                    updateText(gameText);
                    drawText(gameText);
                }
            }

            if(/*game.active && */game.players[1].pawns < 1 && game.players[1].activePawns < 3) // le joueur 2 a gagné
            {
                game.active = 0;

                if(gameText != NULL)
                {
                    setTextVisible(gameText, !game.active);

                    sprintf(gameChar, "%s a gagne le jeu !", game.players[0].pseudo);

                    gameText->color = game.players[0].color;

                    changeTextValue(gameText, gameChar);


                    updateText(gameText);
                    drawText(gameText);
                }
            }

            if(pass == 3)
            {
                if(gameText != NULL)
                {
                    setTextVisible(gameText, 1);

                    sprintf(gameChar, "Avertissement: La partie sera nulle si vous passez le tour");

                    gameText->color = black;

                    changeTextValue(gameText, gameChar);

                    updateText(gameText);
                    drawText(gameText);
                }
            }

            else if(pass > 3)
            {
                game.active = 0;

                if(gameText != NULL)
                {
                    setTextVisible(gameText, !game.active);

                    sprintf(gameChar, "Partie nulle !");

                    gameText->color = black;

                    changeTextValue(gameText, gameChar);

                    updateText(gameText);
                    drawText(gameText);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a);

        /* Permet de limiter le nombre de FPS.
         * La constante SCREEN_FPS est definie dans game.h
         */
        /*while(lastTime - SDL_GetTicks() < (1000 / SCREEN_FPS))
        {
            SDL_Delay(1);
        }*/

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        lastTime = SDL_GetTicks();

    }

    /* Déchargement */
    if(turnText != NULL)    destroyText(turnText);
    if(playerText != NULL)  destroyText(playerText);
    if(moulinText != NULL)  destroyText(moulinText);
    if(gameText != NULL)    destroyText(gameText);
    for(int i = 0; i < 4; i++) if(gameConfigText[i] != NULL) destroyText(gameConfigText[i]);

    for(int i = 0; i < 3; i++) if( screen[i] != NULL )  destroyScreen(screen[i]);
    destroyPlayer(& (game.players[0]) );
    destroyPlayer(& (game.players[1]) );

    /*if(countdown != NULL)
       destroyCountdown(countdown);*/

    if(music != NULL)
        Mix_FreeMusic(music);
    if(pawnSound != NULL)
        Mix_FreeChunk(pawnSound);

    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    //SDLNet_Quit();
    SDL_Quit();

    end();

    return EXIT_SUCCESS;
}

int Random(int min, int max)
{
    return min + rand() % (max+1 - min);
}

Uint32 updateCountdown(Uint32 intervalle, void* Count)
{
    int* count = (int*)Count;

    (*count)++;

    return intervalle;
}

Uint32 removeErrorScreen(Uint32 intervalle, void* params)
{
    SDL_Color white = {255, 255, 255, 255};

    setWidgetColor(screen[1]->widgets[ERROR_SCREEN], white);
    SDL_SetTextureAlphaMod(screen[1]->widgets[ERROR_SCREEN]->sprite.texture, 255);
    setWidgetVisible(screen[1]->widgets[ERROR_SCREEN], 0);

    SDL_RemoveTimer(timerError);

    return intervalle;
}

void resetTexts()
{
    changeTextValue(turnText, "Tour:");
    changeTextValue(playerText, "Joueur:");
    changeTextColor(playerText, black);
    changeTextValue(moulinText, "Moulin!");
    changeTextValue(gameText, "Etat du jeu");
    changeTextValue(countdownText, "0s");
    changeTextValue(randomPlayerText, "randomPlayer");
}

void displayErrorScreen(int id)
{
	SDL_Color errorScreenColor = game.players[id].color;

	setWidgetColor(screen[1]->widgets[ERROR_SCREEN], errorScreenColor);

    SDL_SetTextureAlphaMod(screen[1]->widgets[ERROR_SCREEN]->sprite.texture, 128);

    setWidgetVisible(screen[1]->widgets[ERROR_SCREEN], 1);

	printf("[ErrorScreen] Affichage de l'ecran d'erreur\n");
}

static void init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

static int onlineFunction(void* params)
{
    int n;

    int id = convertTurn(game.turn);

    char buffer[64];

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    SOCKADDR_IN sin = { 0 };

    struct hostent *hostinfo = NULL;

    const char *hostname = HOSTNAME;
    Uint16 port = PORT;

    hostname = getIPAdress(&port);

    printf("\nHOST %s\tPORT %d\n", hostname, port);

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    if(bind (sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
    {
        perror("bind()");
        exit(EXIT_FAILURE);
    }


    hostinfo = gethostbyname(hostname);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit(EXIT_FAILURE);
    }

    SOCKADDR_IN from = { 0 };

    #ifdef WIN32

    int fromsize = sizeof from;

    #elif defined (linux)

    socklen_t fromsize = sizeof from;

    #endif

    if((n = recvfrom(sock, buffer, sizeof buffer - 1, 0, (SOCKADDR *)&from, &fromsize)) < 0)
    {
        perror("recvfrom()");
        //exit(EXIT_FAILURE);
    }

    else
    {
        buffer[n] = '\0';

        /* traitement */

        printf("Message reçu %s\n", buffer);
    }

    closesocket(sock);

    int reception;

    if(strlen(buffer) != 0)
        reception = 1;

    if(reception == 1)
    {
        //char type = buffer[0];
        char type = '\0';

        int vertex, vertex2;

        sscanf(buffer, "%c%d-%d", &type, &vertex, &vertex2);

        if(type != '\0')
        {
            switch(type)
            {
                case 'o': // positionnement d'un pion
                {
                    printf("Positionnement d'un pion.");
                    break;
                }

                case 'p': // positionnement d'un pion
                {
                    printf("\n\tPositionnement d'un pion au sommet %d\n\n.", vertex);

                    int op = opponent;//1 - id;

                    setVertexOwner(vertices[vertex], &game.players[op]);
                    setPawnVisibilityState(vertices[vertex], 1);

                    placed[op] = vertex;

                    game.players[op].pawns--;

                    game.players[op].activePawns++;

                    game.turn++;

                    printf("[Adversaire-LAN%dvs%d] Il a pose un pion au vertice %d\n", op, id, vertex);

                    pass = 0;

                    int moulinID[4];

                    for(int k = 0; k < 4; k++) moulinID[k] = -1;

                    searchMoulin(&game.players[op], vertices, Lines, moulinID);

                    /*for(int k = 0; k < 4; k++)
                        if(moulinID[k] != -1)
                        {
                            printf("[Joueur-LAN] moulinID[%d](id:%d), %d\n", k, id, moulinID[k]);

                            game.players[op].moulin++;
                            game.players[op].moulinID[k]    =   moulinID[k];

                            printf("\n\t[Joueur-LAN] MOULLLIIIN (%d) POUR %s(%d): (%d, %d)\n", k, game.players[op].pseudo, id, moulinID[k], game.players[op].moulin);
                        }*/


                    break;
                }

                case 'm': // mouvement d'un pion
                {
                    printf("\nMouvement d'un pion du sommet %d au sommet %d\n\n.", vertex, vertex2);

                    int op = 1 - id;//opponent;

                    movePawn(&game.players[op], vertices[vertex], vertices[vertex2]);

                    placed[op] = vertex2;

                    game.turn++;

                    int moulinID[4];

                    for(int k = 0; k < 4; k++) moulinID[k] = -1;

                    searchMoulin(&game.players[op], vertices, Lines, moulinID);

                    break;
                }

                /*case 'f': // formation d'un moulin
                {
                    printf("\tFormation du moulin %d\n\n.", vertex);

                    break;
                }*/

                case 'r': // suppression d'un pion
                {
                    printf("\tSuppression du pion du sommet %d\n\n.", vertex);

                    int op = opponent;//1 - id

                    deletePawn(vertices[vertex]);

                    for(int k = 0; k < 4; k++)
                            Lines[ game.players[op].moulinID[k] ][3]    =   LINE_USED;

                    game.players[op].moulin--;
                    //game.players[id].moulinID = -1;

                    game.players[1-op].activePawns--;

                    if(game.players[op].moulin == 0)
                        game.turn++;

                    break;
                }

                default:
                    printf("\tLe type de message recu est non reconnu.");
            }
        }

        else
        {
            printf("\tLe message reçu est vide.\n");
        }
    }

    else
        printf("\tErreur dans la reception du message, %d\n", reception);

    //onlineThread = NULL;
    activeOnlineThread = 0;

    return 1;
}

static int drawingFunction(void* params)
{
    drawMap(renderer);
    for(int j = 0; j < MAX_PLAYERS; j++)
    {
        for(int i = 0; i < game.players[j].pawns; i++)
            drawSprite( &(sprites[i][j]) );
    }

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
        if(focusedVertex[i] != NULL)
            SDL_SetTextureAlphaMod(focusedVertex[i]->pawn.texture, 100);
    }

    activeDrawingThread = 0;
    return 1;
}

/// Le gestionnaire des pions verifie l'état des pions à chaque tour.
//id: id du joueur jouant actuel
void VerticesManager(int id)
{
    SDL_Rect rect;

    for(int i = 0 ; i < MAX_VERTICES; i++)
    {
        rect = getVertexRectangle( vertices[i] );

        if( SDL_PointInRect(&MS, &rect) )
        {
            if(game.players[id].moulin > 0) // PHASE DE MOULIN (IA : NON)
            {
                MoulinPhase(id, i);
            }

            else
            {
                if(game.players[id].pawns > 0) // PHASE DE PLACEMENT
                    PlacementPhase(id, i);


                else // PHASE DE MOUVEMENT
                    MouvementPhase(id, i);
            }
        }
    }
}

void MoulinPhase(int id, int i)
{
    if(vertices[i]->owner != NULL && vertices[i]->owner == &game.players[1-id])
    {
        //if(vertices[i]->moulin == 1)
        if(isVertexInMoulin(vertices[i], Lines) != -1)
            printf("Vous ne pouvez pas supprimer un pion qui fait partie d'un moulin. (%d)\n", isVertexInMoulin(vertices[i],
                                                                                                                 Lines));
        else
        {
            // Suppression du pion

            deletePawn(vertices[i]);

            for(int k = 0; k < 4; k++)
                    Lines[ game.players[id].moulinID[k] ][3]    =   LINE_USED;

            game.players[id].moulin--;
            //game.players[id].moulinID = -1;

            game.players[1-id].activePawns--;

            if(game.players[id].moulin == 0)
                game.turn++;

            if(game.type == GAME_TYPE_PvAI)
            {
                            /* Seul le joueur est capable de cliquer sur l'écran */
                /* Il faut donc passer le tour du deuxieme joueur qui sera joué par l'ia ici.*/
                game.turn++;
                game.hidingTurn++;
            }

            // On envoie un message qui indique à l'adversaire que le joueur a posé un pion
            // Ainsi, son plateau du jeu doit être mis à jour et ça sera à son tour de jouer
            else if(game.type == GAME_TYPE_PvP_ONLINE)
            {
                char message[8];

                SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
                if(sock == INVALID_SOCKET)
                {
                    perror("socket()");
                    exit(EXIT_FAILURE);
                }

                struct hostent *hostinfo = NULL;

                const char *hostname = HOSTNAME;
                Uint16 port = PORT;

                hostname = getIPAdress(&port);

                printf("\t[Supp] %s:%d", hostname, port);

                hostinfo = gethostbyname(hostname);
                if (hostinfo == NULL)
                {
                    fprintf (stderr, "Unknown host %s.\n", hostname);
                    exit(EXIT_FAILURE);
                }

                    SOCKADDR_IN to = { 0 };
                    to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
                    to.sin_port = htons(port);
                    to.sin_family = AF_INET;
                    int tosize = sizeof to;

                sprintf(message, "r%d-0", i);

                printf("\nMessage de suppression:%s\n", message);

                if(sendto(sock, message, strlen(message), 0, (SOCKADDR *)&to, tosize) < 0)
                {
                    perror("sendto()");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void MouvementPhase(int id, int i)
{
    partyState = INFO_MOUVEMENT; /* permet de changer le texte informatif pour
                                        expliquer la phase de mouvement */

    if(focusedVertex[id] == NULL)
    {
        if(vertices[i]->owner != NULL && vertices[i]->owner != &game.players[id])
            printf("Vous n'etes pas proprietaire de cette case.\n");

        else focusedVertex[id] = vertices[i];
    }

    else
    {
        int j = focusedVertex[id]->id;

        // si la régle optionnelle n'est pas active
        if(game.optionnel == 0 || (game.optionnel == 1 &&
                game.players[id].activePawns > 3))

        {
            if(Adjacency[i][j] == 0)
            {
                printf("Mouvement impossible.");

                SDL_SetTextureAlphaMod(focusedVertex[id]->pawn.texture, 255);

                focusedVertex[id] = NULL;
                //break;
            }

            else if(vertices[i]->owner == NULL)
            {
                int vertex =  focusedVertex[id]->id;

                printf("\n\tisVertexInMoulin %d", isVertexInMoulin(vertices[j], Lines));
                if(isVertexInMoulin(vertices[j], Lines) != -1)
                        deleteMoulins(vertices[j], Lines);

                movePawn(&game.players[id], focusedVertex[id], vertices[i]);

                pass = 0;

                printf("[Joueur] Mouvement de %d vers %d\n", focusedVertex[id]->id, i);

                if(Adjacency[ focusedVertex[id]->id ][i] == 0)
                    printf("[Joueur] Il n'y a pas d'adjacence entre %d et %d\n", focusedVertex[id]->id, i);

                focusedVertex[id] = NULL;

                placed[id] = i;

                game.turn++;

                // Une fois que le joueur a fait bouger son pion, l'ai fera de même
                if(game.type == GAME_TYPE_PvAI) // AI phase de mouvement
                {
                    int ai = 1 - id;

                    if(game.players[ai].activePawns == 3)
                        ai_saut(ai, placed);

                    else
                        ai_movement(ai, placed);
                }

                // On envoie un message qui indique à l'adversaire que le joueur a posé un pion
                // Ainsi, son plateau du jeu doit être mis à jour et ça sera à son tour de jouer
                else if(game.type == GAME_TYPE_PvP_ONLINE)
                {
                    char message[8];

                    //vertexOwnerMessage(message, id, i);

                    //sendMessage(message, 5);

                    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
                    if(sock == INVALID_SOCKET)
                    {
                        perror("socket()");
                        exit(EXIT_FAILURE);
                    }

                    struct hostent *hostinfo = NULL;

                    const char *hostname = HOSTNAME;
                    Uint16 port = PORT;

                    hostname = getIPAdress(&port);

                    hostinfo = gethostbyname(hostname);
                    if (hostinfo == NULL)
                    {
                        fprintf (stderr, "Unknown host %s.\n", hostname);
                        exit(EXIT_FAILURE);
                    }

                    SOCKADDR_IN to = { 0 };
                    to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
                    to.sin_port = htons(port);
                    to.sin_family = AF_INET;
                    int tosize = sizeof to;

                    sprintf(message, "m%d-%d", vertex, i);

                    if(sendto(sock, message, strlen(message), 0, (SOCKADDR *)&to, tosize) < 0)
                    {
                        perror("sendto()");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }

        else if(game.optionnel == 1 && game.players[id].activePawns == 3)// si la régle optionnelle est active
        {
            partyState = INFO_SAUT; /* permet de changer le texte informatif pour
                                            expliquer la phase de placement */

            if(vertices[i]->owner == NULL)
            {
                int vertex = focusedVertex[id]->id;

                printf("\nisVertexInMoulin %d", isVertexInMoulin(vertices[j], Lines));
                if(isVertexInMoulin(vertices[j], Lines) != -1)
                    deleteMoulins(vertices[j], Lines);

                movePawn(&game.players[id], focusedVertex[id], vertices[i]);

                focusedVertex[id] = NULL;

                placed[id] = i;

                game.turn++;

                pass = 0;

                if(game.type == GAME_TYPE_PvAI) // AI phase de mouvement
                {
                    int ai = 1 - id;

                    if(game.players[ai].activePawns == 3)
                        ai_saut(ai, placed);

                    else
                        ai_movement(ai, placed);
                }

                // On envoie un message qui indique à l'adversaire que le joueur a posé un pion
                // Ainsi, son plateau du jeu doit être mis à jour et ça sera à son tour de jouer
                else if(game.type == GAME_TYPE_PvP_ONLINE)
                {
                    char message[8];

                    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
                    if(sock == INVALID_SOCKET)
                    {
                        perror("socket()");
                        exit(EXIT_FAILURE);
                    }

                    struct hostent *hostinfo = NULL;

                    const char *hostname = HOSTNAME;
                    Uint16 port = PORT;

                    hostname = getIPAdress(&port);

                    hostinfo = gethostbyname(hostname);
                    if (hostinfo == NULL)
                    {
                        fprintf (stderr, "Unknown host %s.\n", hostname);
                        exit(EXIT_FAILURE);
                    }

                    SOCKADDR_IN to = { 0 };
                    to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
                    to.sin_port = htons(port);
                    to.sin_family = AF_INET;
                    int tosize = sizeof to;

                    sprintf(message, "m%d-%d", vertex, i);

                    if(sendto(sock, message, strlen(message), 0, (SOCKADDR *)&to, tosize) < 0)
                    {
                        perror("sendto()");
                        exit(EXIT_FAILURE);
                    }
                }
            }

            else printf("[Optionelle] Cette case est occupée.\n");
        }
    }
}

void PlacementPhase(int id, int i)
{
    if(vertices[i]->owner == NULL)
    {
        if(game.type != GAME_TYPE_PvP_ONLINE)
        {
            setVertexOwner(vertices[i], &game.players[id]);
            setPawnVisibilityState(vertices[i], 1);

            placed[id] = i;

            game.players[id].pawns--;

            game.players[id].activePawns++;

            game.turn++;

            printf("[Joueur] Il a pose un pion au vertice %d\n", i);

            pass = 0;

            if(game.type == GAME_TYPE_PvAI)
            {
                /* L'IA va placer ses pions ici */

                int ai = 1 - id;

                ai_positionment(ai, placed);
            }
        }

        else
        {
            if(id != opponent)
            {
                setVertexOwner(vertices[i], &game.players[id]);
                setPawnVisibilityState(vertices[i], 1);

                placed[id] = i;

                game.players[id].pawns--;

                game.players[id].activePawns++;

                game.turn++;

                printf("[Joueur-LAN] Il a pose un pion au vertice %d\n", i);

                pass = 0;

                char message[8];

                SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
                if(sock == INVALID_SOCKET)
                {
                    perror("socket()");
                    exit(EXIT_FAILURE);
                }

                struct hostent *hostinfo = NULL;

                const char *hostname = HOSTNAME;
                Uint16 port = PORT;

                hostname = getIPAdress(&port);

                hostinfo = gethostbyname(hostname);
                if (hostinfo == NULL)
                {
                    fprintf (stderr, "Unknown host %s.\n", hostname);
                    exit(EXIT_FAILURE);
                }

                SOCKADDR_IN to = { 0 };
                to.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
                to.sin_port = htons(port);
                to.sin_family = AF_INET;
                int tosize = sizeof to;

                sprintf(message, "p%d", i);

                if(sendto(sock, message, strlen(message), 0, (SOCKADDR *)&to, tosize) < 0)
                {
                    perror("sendto()");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    else
    {
        printf("Cette case est deja occupee.\n");

        displayErrorScreen(id);

        timerError = SDL_AddTimer(ERROR_TIME, removeErrorScreen, NULL); // T = 1000ms = 1s
    }
}

void WidgetsManager()
{
    Widget wid;

    SDL_Rect RMS;
    RMS.h = 2; RMS.w = 2; RMS.x = MS.x; RMS.y = MS.y;

    int i = getClickedWidget(screen[menu], &RMS, &wid);

    if(i != -1)
    {
        screen[menu]->widgets[i]->clicked = 0;

        setWidgetColor(screen[menu]->widgets[i], white);
    }

    // Si le joueur fait un clique gauche dans le menu "demarrage"
    if(menu == MENU_START)
    {
        if(i == 1+2) // Jouer
            menu = MENU_SETTINGS;

        else if(i == 2+2) // Ouvre le  fichier de configuration (seulement dans Windows)
        {
            #ifdef WIN32

            HINSTANCE notepad = ShellExecute(NULL, NULL, GAME_CONFIG_FILE, NULL, NULL, SW_SHOWNORMAL);

            printf("[Options] Output : %d", (int)notepad);

            #else

            printf("Vous n'etes pas sur Windows, pour acceder au fichier de configuration, ouvrez directement %s dans le dossier de votre jeu.", GAME_CONFIG_FILE);

            #endif // WIN32
        }

        else if(i == 3+2) quit = SDL_TRUE;
    }

    // Il doit d'abords configurer sa partie
    else if(menu == MENU_SETTINGS)
    {
        switch(i)
        {
            // Les identifiants des widgets commencent de 2 jusqu'à MAX_WIDGETS, d'où le +2.
            case 1+2: // pvp
            {
                if(game.type != GAME_TYPE_PvP)
                {
                    game.type = GAME_TYPE_PvP;

                    setWidgetColor(screen[menu]->widgets[i], green);
                    setWidgetColor(screen[menu]->widgets[i+1], white);
                    setWidgetColor(screen[menu]->widgets[i+2], white);

                    setWidgetColor(screen[menu]->widgets[4+2], grey);
                    setWidgetColor(screen[menu]->widgets[5+2], grey);

                    setWidgetClickable(screen[menu]->widgets[4+2], WIDGET_NOT_CLICKABLE);
                    setWidgetClickable(screen[menu]->widgets[5+2], WIDGET_NOT_CLICKABLE);
                }

                break;
            }

            case 2+2: // pvAI
            {
                if(game.type != GAME_TYPE_PvAI)
                {
                    game.type = GAME_TYPE_PvAI;

                    setWidgetColor(screen[menu]->widgets[i-1], white);
                    setWidgetColor(screen[menu]->widgets[i], green);
                    setWidgetColor(screen[menu]->widgets[i+1], white);

                    setWidgetColor(screen[menu]->widgets[4+2], white);
                    setWidgetColor(screen[menu]->widgets[5+2], white);

                    setWidgetClickable(screen[menu]->widgets[4+2], WIDGET_CLICKABLE);
                    setWidgetClickable(screen[menu]->widgets[5+2], WIDGET_CLICKABLE);
                }
                break;
            }

            case 3+2: // pvp_lan
            {
                if(game.type != GAME_TYPE_PvP_ONLINE)
                {
                    game.type = GAME_TYPE_PvP_ONLINE;

                    setWidgetColor(screen[menu]->widgets[i], green);
                    setWidgetColor(screen[menu]->widgets[i-1], white);
                    setWidgetColor(screen[menu]->widgets[i-2], white);

                    setWidgetColor(screen[menu]->widgets[4+2], grey);
                    setWidgetColor(screen[menu]->widgets[5+2], grey);

                    setWidgetClickable(screen[menu]->widgets[4+2], WIDGET_NOT_CLICKABLE);
                    setWidgetClickable(screen[menu]->widgets[5+2], WIDGET_NOT_CLICKABLE);
                }
                break;
            }

            case 4+2: // facile
            {
                if(game.type == GAME_TYPE_PvAI)
                {
                    game.difficulty = AI_TYPE_EASY_RANDOM;

                    setWidgetColor(screen[menu]->widgets[i], green);
                    setWidgetColor(screen[menu]->widgets[i+1], white);
                }
                break;
            }

            case 5+2: // moyen
            {
                if(game.type == GAME_TYPE_PvAI)
                {
                    game.difficulty = AI_TYPE_MEDIUM;

                    setWidgetColor(screen[menu]->widgets[i-1], white);
                    setWidgetColor(screen[menu]->widgets[i], green);
                }

                break;
            }

            case 6+2: // regle oui
            {
                game.optionnel = 1;

                setWidgetColor(screen[menu]->widgets[6+2], green);
                setWidgetColor(screen[menu]->widgets[7+2], black);
                /*setWidgetVisible(screen[menu]->widgets[i], 0);
                setWidgetVisible(screen[menu]->widgets[i+1], 1);*/
                break;
            }

            case 7+2: // regle non
            {
                game.optionnel = 0;

                setWidgetColor(screen[menu]->widgets[6+2], black);
                setWidgetColor(screen[menu]->widgets[7+2], red);
                /*setWidgetVisible(screen[menu]->widgets[i-1], 1);
                setWidgetVisible(screen[menu]->widgets[i], 0);*/
                break;
            }

            case 8+2: // theme_0
            {
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_0], 1);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_1], 0);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_2], 0);
                break;
            }

            case 9+2: // theme_1
            {
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_0], 0);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_1], 1);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_2], 0);
                break;
            }

            case 10+2: // theme_2
            {
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_0], 0);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_1], 0);
                setWidgetVisible(screen[1]->widgets[BOARD_THEME_2], 1);
                break;
            }

            case 11+2:
            {
                menu = MENU_GAME;

                partyState = INFO_PLACEMENT; /* permet de changer le texte informatif pour
                                                    expliquer la phase de placement */
                // Le décompte commence!
                timer1s = SDL_AddTimer(1000, updateCountdown, &count); // T = 1000ms = 1s

                if(music != NULL)
                    Mix_PlayMusic(music, -1);

                game.active = 1;
                break;
            }

            case 12+2:
            {
                menu = MENU_START;
                break;
            }
        }
    }

    else if(menu == MENU_GAME)
    {
        switch(i)
        {
            // Les identifiants des widgets commencent de 2 jusqu'à MAX_WIDGETS, d'où le +2.
            case 3+2: // Recommencer la partie
            {
                resetGame(&game, game.type);

                partyState = INFO_PLACEMENT;

                timer1s = SDL_AddTimer(1000, updateCountdown, &count); // T = 1000ms = 1s

                if(music != NULL)
                    Mix_PlayMusic(music, -1);

                game.active = 1;
                break;
            }

            case 4+2: // Passer le tour
            {
                if(game.type == GAME_TYPE_PvAI)
                    printf("Il n'est pas possible de passer le tour dans une game contre IA");

                else
                {
                    int id; // id du joueur qui joue actuellement

                    id = convertTurn(game.turn); // [voir le fichier game.h]

                    pass++;

                    game.turn++;

                    for(int k = 0; k < 4; k++)
                    {
                        if(game.players[id].moulinID[k] != -1)
                        {
                            Lines[ game.players[id].moulinID[k] ][3]    =   LINE_USED;

                            game.players[id].moulin--;
                            game.players[id].moulinID[k] = -1;
                        }
                    }

                }

                break;
            }

            case 5+2: // quitter la partie
            {
                game.active = 0;

                resetGame(&game, GAME_TYPE_PvP);


                menu = MENU_START;
                break;
            }

        }
    }
}



void resetGame(Game* game, enum gameType type)
{
    initializeGame(game, type, 1);

    setWidgetColor(screen[2]->widgets[2+2], white);
    setWidgetColor(screen[2]->widgets[3+2], white);

    setWidgetColor(screen[2]->widgets[1+2], green);
    setWidgetColor(screen[2]->widgets[4+2], grey);
    setWidgetColor(screen[2]->widgets[5+2], grey);
    setWidgetColor(screen[2]->widgets[6+2], green);
    setWidgetColor(screen[2]->widgets[7+2], black);
    setWidgetClickable(screen[2]->widgets[4+2], WIDGET_NOT_CLICKABLE);
    setWidgetClickable(screen[2]->widgets[5+2], WIDGET_NOT_CLICKABLE);

    resetTexts();

    for(int i = 0; i < MAX_PLAYERS; i++)
    {
         //game.players[i].pseudo = "";

        game->players[i].pawns = PLAYER_INITIAL_PAWNS;

        game->players[i].activePawns = 0;

        game->players[i].moulin = 0;

        for(int k = 0; k < 4; k++) game->players[i].moulinID[k] = -1;

        focusedVertex[i] = NULL;
    }

    for(int i = 0; i < MAX_VERTICES; i++)
    {
        vertices[i]->owner = NULL;

        setPawnVisibilityState(vertices[i], 0);
    }

    for(int i = 0; i < MAX_LINES; i++)
        Lines[i][3] = LINE_INACTIVE;

    partyState = 0; // phase actuel de la partie (placement, mouvement, saut)
    count = 1; // temps du jeu en seconde
    pass = 0;
    lastTime = 0;
    randomPlayer = -1; // premier joueur à jouer

    SDL_RemoveTimer(timer1s);

    Mix_PauseMusic();

    printf("\tPartie redemarre.\n");
}

void LoadPawns()
{
    for(int i = 0; i < PLAYER_INITIAL_PAWNS; i++)
    {
        for(int j = 0; j < MAX_PLAYERS; j++)
        {
            sprites[i][j].imageFile = PAWN_FILE;
            sprites[i][j].renderer = renderer;

            loadSprite( &(sprites[i][j]) );


            setSpritePosition( &(sprites[i][j]), 50*j + 25, 20*i + 100);
        }

        changeColorSprite( &(sprites[i][0]), blue);
        changeColorSprite( &(sprites[i][1]), red);
    }
}

void DrawWidgets()
{
  if(screen[menu] != NULL)
        for(int i = 2; i < MAX_WIDGETS; i++)
        {
            if(screen[menu]->widgets[i] != NULL)
                drawWidget(screen[menu]->widgets[i]);
        }
}

void DrawConfigTexts()
{
    for(int i = 0; i < 4; i++)
        if(gameConfigText[i] != NULL)
        {
            if(menu == MENU_SETTINGS) setTextVisible(gameConfigText[i], 1 );
            else setTextVisible(gameConfigText[i], 0 );

            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
            drawText(gameConfigText[i]);
        }
}

/***Pour réduire la taille de main.c, quelques fonctions utilisés seulement dans le main()
    ont été transferé vers les fichiers suivantes: */

#include "source/ai.c"
#include "source/moulin.c"

/***On aurait pu réduire encore plus la taille du main.c en transferant le code source des fonctions vers d'autres fichiers.." **/

/************************************************************** FIN *************************************************************/

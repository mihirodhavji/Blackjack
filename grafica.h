#ifndef GRAFICA_H_INCLUDED
#define GRAFICA_H_INCLUDED

#include "library.h"
#include "estruturas.h"
#include "funcoes_d.h"

void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int ,int ,no_jogadores *_players, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(struct no_casa casa, SDL_Surface ** _cards, SDL_Renderer * _renderer, TTF_Font *_font);
void RenderPlayerCards(no_jogadores *players, SDL_Surface **_cards, SDL_Renderer* _renderer);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);


#endif // GRAFICA_H_INCLUDED

#ifndef OUTRAS_H_INCLUDED
#define OUTRAS_H_INCLUDED

#include "library.h"
#include "estruturas.h"
#include "limpeza.h"
#include "funcoes_d.h"

int ea_jogar (no_jogadores *player, int j,node **head_baralho,int nr_baralhos, int nr_cartas);
void add_player(int x, int y, no_jogadores **players, no_jogadores **falidos, int *nr_jogadores);
void mudar_aposta (no_jogadores *players);
void ges_money(no_jogadores *player, struct no_casa *casa);
int start_game (no_jogadores *players, struct no_casa *casa, int nr_baralhos,int nr_jogadores, int nr_cartas, node **head_baralho);


#endif // OUTRAS_H_INCLUDED

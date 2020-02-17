#ifndef FUNCOES_D_H_INCLUDED
#define FUNCOES_D_H_INCLUDED

#include "library.h"
#include "estruturas.h"
node * fazer_baralho(int nr_baralhos);
int cal_pts(stack_cartas *top, int );
node * retirar_carta(node *(*head_baralho), int *nr_cartas);
stack_cartas * push (stack_cartas *top, int c, int n);
stack_cartas * pop (stack_cartas *top, carta *saida);
int verificar_baralho (int nr_cartas,node **head_baralho,int nr_baralhos);
int hit (int nr_cartas, no_jogadores *player, node **head_baralho, int nr_baralhos);


#endif // FUNCOES_D_H_INCLUDED

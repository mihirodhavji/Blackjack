#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

#include "library.h"

///estrutura carta
typedef struct {

    int codigo;
    int naipe;
}carta;

/// stack para as cartas dos jogadores
typedef struct stack_cartas{

    carta card;
    struct stack_cartas *next;
}stack_cartas;

/// estrutura da casa
struct no_casa{

    int pontos;
    int pos_house_hand;
    float dinheiro;
    int status; /// mesma codificação que os jogadores, nao faz nem surrender , nem double,
    stack_cartas *top;
};

/// para fazer o baralho
typedef struct node{

    carta payload;
    struct node *next;

}node;
/**
  * status
  * 0 = ainda não jogou
  * 1 = Hit
  * 2 = Stand
  * 3 = Surrender
  * 4 = Double
  * 5 = BUST
  * 6 = BlackJack
  * 7 = retirado da mesa
  */
typedef struct {

    char nome [STRING_SIZE];
    float dinheiro;
    float aposta;
    int vitoria;
    int empate;
    int derrota;
    int tipo;   /// se Humano(0) ou EA (1)
    int ativo;  /// se 1 esta a jogar, caso contrario 0
    int status;
    int pontos;
    int pos_player_hand; /// numero de cartas que tem
    int posicao;/// posicao na lista de jogadores
    int press_stats; /// se é para escrever no ficheiro ou nao
    stack_cartas *top;

}jogador;


typedef struct no_jogadores{

    jogador payload;
    struct no_jogadores *next;

} no_jogadores;



#endif // ESTRUTURAS_H_INCLUDED

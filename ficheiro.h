#ifndef FICHEIRO_H_INCLUDED
#define FICHEIRO_H_INCLUDED

#include "library.h"
#include "estruturas.h"
no_jogadores * ler_parametros(int *nr_baralhos, int *nr_jogadores, char** );
void escrever_stats(no_jogadores *head,no_jogadores *falidos, struct no_casa casa);
int **ler_jogadas (char** );

#endif // FICHEIRO_H_INCLUDED

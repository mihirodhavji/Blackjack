

#include "limpeza.h"

/**
 * funcao que limpa a lista que tem as cartas do baralho
 * recebe o head da lista
 */
void limpar_baralho (node *head)
{
    node *aux = head;/// aponta para a head
    node *tmp = NULL;
    while (aux != NULL)
    {
        tmp = aux; /// guarda a atual
        aux = aux->next; /// avança
        free(tmp); /// liberta a memoria
    }

}
/**
 * funcao que limpa a lista de jogadores
 * raciocinio semelhante ao limpar baralho
 */
void limpar_jogadores (no_jogadores *head)
{
    no_jogadores *aux = head;
    no_jogadores *tmp = NULL;
    while (aux != NULL)
    {
        tmp = aux;
        aux = aux->next;
        free(tmp);
    }

}


/**
 * funcao que limpa as stacks
 * recebe o top da stack e devolve o novo topo
 */
stack_cartas* limpar_stack(stack_cartas *top)
{
    carta card;
    stack_cartas *aux = NULL;
    stack_cartas *tmp = NULL;

    aux = top; /// aponta para o topo

    while (aux != NULL) /// percorre todas as posicoes
    {
        tmp = pop(aux, &card);
        aux = aux->next;
    }

    free(tmp); /// liberta memoria
    return aux; /// devolve o novo topo
}

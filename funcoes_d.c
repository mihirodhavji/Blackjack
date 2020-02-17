
#include "funcoes_d.h"

/**
 *  funcao que faz o baralho de uma maneira ordenada e que recebe como para de entradas o nr de baralhos
 *  lidos do ficheiro
 *  retorna a head desse baralho
 */

node * fazer_baralho(int nr_baralhos)
{
    node *aux;
    node *new_node;
    node *_head_baralho = NULL;
    int b, n, i;

    /// preencher a lista
    for (b = 0;b < nr_baralhos;b++) /// conta baralhos
        {
            for (n = 0;n < 4;n++)    /// conta naipes
            {
                for (i = 1;i  <= 13 ;i++) /// conta codigo
                {
                    /// alocação de memória
                    new_node = (node *)malloc(sizeof(node));
                    ///preencher a lista
                    new_node->payload.codigo = i;
                    new_node->payload.naipe = n;
                    new_node->next = NULL;

                    if (_head_baralho == NULL ) /// se a lista estiver vazia
                    {
                        _head_baralho = new_node;
                    }
                    else                        /// se ja tiver elementos
                    {
                        aux =_head_baralho; /// o aux aponta para a cabeça
                        while(aux->next != NULL)  /// percorre a lista ate encontrar o ultimo elemento
                        {
                            aux = aux->next;
                        }
                        aux->next = new_node; /// o next do ultimo elemento passa a ser o novo node
                    }
                }
            }
        }
return _head_baralho;
}

/**
 * funcao que retira uma carta de uma posicao aleatoria e retorna
 * recebe o ponteiro ponteiro da head do baralho e o ponteiro do
 * numero de cartas
 */
node * retirar_carta(node *(*head_baralho), int *nr_cartas)
{
    node *aux1;
    node *aux2;
    int i=0, r = 0;


    r = rand() % (*nr_cartas); /// numero aleatorio, posicao da qual temos de retirar a carta
    *nr_cartas -= 1; /// subtrai uma carta

    if (r == 0) /// retirar da posicao 0
    {
        aux1 =  (*head_baralho); /// aponta para a head
        aux2 =  (*head_baralho)->next; /// aponta para 2 posicao
        (*head_baralho) = aux2 ;    /// head aponta para a segunda posicao
        aux1->next = NULL;          /// destroi a ligacao
        return aux1;                /// devolve a carta
        free(aux1);                 /// liberta a memoria
    }
    else /// retirar de outra posicao
    {
        aux1 =  (*head_baralho);        /// aponta para a head
        aux2 =  (*head_baralho)->next;  /// aponta para a posicao 1
        i = 0;
        while (i != (r-1)) /// é r - 1, porque se quisermos tirar a carta da posicao 1, nao temos de andar temos de apenas remover o aux2
        {
            aux1 = aux1->next; /// aux1 e aux2 andam em simultaneo ate encontrarem a posicao
            aux2 = aux2->next;
            i++;
        }
        aux1->next = aux2->next; /// destroi a ligacao aux1->next = aux2, agora o next do aux1 aponta pra o next do aux2
        aux2->next = NULL; /// destroi as ligacoes
        return aux2; /// devolve o aux2
        free(aux2); /// liberta memoria
    }

}

/**
 * funcao que guarda coloca uma carta na stack(no topo)
 * recebe o top da stack em questao, o c é o codigo da carta e o n é o naipe
 */
stack_cartas * push (stack_cartas *top, int c, int n)
{
    stack_cartas *new_node = NULL;
    /// alocação de memoria
    new_node = (stack_cartas *)malloc(sizeof(stack_cartas));

    if (new_node == NULL)
    {///verificao do malloc
        printf("malloc stack falhou");
        exit(EXIT_FAILURE);
    }
    new_node->card.codigo = c; /// guardar o codigo
    new_node->card.naipe = n; /// guardar o naipe
    new_node->next = top; /// o next aponta para o top
    return new_node; /// retorna o novo node
}

/**
 * funcao que tira e retorna uma carta da stack
 * recebe o top da stack de cartas em questao, e o ponteiro para uma carta
 *
 */
stack_cartas * pop (stack_cartas *top, carta *saida)
{
    stack_cartas *tmp = NULL;

    if ( top == NULL) /// caso a stack esteja vazia
        return NULL;

    else
    {
    tmp = top; /// guarda o topo
    saida->codigo = top->card.codigo; /// passa o data
    saida->naipe = top->card.naipe;
    top = top->next; /// o top aponta para o prox
    free(tmp); /// liberta memoria
    return top; /// devolve o topo
    }
}

/**
 * funcao que calcula os pontos, recebe o topo da stack, e o numero de cartas que tem na mao
 * devolve os pontos calculados
 */
int cal_pts(stack_cartas *top, int a)
{
    stack_cartas *aux = NULL;
    aux = top;
    int soma = 0, b = 0, i = 0;
    int as = 0;

    if (a == 1)
    {
        aux = aux->next;    /// quando tem apenas uma carta (caso da casa), aponto logo para a carta seguinte, e calcula os pontos dessa carta
    }                       /// este if nao vai influenciar o funcionamento do resto do codigo para os jogadores
                            /// visto que esta funcao só é chamada depois deles terem duas cartas
        while (i < a)
            {
                b = aux->card.codigo;

                    /// dar uma pontuação a cada carta

                    if (b >= 1 && b <= 8) /// carta de 2 a 9
                        b += 1;

                    else if (b >= 9 && b <= 12) /// carta dez,valete,rainha ou rei
                        b = 10;

                    else if (b == 13)/// ás
                    {
                        b = 1;
                        as++;
                    }
                soma += b;
                aux = aux->next;
                i++;
            }
            /// caso haja um as na mao e a soma (com o as a valer 1 ponto) seja inferior ou igual a 11 este soma 10
            if (soma <= 11 && as > 0)
                soma +=10;
    return soma;
}

/**
 * funcao que verifica se o baralho acabou ou nao
 * recebe o nr de cartas,
 * o ponteiro ponteiro da head do baralho
 * e o nr de baralhos
 */

int verificar_baralho (int nr_cartas,node **head_baralho,int nr_baralhos)
{
    if (nr_cartas == 0) /// se for 0 cria um novo baralho, com o mesmo numero de carats que o inicial
    {
        nr_cartas = nr_baralhos * MAX_DECK_SIZE;
        *head_baralho = fazer_baralho(nr_baralhos);
    }
    return nr_cartas; /// devolve o nr de cartas
}


/**
 * funcao que tira uma carta do baralho
 * recebe o nr cartas, o elemento da lista de jogadores em questao,
 * o ponteiro ponteiro da head do baralho e o nr de baralhos
 * devolve o nr de cartas
 */

int hit (int nr_cartas, no_jogadores *player, node **head_baralho,int nr_baralhos)
{
    node *carta_saida;
    no_jogadores *aux_players;

    aux_players = player; /// aponta para o elemento em questao

    /// tira uma carta do baralho
    carta_saida = retirar_carta(&(*head_baralho),&nr_cartas);
    /// coloca essa carta na stack do jogador
    aux_players->payload.top = push(aux_players->payload.top , carta_saida->payload.codigo , carta_saida->payload.naipe);
    /// aumenta o nr de cartas que tem na mao
    aux_players->payload.pos_player_hand ++;
    ///atuliza o status
    aux_players->payload.status = 1;
    /// calcula os pontos depois de receber a carta
    aux_players->payload.pontos = cal_pts(aux_players->payload.top, aux_players->payload.pos_player_hand);

    return nr_cartas; /// devolve o nr de cartas
}


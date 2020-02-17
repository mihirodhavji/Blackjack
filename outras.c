
#include "outras.h"

/**
 * funcao que muda a aposta de um jogador, apenas quando pressiona a tecla B
 * recebe o topo da lista de jogadores
 */
void mudar_aposta(no_jogadores *players)
{
    no_jogadores *aux = NULL;
    char nome[MAX_NAME_SIZE], n_aux [MAX_NAME_SIZE];
    int a = 0;
    float aux_money = 0;

    /// aponta para o topo
    aux = players;
    /// ler o nome do teclado
    printf("Escreva o nome do jogador\n");
    fflush(stdout);
    fgets(n_aux,MAX_NAME_SIZE,stdin);
    a = sscanf(n_aux, "%s", nome);

    if (a != 1)
    { /// verificao so sscanf
        printf("sscanf falhou\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

        while (aux != NULL)
        {
            if (strcmp(nome,aux->payload.nome) == 0) /// compara a string lida do teclado com o nome dos jogadores
            {
                /// se for igual

                printf("Escreva o valor da aposta\n");  /// ler a aposta do teclado
                fflush(stdout);
                fgets(n_aux,MAX_NAME_SIZE,stdin);
                a = sscanf(n_aux, "%f", &(aux_money));
                if (a != 1)
                {
                    printf("sscanf falhou");
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }

                if  (aux_money <= aux->payload.dinheiro && aux_money > 0) /// o valor lido tem de ser positivo e menor que a quantidade de dinheiro do jogador
                    aux->payload.aposta = aux_money;

                else
                {
                    printf("Valor inválido!\n");
                    fflush(stdout);
                }

                return;
            }
            else /// se o nome lido do teclado for diferente... passa para o proximo
                aux = aux->next;
        }
        printf("nome invalido\n"); /// se nao houver nenhum igual
        fflush(stdout);
        return;


}

/**
 * funcao que efetua a jogada do ea
 * recebe o jogador EA em questao, um int que é a jogada (proveniente da matriz)
 * o ponteiro ponteiro da head do baralho, o nr de baralhos e o nr de cartas
 * devolve o nr de cartas
 */
int ea_jogar (no_jogadores *player, int jogada, node **head_baralho,int nr_baralhos, int nr_cartas)
{
    no_jogadores *aux = NULL;
    aux = player;
    /// o a numeracao das jogadas esta de acordo com o bloco de comentarios em cima da estrutura jogador
    if(jogada == 1)
    {
        ///hit
        nr_cartas = hit (nr_cartas, aux, &(*head_baralho), nr_baralhos);
        nr_cartas = verificar_baralho (nr_cartas,&(*head_baralho), nr_baralhos);
        aux->payload.status = 1;
        if (aux->payload.pontos > 21)
            aux->payload.status = 5; /// fez  BUST
    }
    else if(jogada == 2)
    {
        ///stand
        aux->payload.status = 2; /// atulizar o status
    }
    else if(jogada == 3)
    {
        ///surrender
        if (aux->payload.status == 0)
        {
            aux->payload.status = 3; /// atulizar o status
        }
        else
        {
            /// se nao puder fazer surrender devido ao nº de cartas da sua mao, faz hit
            nr_cartas = hit (nr_cartas, aux, &(*head_baralho), nr_baralhos);
            nr_cartas = verificar_baralho (nr_cartas,&(*head_baralho), nr_baralhos);
            aux->payload.status = 1;
            if (aux->payload.pontos > 21)
                aux->payload.status = 5; /// verificar  BUST
        }

    }
    else if(jogada == 4)
    {
        ///double
        if (aux->payload.status == 0)
        {
            nr_cartas = hit (nr_cartas, aux, &(*head_baralho), nr_baralhos);
            nr_cartas = verificar_baralho (nr_cartas,&(*head_baralho), nr_baralhos);
            aux->payload.dinheiro -= aux->payload.aposta; /// retira logo o dinheiro
            aux->payload.status = 4; /// atulizar o status
        }
        else
        {
            if (aux->payload.pontos == 18) /// caso especial em nao pode fazer double e tem 18 pontos
            {
                ///stand
                aux->payload.status = 2; /// atulizar o status
            }
            else
            {
                /// se nao puder fazer double devido ao nº de cartas da sua mao, faz hit
                nr_cartas = hit (nr_cartas, aux, &(*head_baralho), nr_baralhos);
                nr_cartas = verificar_baralho (nr_cartas,&(*head_baralho), nr_baralhos);
                aux->payload.status = 1;
                if (aux->payload.pontos > 21)
                    aux->payload.status = 5; /// fez  BUST
            }
        }
    }

    return nr_cartas; /// devolve o nr de cartas

}

/**
 * funcao que adiciona um jogador
 * recebe as coordenadas(x,y) do clique do rato
 * o ponteiro ponteiro para as duas listas de jogadores
 * o ponteiro para o nr de jogadores
 */
void add_player(int x, int y, no_jogadores **players, no_jogadores **falidos, int *nr_jogadores)
{
    no_jogadores *aux_players = NULL;
    aux_players = (*players);
    char aux[MAX_NAME_SIZE];
    char hu[] = "HU", ea[] = "EA";
    int square_press = 0, s = 0, i = 0;
    no_jogadores *new_node = NULL;
    no_jogadores *aux1 = NULL;
    no_jogadores *aux2 = NULL;
    no_jogadores *aux_falidos = NULL;

    if (y > 290 && y < 505 ) /// 1º lugar avalia-se o Y, visto que é igual para todos os quadrados, se estiver entre 290 e 505 , é válido
    {
        if (x > 10 && x < 218 )        /// se foi no 1º quadrado
            {
                square_press = 0;
            }

        else if (x > 218 && x < 425 )   /// se foi no 2º quadrado
            {
                square_press = 1;
            }

        else if (x > 225 && x < 634 )   /// se foi no 3º quadrado
            {
                square_press = 2;
            }

        else if (x > 634 && x < 842)   /// se foi no 4º quadrado
            {
                square_press = 3;
            }

        else                    /// se nao foi em nenhum dos quadrados
        {
            printf("Local inválido!\n");
            fflush(stdout);
            return;
        }
    }
    else  /// posicao de Y incorreta
    {
        printf("Local inválido!\n");
        fflush(stdout);
        return;
    }
    aux_players = (*players);

    while (aux_players != NULL) /// verificar se essa posicao ja se encontra ocupada ou nao
    {                           /// para tal tem que estar la um jogador ativo
        if (aux_players->payload.ativo == 1 && square_press == aux_players->payload.posicao)
        {
            printf("Posicao Ocupada!\n");
            fflush(stdout);
            return;
        }
        aux_players = aux_players->next; /// passa para o proximo
    }

    printf("Posicao Livre!\n");
    fflush(stdout);

    /// alocacao de memoria
    new_node = (no_jogadores *)malloc(sizeof(no_jogadores));

    if (new_node == NULL)
        {   /// verificação da alocação de memória
            printf("malloc jogadores falhou");
            exit(EXIT_FAILURE);
        }

    /// leitura do tipo
    printf("Introduza o tipo\n");
    fflush(stdout);
    fgets(aux,MAX_NAME_SIZE,stdin);
    aux[strlen(aux)-1] = 0; /// tirar o \n

    if (strcmp(aux,hu) == 0)
        new_node->payload.tipo = 0;
    else if (strcmp(aux, ea) == 0)
        new_node->payload.tipo = 1;
    else
        {   /// se nao for nenhum dos dois
            printf("Tipo inválido!\n");
            fflush(stdout);
            free(new_node);
            return;
        }

    /// leitura do nome
    printf("Introduza o nome\n");
    fflush(stdout);
    fgets(aux,MAX_NAME_SIZE,stdin);
    aux[strlen(aux)-1] = 0;
    strcpy(new_node->payload.nome, aux);

    if ((strlen(new_node->payload.nome)) > 8) ///verificao de parametros
    {
        printf("Parâmetros inválidos\nMáximo 8 caracteres\n");
        fflush(stdout);
        free(new_node);
        return;
    }

    /// leitura do dinheiro
    printf("Introduza o dinheiro\n");
    fflush(stdout);
    fgets(aux,MAX_NAME_SIZE,stdin);
    aux[strlen(aux)-1] = 0;
    s = sscanf(aux, "%f", &(new_node->payload.dinheiro));

    if (s != 1)
    {   /// verificação do sscanf
        printf("erro sscanf");
        exit(EXIT_FAILURE);
    }

    ///verificao de parametros
    if (new_node->payload.dinheiro > 500 || new_node->payload.dinheiro < 10)
    {
        printf("Parâmetros inválidos\nDinheiro Inicial entre 10 e 500\n");
        fflush(stdout);
        free(new_node);
        return;
    }

    s = 0; /// para um futuro sscanf

    /// leitura da aposta
    printf("Introduza a aposta\n");
    fflush(stdout);
    fgets(aux,MAX_NAME_SIZE,stdin);
    aux[strlen(aux)-1] = 0;
    s = sscanf(aux, "%f", &(new_node->payload.aposta));

    if (s != 1)
    {   /// verificação do sscanf
        printf("erro sscanf");
        exit(EXIT_FAILURE);
    }

    ///verificao de parametros
    if (new_node->payload.aposta < 2 || new_node->payload.aposta > (0.25 * new_node->payload.dinheiro))
    {
        printf("Parâmetros inválidos\nAposta inicial não deve ser superior a 25%% do dinheiro inicial nem inferior a 2 euros \n");
        fflush(stdout);
        free(new_node);
        return;
    }
    /// inicializar outros parametros tal como na funcao ler parametros
    new_node->payload.empate = 0;
    new_node->payload.derrota = 0;
    new_node->payload.vitoria = 0;
    new_node->payload.pontos = 0;
    new_node->payload.ativo = 1; /// 1 significa que esta a jogar
    new_node->payload.top = NULL;
    new_node->payload.status = 0; /// ainda nao jogou
    new_node->payload.pos_player_hand = 0;
    new_node->payload.press_stats = 1; /// permissao para imprimir
    new_node->payload.posicao = square_press;

    /// retirar o jogador da lista dos jogadores que ocupa a posicao igual ao square_press
    /// essa lista esta feita de tal maneira que tera sempre 5 elementos 4 players (ativos ou nao)
    /// e o dummy node

    if (square_press == 0)  /// se o jogador estiver na 1 posicao
    {
        /// aux1 guarda a 1 posicao
        aux1 = (*players);
        /// a head avança
        (*players) = (*players)->next;
        /// a aux1 posicao deixa de aponta para a aux2 segunda posicao e aponta para NULL
        aux1->next = NULL;

        /// se a lista dos falidos estiver vazia

        if ((*falidos) == NULL)
        {
            (*falidos) = aux1; /// sera o 1º elemento
        }
        else
        {
            /// se nao estiver vazia
            aux_falidos = (*falidos);
            /// procura a ultima posicao
            while (aux_falidos->next != NULL)
            {
                aux_falidos = aux_falidos->next;
            }
            /// a ultima posicao aponta para o aux 1(agora a ultima posicao), e este ja apontava para NULL
            aux_falidos->next = aux1;
        }
    }
    else                    /// se o jogador estiver noutra posicao
    {
        aux1 = (*players);
        aux2 = (*players)->next;
        i = 0;
        while (i != (square_press-1)) /// raciocinio igual ao retirar carta
        {
            aux1 = aux1->next;
            aux2 = aux2->next;
            i++;
        }
        aux1->next = aux2->next;
        aux2->next= NULL;

        if ((*falidos) == NULL) /// caso a lista esteja vazia
        {
            (*falidos) = aux2;
        }
        else
        {
            /// se nao estiver vazia
            aux_falidos = (*falidos);
            /// procura a ultima posicao
            while (aux_falidos->next != NULL)
            {
                aux_falidos = aux_falidos->next;
            }
            /// a ultima posicao aponta para o aux 1(agora a ultima posicao), e este ja apontava para NULL
            aux_falidos->next = aux2;
        }
    }

    /// adicionar o new_node na posicao de valor igual a square_press

    if (square_press == 0) /// acrescenta no topo da lista
    {
        new_node->next = (*players);
        (*players) = new_node;
    }
    else
    {
        aux1 = *players;
        aux2 = (*players)->next;
        i = 0;
        while (i != square_press-1) /// raciocinio igual ao retirar carta
        {
            aux1 = aux1->next;
            aux2 = aux2->next;
            i++;
        }

        aux1->next = new_node;
        new_node->next = aux2;
    }
    *nr_jogadores += 1;

return;
}


/**
 * funcao que gere o dinheiro, conta as estatisticas
 * recebe como parametros apenas um jogador (elemento da lista de jogadores), e um ponteiro para a casa
 */

void ges_money(no_jogadores *player, struct no_casa *casa)
{
    no_jogadores *aux;
    aux = player;
    float aposta = 0;


        if (aux->payload.status == 4) /// se tiver feito Double, a aposta é multiplicada por 2
        {
            aposta = 2*aux->payload.aposta;
            aux->payload.status = 2; /// o status muda para stand

            if (aux->payload.pontos > 21) /// verficacao de BUST
                aux->payload.status = 5;
        }
        else /// caso nao tenha feito Double, a aposta não é alterada
        {
            aposta = aux->payload.aposta;
        }

        if (aux->payload.status == 5 )                                  /// se ocorrer bust perde o dinheiro
        {
            /// o valor da aposta ja foi retirado anteriormente
            aux->payload.derrota += 1;
            casa->dinheiro += aposta;
        }
        else if (aux->payload.status == 3)  /// player fez surrender
        {
            aux->payload.dinheiro += (aposta / 2); /// recebe metade da aposta
            aux->payload.derrota += 1; /// conta como derrota
            casa->dinheiro += (aposta / 2); /// a casa recebe outra metade da aposta
        }

        else if (aux->payload.status == 6 && casa->status == 6) /// casa e player ambos com BlackJack
        {
            aux->payload.dinheiro += aposta; /// recebe de volta o dinheiro inicialmente retirado
            aux->payload.empate += 1;
        }

        else if (aux->payload.status == 6 && casa->status != 6)  /// se o jogador tiver BlackJack e a casa não tiver
        {
            aux->payload.dinheiro += 1.5*(aposta) + aposta;
            aux->payload.vitoria += 1;
            casa->dinheiro -= (1.5*aposta);
        }

        else if (aux->payload.status != 6 && casa->status == 6)   /// caso a casa tenha BlackJack o jogador não tenha
        {
            /// perde o dinheiro, o valor da aposta ja foi retirado anteriormente
            aux->payload.derrota += 1;
            casa->dinheiro += aposta;
        }

        else if (aux->payload.pontos <= 21 && aux->payload.status == 2 && casa->status == 5)       /// caso se a casa fizer bust e o jogador não fizer
        {
            aux->payload.dinheiro += 2*(aposta);
            aux->payload.vitoria += 1;
            casa->dinheiro -= aposta;
        }

        else if (aux->payload.pontos == casa->pontos && aux->payload.status == 2 && casa->status == 2)      /// ambas as partes com os mesmos pontos
        {
            aux->payload.dinheiro += aposta;                                               /// recebe de volta o dinheiro inicialmente retirado
            aux->payload.empate += 1;
        }

        else if (aux->payload.pontos > casa->pontos  && aux->payload.status == 2 && casa->status == 2)       /// se o jogador tiver mais pontos que a casa
        {
            aux->payload.dinheiro += 2*(aposta); /// volta a receber o dinheiro apostado + um valor igual ao da aposta dado pela casa
            aux->payload.vitoria += 1;
            casa->dinheiro -= aposta;
        }

        else if (aux->payload.pontos < casa->pontos   && aux->payload.status == 2 && casa->status == 2)     /// se o jogador tiver menos pontos que a casa
        {
            /// perde o dinheiro, o valor da aposta ja foi retirado anteriormente
            aux->payload.derrota += 1;
            casa->dinheiro += aposta;
        }
}



/**
 * funcao que inicia uma ronda, devolve o nr cartas
 * recebe a lista de jogadores, um ponteiro para casa
 * o numero de baralhos, o nr de jogadores, o nr de cartas
 * e o ponteiro ponteiro da head do baralho
 */

int start_game (no_jogadores *players, struct no_casa *casa, int nr_baralhos,int nr_jogadores, int nr_cartas, node **head_baralho)
{
    no_jogadores *aux_players = NULL;
    node *carta_saida = NULL;
    aux_players = players;
    int i = 0;


    for (i = 0; i < 2; i++) /// dar as cartas
    {
        aux_players = players; /// aponta para a head

        while (aux_players != NULL)
        {   /// dar as cartas aos jogadores
            if (aux_players->payload.ativo == 1)
            {
                /// retirar uma carta ao calhas da lista do baralho
                carta_saida = retirar_carta(&(*head_baralho),&nr_cartas);
                /// verificar final de baralho
                nr_cartas = verificar_baralho (nr_cartas,&(*head_baralho), nr_baralhos);
                /// passar a carta para a stack do player
                aux_players->payload.top = push(aux_players->payload.top , carta_saida->payload.codigo , carta_saida->payload.naipe);
                /// aumentar o pos_player_hand para as cartas aparecerem na janela
                aux_players->payload.pos_player_hand ++;
                /// passar ao player seguinte
                aux_players = aux_players->next;

            }
            else
            {
                aux_players = aux_players->next; /// se o jogador nao for ativo passa para o seguinte
            }
        }
        /// dar as cartas a casa, processo semelhante ao processo dos jogadores
        carta_saida = retirar_carta(&(*head_baralho),&nr_cartas);
        casa->top = push(casa->top, carta_saida->payload.codigo, carta_saida->payload.naipe);
    }

    /// casa ainda não jogou
    casa->status = 0;
    /// a casa mostra apenas 1 carta
    casa->pos_house_hand = 1;
    /// calcular os pontos da casa
    casa->pontos = cal_pts(casa->top, casa->pos_house_hand);
    /// aux_players aponta novamente para o topo, para entrar no prox ciclo "for"
    aux_players = players;

    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (aux_players->payload.ativo == 1) /// apenas para os ativos
        {
            /// calcular os pontos
            aux_players->payload.pontos = cal_pts(aux_players->payload.top, aux_players->payload.pos_player_hand);
            /// descontar o valor do dinheiro por ser inicio de ronda
            aux_players->payload.dinheiro -= aux_players->payload.aposta;
        }

        /// passar para o player seguinte
        aux_players = aux_players->next;
    }
    return nr_cartas; /// devolve o nr de cartas
}

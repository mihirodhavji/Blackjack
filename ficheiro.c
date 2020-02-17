
#include "ficheiro.h"

/**
 * funcao que parametros do ficheiro de configurao
 * recebe o ponteiro para o nr de baralhos, e para o nr de jogadores
 * recebe os nomes dos ficheiros
 */

no_jogadores * ler_parametros(int *nr_baralhos, int *nr_jogadores, char** argv )
{
    FILE *f1 = NULL;
    char aux [STRING_SIZE], a[] = "-", hu[] = "HU", ea[] = "EA";
    char *token;
    int s = 0, i = 0;
    no_jogadores *new_node = NULL;
    no_jogadores *aux2 = NULL;
    no_jogadores *head = NULL;

    /// abertura do ficheiro
    f1 = fopen(argv[1], "r"); /// é o primeiro ficheiro
    if (f1  == NULL)
    {   /// verificação do ficheiro
        printf("Erro na abertura do ficheiro");
        exit(EXIT_FAILURE);
    }

    /// passar o conteudo para uma string auxiliar
	fgets(aux,STRING_SIZE,f1);
    aux [strlen(aux)-1] = 0; /** tirar o \n */

	token = strtok (aux, a); /**  "partir" a string ate ao -*/
	s = sscanf (token,"%d", &(*nr_baralhos));
	if (s != 1)
    {   /// verificação do sscanf
        printf("erro sscanf");
        exit(EXIT_FAILURE);
    }

	s = 0; /** colocar o s = 0 para um proximo sscanf */

	token = strtok (NULL, a); /** comeca a ler a partir do ultimo ponto, logo no 1 argumento temos NULL */
	s = sscanf (token,"%d", &(*nr_jogadores));
	if (s != 1)
    {   /// verificação do sscanf
        printf("erro sscanf");
        exit(EXIT_FAILURE);
    }

    /// verificao de parametros

	if (*nr_baralhos > 8 || *nr_baralhos < 4 || *nr_jogadores > 4 || *nr_jogadores < 1)
    {
        printf("parametros inválidos(nº de jogadores ou nº de baralhos)");
        exit(EXIT_FAILURE);
    }

    /// ler parametros dos jogadores

    for (i = 0; i < *nr_jogadores ; i++)
    {
        /// alocação de memória
        new_node = (no_jogadores *)malloc(sizeof(no_jogadores));

        if (new_node == NULL)
        {   /// verificação da alocação de memória
            printf("malloc jogadores falhou");
            exit(EXIT_FAILURE);
        }

        /// leitura do tipo

        fgets(aux,STRING_SIZE,f1);
        aux [strlen(aux)-1] = 0; /** tirar o \n */
        token = strtok (aux, a); /**  "partir" a string ate ao -*/

        if (strcmp(aux,hu) == 0)
            new_node->payload.tipo = 0;
        else if (strcmp(aux, ea) == 0)
            new_node->payload.tipo = 1;
        else
        {
            printf("parametros errados!\n");
            exit(EXIT_FAILURE);
        }

        /// leitura do nome

        s = 0; /** colocar o s = 0 para um proximo sscanf */

        token = strtok (NULL, a);
        s = sscanf (token,"%s", new_node->payload.nome);

        if (s != 1)
        {   /// verificação do sscanf
            printf("erro sscanf");
            exit(EXIT_FAILURE);
        }

        if ((strlen(new_node->payload.nome)) > 8)
        {
            printf("Parâmetros inválidos\nMáximo 8 caracteres\n");
            exit(EXIT_FAILURE);
        }
        /// leitura do dinheiro inicial

        s = 0;

        token = strtok (NULL, a);
        s = sscanf (token,"%f", &(new_node->payload.dinheiro));
        if (s != 1)
        {   /// verificação do sscanf
            printf("erro sscanf");
            exit(EXIT_FAILURE);
        }

        /// verificacao dos parametros do dinheiro inicial

        if (new_node->payload.dinheiro > 500 || new_node->payload.dinheiro < 10)
        {
            printf("Parâmetros inválidos\nDinheiro Inicial entre 10 e 500\n");
            exit(EXIT_FAILURE);
        }

        /// leitura da aposta inicial

        s = 0;

        token = strtok (NULL, a);
        s = sscanf (token,"%f", &(new_node->payload.aposta));

        if (s != 1)
        {   /// verificação do sscanf
            printf("erro sscanf");
            exit(EXIT_FAILURE);
        }

        ///  verificao dos parametros da aposta inicial

        if (new_node->payload.aposta < 2 || new_node->payload.aposta > (0.25 * new_node->payload.dinheiro))
        {
            printf("Parâmetros inválidos\nAposta inicial não deve ser superior a 25%% do dinheiro inicial nem inferior a 2 euros \n");
            exit(EXIT_FAILURE);
        }
        /// inicializar outros parametros

        new_node->payload.empate = 0;
        new_node->payload.derrota = 0;
        new_node->payload.vitoria = 0;
        new_node->payload.ativo = 1; /// 1 significa que esta a jogar
        new_node->payload.top = NULL;
        new_node->payload.status = 0; /// ainda nao jogou
        new_node->payload.pos_player_hand = 0;
        new_node->payload.press_stats = 1; /// tem permissao para aparecer no ficheiro final
        new_node->payload.posicao = i; /// ocupa a posicao na lista igaul a i
        if (head == NULL) /// caso seja o 1 elemento da lista
            head = new_node;
        else
        {
            aux2 = head;
            while (aux2->next != NULL) ///procurar do ultimo elemento da lista
            {
                aux2 = aux2->next;
            }
            aux2->next = new_node; /// colocar no fim da lista
        }

    }

    while (i != (MAX_PLAYERS + 1)) /// mais 1 para criar o dummy node
    {
         /// alocação de memória
        new_node = (no_jogadores *)malloc(sizeof(no_jogadores));

        new_node->payload.ativo = 0; /// nao e ativo
        new_node->payload.press_stats = 0; /// nao tem permissao para aparecer no ficheiro final
        new_node->payload.posicao = i; /// ocupa a posicao na lista igual a i
        new_node->next = NULL;

        aux2 = head;
        while (aux2->next != NULL) /// ir a procura do ultimo elemento da lista
        {
            aux2 = aux2->next;
        }
        aux2->next = new_node; /// colocar no fim da lista

        i++;
    }


    fclose(f1); /**  fechar o ficheiro*/
    return head;
}

/**
 * funcao que escreve as estatisticas de cada jogador
 * recebe a head para a lista de jogadores que aparecem na tela, que nao aparecem na tela, e a casa
 */
void escrever_stats(no_jogadores *head,no_jogadores *falidos, struct no_casa casa)
{
    FILE *out;
    no_jogadores *aux = NULL;
    no_jogadores *aux_falidos = NULL;
    char type[2];
    out = fopen("stats.txt", "w");

    aux = head; /// aponta para a head

    while(aux!= NULL)
    {

        if (aux->payload.press_stats == 1) /// se tiver permissao para aparecer no ficheiro
        {
            if (aux->payload.tipo == 0) /// saber se é EA ou HU
                sprintf(type,"HU");
            else
                sprintf(type,"EA");

            fprintf(out,"%s- %s - %2dV - %2dE - %2dD - %.1f euros\n", aux->payload.nome, type, aux->payload.vitoria, aux->payload.empate, aux->payload.derrota,aux->payload.dinheiro);
        }
        aux = aux->next; /// passa para o prox
    }

    aux_falidos = falidos; /// aponta para a outra lista, raciocinio analogo com o anterior

    while(aux_falidos != NULL)
    {
        if (aux_falidos->payload.press_stats == 1)
        {
            if (aux_falidos->payload.tipo == 0)
                sprintf(type,"HU");
            else
                sprintf(type,"EA");

            fprintf(out,"%s- %s - %2dV - %2dE - %2dD - %.1f euros\n", aux_falidos->payload.nome, type, aux_falidos->payload.vitoria, aux_falidos->payload.empate, aux_falidos->payload.derrota,aux_falidos->payload.dinheiro);
            aux_falidos = aux_falidos->next;
        }
        else
            aux_falidos = aux_falidos->next;
    }
    /// imprimir o dinheiro final da casa
    fprintf(out,"Balanço Final da casa : %.1f euros", casa.dinheiro);
    fclose(out);
}

/**
 * funcao que a matriz de jogadas do EA de um ficheiro
 * devolve uma matriz 2D (com ambas as estrategias hard e soft)
 */

int ** ler_jogadas (char** argv)
{
    FILE *in;
    int **j = NULL;
    char a[] = "-";
    char *token;
    int  i = 0, n = 0, s = 0;
    char aux[STRING_SIZE];

    ///abertura do ficheiro
    in = fopen(argv[2], "r");

    if (in  == NULL)
    {   /// verificação do ficheiro
        printf("Erro na abertura do ficheiro ea");
        exit(EXIT_FAILURE);
    }


    /// alocacao de memoria
    j = (int **)malloc(H_LINHAS * sizeof(int *));
     if (j == NULL)
        {   /// verificação da alocação de memória
            printf("malloc ea falhou");
            exit(EXIT_FAILURE);
        }
    for(i = 0;i < H_LINHAS;i++)
    {
        /// alocacao de memoria
        j[i] = (int *)malloc(COLUNAS * sizeof(int));
        if (j[i] == NULL)
        {   /// verificação da alocação de memória
            printf("malloc ea falhou");
            exit(EXIT_FAILURE);
        }
    }

    /// leitura da matriz
    for (n = 0; n < H_LINHAS; n++)
    {
        /// le uma linha inteira
        fgets(aux,STRING_SIZE,in);
        aux [strlen(aux)-1] = 0; /// tirar o \n
        for (i = 0; i < COLUNAS; i++)
        {
            if (i == 0 ) /// se for a 1ª entrada
            {
                /// raciocinio semelhante ao do ler parametros
                token = strtok (aux, a);
                s = sscanf(token, "%d", &j[n][i]);
                if (s != 1)
                {
                    printf("sscanf ea falhou");
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                token = strtok (NULL, a);
                s = sscanf(token, "%d", &j[n][i]);
                if (s != 1)
                {
                    printf("sscanf ea falhou");
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }
            }
            s = 0; /// para um futuro sscanf
        }
    }
    fclose(in);/// fechar o ficheiro
    return j;
}

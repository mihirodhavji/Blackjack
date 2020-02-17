
#include "library.h"
#include "estruturas.h"
#include "ficheiro.h"
#include "funcoes_d.h"
#include "grafica.h"
#include "limpeza.h"
#include "outras.h"

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* argv[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int delay_ea = 1000;
    int quit = 0;
    int nr_baralhos = 0, nr_jogadores = 0;
    int nr_cartas = 0;
    node *head_baralho = NULL;
    no_jogadores *players = NULL;
    no_jogadores *aux_players = NULL;
    no_jogadores *players_falidos = NULL;
    node *carta_saida = NULL;
    stack_cartas *aux_conta_as = NULL;
    int i = 0; /// auxiliar
    int jogada = 0, as = 0 ,carrega = 0;
    int **j = NULL;
    struct no_casa casa;
    int posx = 0, posy = 0, x = 0, y = 0;
    srand(time(NULL));



    /// funcao que os parametros do ficheiro
    players = ler_parametros(&nr_baralhos, &nr_jogadores, argv);
    /// calcular o numero de cartas total do baralho
    nr_cartas = (nr_baralhos*MAX_DECK_SIZE);
    /// funcao que faz o baralho todo ordenado
    head_baralho = fazer_baralho(nr_baralhos);

    /// ler o ficheiro de configuracao da EA
    j = ler_jogadas(argv);

    /// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);

    /// loads the cards images
    LoadCards(cards);
    /// Iniciar o jogo
    nr_cartas = start_game (players, &casa, nr_baralhos,nr_jogadores, nr_cartas, &(head_baralho));
    /// casa começa com 0 euros
    casa.dinheiro = 0;
    /// aux_players aponta novamente para o topo, para entrar no ciclo "while"
    aux_players = players;

    i = 0;
 	while( quit == 0 )
    {

            if (i > MAX_PLAYERS) /// para evitar que aux_players aponte para NULL
                aux_players = players;

            if (aux_players->payload.ativo == 0 ) /// se o jogador estiver inativo
            {
                aux_players = aux_players->next;
                i+=1;
            }

            if (aux_players != NULL && aux_players->payload.pontos == 21 && aux_players->payload.pos_player_hand == 2)
            {   /// caso haja BJ
                aux_players->payload.status = 6;
                i++;
                aux_players = aux_players->next;
            }

            if (aux_players != NULL ) /// jogada da EA
            {
                if(aux_players->payload.tipo == 1) /// se for do tipo EA
                {
                    while(aux_players->payload.status == 0 || aux_players->payload.status == 1 ) /// enquanto for diferente de nao jogar e hit
                    {
                        as = 0;/// variavel que conta ases
                        /// comeca a percorrer a stack de cartas pelo topo
                        aux_conta_as = aux_players->payload.top; /// aponta para o topo da stack

                        while (aux_conta_as != NULL)
                        {
                            if (aux_conta_as->card.codigo ==  13)/// se o codigo da carta for 13 é as, aumenta a variavel
                                as++;
                            aux_conta_as = aux_conta_as->next; /// passa para carta seguinte
                        }
                        SDL_Delay(delay_ea); /// provoca o delay

                        /// se o as é 0, mao hard , logo utilizamos a parte de cima da matriz parte de cima da matriz
                        /// a linha 0 da matriz corresponde a ter 4 quatros, logo subtrai-se 4
                        /// a coluna 0 da matriz corresponde à casa ter 2 pontos, portanto subtrai-se 2
                        if  (as == 0)
                            jogada = j [aux_players->payload.pontos - 4][casa.pontos - 2];

                        /// caso seja uma jogada soft, pontuacao mais baixa com soft é 12
                        /// a estrategia para a hard e soft estam na mesma matriz
                        /// as primeiras 18 linhas (0 ate 17) sao hard e as seguintes sao todas soft
                        /// o numero de colunas é sempre o mesmo
                        /// Deste modo ter 12 pontos com a mao soft correspondera à linha 18(as linhas da hard vao de 0 a 17)
                        /// e é por isso que se adiciona 6 à pontuacao caso haja uma mao soft

                        else
                            jogada = j [aux_players->payload.pontos + 6][casa.pontos - 2];
                        /// funcao que realiza as jogadas da casa e retorna o nr_cartas restantes do baralho
                        nr_cartas = ea_jogar (aux_players, jogada,&(head_baralho),nr_baralhos, nr_cartas);
                    }

                aux_players = aux_players->next; /// passa a vez ao proximo jogador
                i++;/// atuliza a cor do quadrado

                }
            }

            // while there's events to handle
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT ) /// sair do jogo
                {
                    /// escrever ficheiro
                    escrever_stats(players,players_falidos, casa);
                    /// sair do while
                    quit = 1;
                }
                else if ( event.type == SDL_KEYDOWN )
                {
                    switch ( event.key.keysym.sym )
                        {
                            case SDLK_DOWN: /// aumenta o delay, decisões mais lentas
                                    delay_ea += 500;
                                    break;
                            case SDLK_UP: /// diminui o delay, decisões mais rápidas
                                    delay_ea -= 1000;
                                    if (delay_ea == 0)
                                        delay_ea = 1000;
                                    break;
                            case SDLK_s: /// stand
                                    if (i < 4 && aux_players->payload.tipo == 0 && aux_players->payload.ativo == 1)
                                    { /// player faz stand
                                        aux_players->payload.status = 2;
                                        aux_players = aux_players->next;
                                        i++;
                                    }
                                    break;
                            case SDLK_q: /// sair do jogo
                                    if (i <= MAX_PLAYERS)        /// se houver algum jogador a jogar
                                    {                           /// não faz nada
                                    }
                                    else /// sai do jogo no final de cada ronda
                                    {
                                        /// escrever ficheiro
                                        escrever_stats(players,players_falidos, casa);
                                        /// sair do while
                                        quit = 1;
                                    }
                                    break;
                            case SDLK_b: /// mudar o valor da aposta
                                    if (i <= MAX_PLAYERS)        /// so possivel no final das rondas
                                    {                           /// não faz nada
                                    }
                                    else
                                    {
                                        /// funcao que trata da mudanca de valores da aposta
                                        mudar_aposta (players);
                                        /// aponta para o inicio por precaução
                                        aux_players = players;
                                    }
                                    break;
                            case SDLK_n: /// new game
                                    if (i <= MAX_PLAYERS)        /// se houver algum jogador a jogar
                                    {                           /// não faz nada
                                    }
                                    else
                                    {
                                        /// aponta para o topo da lista de players
                                        aux_players = players;
                                        while (aux_players != NULL)
                                        {
                                            /// limpa as stacks de todos os players
                                            aux_players->payload.top = limpar_stack(aux_players->payload.top);
                                            /// tira as cartas do janela
                                            aux_players->payload.pos_player_hand = 0;
                                            /// como ainda nao jogaram
                                            aux_players->payload.status = 0;
                                            /// repete para todos
                                            aux_players = aux_players->next;
                                        }
                                        /// aponta para o topo da lista de players
                                        aux_players = players;
                                        /// recebe as cartas
                                        nr_cartas = start_game (players, &casa, nr_baralhos,nr_jogadores, nr_cartas, &(head_baralho));
                                        /// verificacao do baralho ja efetuada dentro da funcao anterior
                                        aux_players = players;
                                        i = 0;
                                    }
                                    break;
                            case SDLK_d: /// double
                                    /// so possivel se ainda nao tenha jogado
                                    if (aux_players->payload.status == 0 && aux_players->payload.tipo == 0 && aux_players->payload.ativo == 1)
                                    {
                                        if (aux_players->payload.dinheiro >= aux_players->payload.aposta) /// se tiver mais dinheiro que o valor da aposta
                                        {
                                            /// pedir carta
                                            nr_cartas = hit (nr_cartas, aux_players, &(head_baralho), nr_baralhos);
                                            /// verificar o final de baralho
                                            nr_cartas = verificar_baralho (nr_cartas,&(head_baralho), nr_baralhos);
                                            /// dinheiro é logo retirado
                                            aux_players->payload.dinheiro -= aux_players->payload.aposta;
                                            /// o status é Double
                                            aux_players->payload.status = 4;
                                            /// passa para o prox jogador
                                            aux_players = aux_players->next;
                                            i+=1;
                                        }
                                    }
                                    break;
                            case SDLK_r: /// surrender
                                    /// so possivel se ainda nao tenha jogado
                                    if (aux_players->payload.status == 0 && aux_players->payload.tipo == 0 && aux_players->payload.ativo == 1)
                                    {
                                        /// status passa a Surrender
                                        aux_players->payload.status = 3;
                                        /// passa para o prox player
                                        aux_players = aux_players->next;
                                        i+=1;
                                    }
                                    break;
                            case SDLK_h: /// hit
                                    if (aux_players->payload.tipo == 0 && aux_players->payload.ativo == 1 && i < MAX_PLAYERS)
                                    {
                                        /// pede uma carta
                                        nr_cartas = hit (nr_cartas, aux_players, &(head_baralho), nr_baralhos);
                                        /// verificar o final de baralho
                                        nr_cartas = verificar_baralho (nr_cartas,&(head_baralho), nr_baralhos);
                                        /// verificao de BUST
                                        if (aux_players->payload.pontos > 21)
                                        {
                                            ///atualizacao de status para BUST
                                            aux_players->payload.status = 5;
                                            /// passa para o proximo player
                                            aux_players = aux_players->next;
                                            i+=1;
                                        }
                                    }
                                    break;
                            case SDLK_a: /// adicionar jogador
                                    if (i <= MAX_PLAYERS)
                                    { /// so possivel no final da ronda
                                    }
                                    else
                                    {
                                        carrega = 0;
                                        while (carrega == 0) /// a é como o quit la em  cima
                                        {
                                            while( SDL_PollEvent( &event ) )
                                            {
                                                switch (event.type)
                                                {
                                                    case SDL_MOUSEBUTTONDOWN:
                                                        SDL_GetMouseState(&posx,&posy); /// obter as posicoes onde rato carregou

                                                    switch(event.button.button)
                                                    {
                                                        case SDL_BUTTON_LEFT: /// botao lado esquerdo
                                                            x = posx;
                                                            y = posy;
                                                            /// adicionar jogador
                                                            add_player(x, y, &(players),&(players_falidos),&nr_jogadores);
                                                            carrega = 1;/// para sair do ciclo while
                                                            break;
                                                        default:
                                                            break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                            default:
                                break;
                        }
                }
            }

            if (i == 4) /// quando todos ja jogaram
            {
                /// casa mostra a segunda carta e calcula os pontos
                casa.pos_house_hand = 2;
                /// casa calcula os pontos com a nova carta
                casa.pontos = cal_pts(casa.top, casa.pos_house_hand);

                while (casa.pontos < 17) /// pedir cartas ate atingir os 17 pontos
                {
                    /// dar uma carta a casa
                    carta_saida = retirar_carta(&head_baralho,&nr_cartas);
                    /// verificar o final de baralho
                    nr_cartas = verificar_baralho (nr_cartas,&(head_baralho), nr_baralhos);
                    casa.top = push(casa.top, carta_saida->payload.codigo, carta_saida->payload.naipe);
                    /// aumentar o numero de cartas da casa
                    casa.pos_house_hand++;
                    /// atuliza o status para hit
                    casa.status = 1;
                    /// calcula os pontos
                    casa.pontos = cal_pts(casa.top, casa.pos_house_hand);
                }
                /// atuliza o status para stand
                casa.status = 2;
                /// verifica se tem BJ
                if (casa.pontos == 21 && casa.pos_house_hand == 2)
                    casa.status = 6;
                /// verifica se fez BUST
                if (casa.pontos > 21)
                    casa.status = 5;


                ///gestao_money

                /// aponta para a head
                aux_players = players;

                while (aux_players != NULL)
                {
                    if (aux_players->payload.ativo == 1) /// calcula so para os jogadores ativos
                    {
                        ges_money(aux_players, &casa); /// funcao que gere o dinheiro dos players, da casa e conta as vitorias, as derrotas e os empates
                    }
                    aux_players = aux_players->next; /// passa para o proximo elemento da lista
                }

                aux_players = players; ///aponta novamente para a head

                while (aux_players != NULL) /// verificar se algum player faliu
                {
                    /// se ja nao tiver dinheiro ou se o dinheiro que tiver for menor que aposta
                    if(aux_players->payload.dinheiro == 0 || (aux_players->payload.aposta > aux_players->payload.dinheiro))
                    {
                        /// passa a desativo
                        aux_players->payload.ativo = 0;
                        /// ja nao tem cartas na mao
                        aux_players->payload.pos_player_hand = 0;
                        /// acabou o jogo
                        aux_players->payload.status = 7;
                        /// o  jogador ainda continua la na lista, mas a sua posicao no ecra parece como livre,
                        /// o  jogador so passa para

                    }
                    aux_players = aux_players->next; /// passa para verificar o prox
                }
                i = 5; /// sair do if
            }

        /// render game table
        RenderTable(i,nr_jogadores, players, serif, imgs, renderer);
        /// render house cards
        RenderHouseCards(casa, cards, renderer, serif);
        /// render player cards
        RenderPlayerCards(players, cards, renderer);
        /// render in the screen all changes above
        SDL_RenderPresent(renderer);
    	/// add a delay
		SDL_Delay(delay);



    }
    /// free memory allocated for images and textures and close everything including fonts
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();


	/// limpar as cartas dos jogadores

    aux_players = players; /// aponta para a head dos players (aqules que aparecem no ecra)

    while (aux_players != NULL)
    {
        /// limpa as stacks de todos os players
        aux_players->payload.top = limpar_stack(aux_players->payload.top);
        aux_players = aux_players->next;
    }

    aux_players = players_falidos; /// aponta para a head dos players_falidos

	while (aux_players != NULL)
    {
        /// limpa as stacks de todos os players falidos
        aux_players->payload.top = limpar_stack(aux_players->payload.top);
        aux_players = aux_players->next;
    }

	/// libertar o vetor com a estrategia da EA

    for (i = 0; i < COLUNAS; i++)
    {
        free(j[i]);
    }
    free(j);

	/// limpar a lista de jogadores
    limpar_jogadores (players);
    limpar_jogadores(players_falidos);

    /// limpar o baralho ;
    limpar_baralho (head_baralho);

	return EXIT_SUCCESS;
}

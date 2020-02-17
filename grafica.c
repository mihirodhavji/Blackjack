
#include "grafica.h"

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Rect boardPos;

    // space occupied by the logo
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = _logoIST->w;
	boardPos.h = _logoIST->h;

    // render it
	text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
	SDL_DestroyTexture(text_IST);
	return _logoIST->h;
}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(int I, int nr_jogadores, no_jogadores *_players, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer)
{
    SDL_Color black = { 0, 0, 0 }; /// black
    SDL_Color white = { 255, 255, 255 }; /// white
    char name_bet_str[50]="PLAYER", name_type_money_str[STRING_SIZE]="PLAYER";
    char hu[] = "HU", ea[] = "EA";
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); /// seperates the left from the right part of the window
    int height;
    char myName[]="Mihir Odhavji";
    char myNumber[]="84151";
    no_jogadores *aux = NULL;
    aux = _players;
    /// set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // renders the squares + name for each player
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    /// mudar a height
    height += 20;
    // renders the areas for each player: names and money too !

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (I == i)
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0 );
        else
            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );

        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

        if (aux != NULL)
        {
            if (aux->payload.ativo == 1)
            {
                if (aux->payload.status != 6 && aux->payload.pontos <= 21) /// caso tenha menos de 21 pontos
                {
                    sprintf(name_bet_str,"%s - %.1f - %d pts", aux->payload.nome, aux->payload.aposta, aux->payload.pontos);
                }

                else if (aux->payload.status == 6)
                {
                    sprintf(name_bet_str,"%s - %.1f - BJ", aux->payload.nome, aux->payload.aposta);
                }

                else if (aux->payload.status == 5)
                {
                    sprintf(name_bet_str,"%s - %.1f - BU", aux->payload.nome, aux->payload.aposta);
                }

                RenderText(playerRect.x+20, playerRect.y-30, name_bet_str, _font, &white, _renderer);

                /// escrever na parte lateral

                if (aux->payload.tipo == 0) /// se for EA
                {
                    sprintf(name_type_money_str,"%s--%s--%.1f", aux->payload.nome, hu ,aux->payload.dinheiro);
                }
                else /// se for HU
                {
                    sprintf(name_type_money_str,"%s--%s--%.1f", aux->payload.nome, ea ,aux->payload.dinheiro);
                }

                RenderText(separatorPos+3*MARGIN, height,name_type_money_str , _font, &black, _renderer);

                SDL_RenderDrawRect(_renderer, &playerRect);
                height += 20;
                aux = aux->next;
            }
            else
            {
                sprintf(name_bet_str,"ADD PLAYER HERE");
                RenderText(playerRect.x+30, playerRect.y+90, name_bet_str, _font, &white, _renderer);
                SDL_RenderDrawRect(_renderer, &playerRect);
                aux = aux->next;
            }
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
}

void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

    // destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(struct no_casa casa, SDL_Surface ** _cards, SDL_Renderer * _renderer, TTF_Font *_font)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    int num_carta, i = 0, a = 0;
    char house_points[STRING_SIZE] = "CASA";
    SDL_Color white = { 255, 255, 255 }; // white
    stack_cartas *top_copia = NULL;
    stack_cartas *top_original = NULL;
    top_original = casa.top;
    carta saida;
    if (casa.pos_house_hand == 1)  /// como a 1ª carta a ser recebida esta no fundo da stack original
        a = 2;                     /// e a casa so tem duas cartas (no inicio do jogo), o a = 2 e nao a = 1
                                   /// para ir "la ao fundo" da stack e imprimir a primeira a ser recebida
    else
        a = casa.pos_house_hand;

    /// drawing all house cards
    for ( card = 0; card < casa.pos_house_hand; card++)
    {
        while (i < a)   /// copia para a stack auxiliar(copia) as cartas da casa, desde modo na stack copia as cartas estaram arrumadas pela
        {               /// mesma ordem que foram destribuidas
            top_copia = push(top_copia, top_original->card.codigo, top_original->card.naipe); /// passa o codigo e o naipe das cartas
            top_original = top_original->next; /// passa para a proxima carta
            i++;
        }
        top_copia =  pop(top_copia, &saida); /// retira a carta do topo da stack para a a imprimir na tela
        num_carta = (saida.codigo + (saida.naipe * 13 )) - 1;   /// multiplica-se o naipe por 13, para sabermos em que naipe estamos
        /// calculate its position                              /// depois soma se o codigo da carta, e por ultimo subtrai-se 1
        x = (div/2-casa.pos_house_hand/2+card)*CARD_WIDTH + 15; /// porque as imagens da carta vao de 0 a 51, mas cartas na lista vao de 1 a 52
        y = (int) (0.26f*HEIGHT_WINDOW);
        /// render it !
        RenderCard(x, y, num_carta, _cards, _renderer);
    }
    /// just one card ?: draw a card face down
    if (casa.pos_house_hand == 1)
    {
        x = (div/2-casa.pos_house_hand/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
    if (casa.status == 6) /// caso a casa tenha BJ
        sprintf(house_points,"BJ");
    else if (casa.status != 6 && casa.status != 5)
        sprintf(house_points,"%d pts", casa.pontos);
    else if (casa.status == 5) /// caso a cas tenha feito BUST
        sprintf(house_points,"BU");
    /// escrever na tela
    RenderText(x, y-30, house_points, _font, &white, _renderer);
}


/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards( no_jogadores *players, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int pos, x, y, num_player = 0, card, num_carta;
    no_jogadores *aux_players = NULL;
    aux_players = players;
    stack_cartas *top_copia = NULL;
    stack_cartas *top_original = NULL;
    int i = 0;
    carta saida;
    // for every card of every player
    while (aux_players != NULL)
    {
        top_original = aux_players->payload.top;
        card = 0;
        while (i < aux_players->payload.pos_player_hand)
        {
            top_copia = push(top_copia, top_original->card.codigo, top_original->card.naipe);
            top_original = top_original->next;
            i++;

        }
        while (top_copia != NULL)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            top_copia =  pop(top_copia, &saida);
            num_carta = (saida.codigo + (saida.naipe * 13 )) - 1;
            pos = card % 4;
            x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            // render it !
            RenderCard(x, y, num_carta, _cards, _renderer);
            card++;
        }
        aux_players = aux_players->next;
        i = 0;
        num_player++;
    }
    top_copia = NULL;
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

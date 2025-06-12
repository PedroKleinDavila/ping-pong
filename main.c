#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

SDL_Rect quad1, mesa, linhaCentral, raquete1, raquete2, bola, placar;
int velocidade_bola_x = -5;
int velocidade_bola_y = 5;
int count = 0;
int velocidade_raquete2 = 5;
bool running = false;
int pontosVermelho = 0;
int pontosAzul = 0;
bool gameOver = false;
bool initialScreen = true;
int sdlDelay = 1000 / 60;

SDL_Rect placarVermelho = {400, 20, 50, 50};
SDL_Rect placarAzul = {830, 20, 50, 50};
SDL_Texture *numerosVermelhos[6];
SDL_Texture *numerosAzuis[6];

void commands(SDL_Event event)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            exit(0);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_UP && raquete1.y > mesa.y && running)
            {
                raquete1.y -= 20;
            }
            else if (event.key.keysym.sym == SDLK_DOWN && raquete1.y < mesa.y + mesa.h - raquete1.h && running)
            {
                raquete1.y += 20;
            }
            else if (event.key.keysym.sym == SDLK_SPACE)
            {
                running = true;
            }
            // tirar depois
            else if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }
    }
}

void initialize()
{
    quad1 = (SDL_Rect){50, 20, 400, 400};

    mesa.w = 1000;
    mesa.h = 500;
    mesa.x = (1280 - mesa.w) / 2;
    mesa.y = (720 - mesa.h) / 2;

    linhaCentral = (SDL_Rect){
        .w = 10, .h = 500, .x = (1280 - 10) / 2, .y = (720 - 500) / 2};

    raquete1 = (SDL_Rect){
        .w = 20, .h = 100, .x = mesa.x - 20, .y = (720 - 100) / 2};

    raquete2 = (SDL_Rect){
        .w = 20, .h = 100, .x = mesa.x + mesa.w, .y = (720 - 100) / 2};

    bola = (SDL_Rect){
        .w = 20, .h = 20, .x = (1280 - 20) / 2, .y = (720 - 20) / 2};
}

void resetGame()
{
    bola.x = (1280 - bola.w) / 2;
    bola.y = (720 - bola.h) / 2;
    velocidade_raquete2 = 5;
    count = 0;
    running = false;

    raquete1.x = mesa.x - raquete1.w;
    raquete1.y = (720 - raquete1.h) / 2;
    raquete2.x = mesa.x + mesa.w;
    raquete2.y = (720 - raquete2.h) / 2;
}

void ballMovement()
{
    if (bola.x == mesa.x && bola.y + bola.h >= raquete1.y && bola.y <= raquete1.y + raquete1.h)
    {
        velocidade_bola_x *= -1;
    }
    else if (bola.x <= mesa.x)
    {
        pontosAzul++;
        resetGame();
    }

    if (bola.x + bola.w == mesa.x + mesa.w &&
        bola.y + bola.h >= raquete2.y &&
        bola.y <= raquete2.y + raquete2.h)
    {
        velocidade_bola_x *= -1;
    }
    else if (bola.x + bola.w >= mesa.x + mesa.w)
    {
        pontosVermelho++;
        resetGame();
    }

    if (bola.y >= mesa.y + mesa.h - bola.h || bola.y <= mesa.y)
    {
        velocidade_bola_y *= -1;
    }

    if (running)
    {
        bola.y += velocidade_bola_y;
        bola.x += velocidade_bola_x;
    }
}

void raquete2Movement()
{
    if (count == 300)
    {
        srand(time(NULL));
        count = 0;
        velocidade_raquete2 = rand() % (5 - 3 + 1) + 3;
    }
    else
    {
        count++;
    }

    int centro_bola = bola.y + bola.h / 2;
    int centro_raquete2 = raquete2.y + raquete2.h / 2;

    if (centro_bola < centro_raquete2 && raquete2.y > mesa.y && running)
    {
        raquete2.y -= velocidade_raquete2;
    }
    else if (centro_bola > centro_raquete2 && raquete2.y + raquete2.h < mesa.y + mesa.h && running)
    {
        raquete2.y += velocidade_raquete2;
    }
}

SDL_Texture *carregarNumero(SDL_Renderer *renderer, int numero, const char *cor)
{
    char caminho[50];
    snprintf(caminho, sizeof(caminho), "./assets/%d%s.png", numero, cor);

    SDL_Surface *surface = IMG_Load(caminho);
    if (!surface)
    {
        printf("Erro ao carregar imagem: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

void carregarTexturas(SDL_Renderer *renderer)
{
    for (int i = 0; i <= 5; i++)
    {
        numerosVermelhos[i] = carregarNumero(renderer, i, "verm");
        numerosAzuis[i] = carregarNumero(renderer, i, "azul");
    }
}

void telaFimDeJogo(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect telaInteira = {0, 0, 1280, 720};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &telaInteira);

    const char *ganhador = (pontosVermelho == 5) ? "vermelhoGanhou" : "azulGanhou";
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "./assets/%s.png", ganhador);

    SDL_Surface *fimSurface = IMG_Load(caminho);
    if (!fimSurface)
    {
        printf("Erro ao carregar imagem de fim de jogo: %s\n", IMG_GetError());
        return;
    }

    SDL_Texture *fimTexture = SDL_CreateTextureFromSurface(renderer, fimSurface);
    SDL_FreeSurface(fimSurface);

    SDL_Rect fimRect = {0, 0, 1280, 720};
    SDL_RenderCopy(renderer, fimTexture, NULL, &fimRect);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(fimTexture);
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Ping Pong", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    initialize();
    carregarTexturas(renderer);

    while (true)
    {
        SDL_Event event;
        commands(event);

        if (initialScreen)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            SDL_Surface *surface = IMG_Load("./assets/inicio.png");
            if (!surface)
            {
                printf("Erro ao carregar imagem: %s\n", IMG_GetError());
                return -1;
            }

            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            SDL_Rect rect = {0, 0, 1280, 720};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderPresent(renderer);

            SDL_DestroyTexture(texture);

            while (SDL_WaitEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    exit(0);
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        exit(0);
                    }
                    else if (event.key.keysym.sym == SDLK_1)
                    {
                        sdlDelay = 1000 / 60;
                        initialScreen = false;
                        break;
                    }
                    else if (event.key.keysym.sym == SDLK_2)
                    {
                        sdlDelay = 1000 / 120;
                        initialScreen = false;
                        break;
                    }
                }
            }
        }
        else if (gameOver)
        {
            telaFimDeJogo(renderer);

            while (SDL_WaitEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    exit(0);
                else if (event.type == SDL_KEYDOWN)
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        exit(0);
                    }
                    else if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        pontosVermelho = 0;
                        pontosAzul = 0;
                        gameOver = false;
                        initialScreen = true;
                        resetGame();
                        break;
                    }
                }
            }
            continue;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
        SDL_RenderFillRect(renderer, &mesa);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &linhaCentral);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &raquete1);
        SDL_SetRenderDrawColor(renderer, 0, 107, 255, 255);
        SDL_RenderFillRect(renderer, &raquete2);
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderFillRect(renderer, &bola);
        if (pontosVermelho <= 5)
        {
            SDL_RenderCopy(renderer, numerosVermelhos[pontosVermelho], NULL, &placarVermelho);
        }
        if (pontosAzul <= 5)
        {
            SDL_RenderCopy(renderer, numerosAzuis[pontosAzul], NULL, &placarAzul);
        }
        if (pontosAzul == 5 || pontosVermelho == 5)
        {
            gameOver = true;
            resetGame();
        }

        SDL_RenderPresent(renderer);
        raquete2Movement();
        ballMovement();
        SDL_Delay(sdlDelay);
    }

    for (int i = 0; i <= 5; i++)
    {
        SDL_DestroyTexture(numerosVermelhos[i]);
        SDL_DestroyTexture(numerosAzuis[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
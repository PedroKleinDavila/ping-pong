#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

SDL_Rect quad1, mesa, linhaCentral, raquete1, raquete2, bola, placar, pikachuRect;

int velocidade_bola_x = -5;
int velocidade_bola_y = 5;
int count = 0;
int velocidade_raquete2 = 5;
bool running = false;

SDL_Texture *pikachuTexture = NULL;

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
            else if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }
    }
}

void initialize()
{
    quad1.x = 50;
    quad1.y = 20;
    quad1.w = 400;
    quad1.h = 400;

    mesa.w = 1000;
    mesa.h = 500;
    mesa.x = (1280 - mesa.w) / 2;
    mesa.y = (720 - mesa.h) / 2;

    linhaCentral.w = 10;
    linhaCentral.h = 500;
    linhaCentral.x = (1280 - linhaCentral.w) / 2;
    linhaCentral.y = (720 - linhaCentral.h) / 2;

    raquete1.w = 20;
    raquete1.h = 100;
    raquete1.x = mesa.x - raquete1.w;
    raquete1.y = (720 - raquete1.h) / 2;
    raquete2.w = 20;
    raquete2.h = 100;
    raquete2.x = mesa.x + mesa.w;
    raquete2.y = (720 - raquete2.h) / 2;

    bola.w = 20;
    bola.h = 20;
    bola.x = (1280 - bola.w) / 2;
    bola.y = (720 - bola.h) / 2;

    pikachuRect.x = 50;
    pikachuRect.y = 500;
    pikachuRect.w = 100;
    pikachuRect.h = 100;
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
    else
    {
        if (bola.x <= mesa.x)
        {
            resetGame();
        }
    }
    if (bola.x + bola.w == mesa.x + mesa.w && bola.y + bola.h >= raquete2.y && bola.y <= raquete2.y + raquete2.h)
    {
        velocidade_bola_x *= -1;
    }
    else
    {
        if (bola.x + bola.w >= mesa.x + mesa.w)
        {
            resetGame();
        }
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

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow(
        "Interface Gráfica usando SDL",
        100, 100,
        1280, 720,
        SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *pikachuSurface = IMG_Load("pikachu.png");
    if (!pikachuSurface)
    {
        printf("Erro ao carregar imagem: %s\n", IMG_GetError());
        return 1;
    }
    pikachuTexture = SDL_CreateTextureFromSurface(renderer, pikachuSurface);
    SDL_FreeSurface(pikachuSurface);
    if (!pikachuTexture)
    {
        printf("Erro ao criar textura: %s\n", SDL_GetError());
        return 1;
    }

    initialize();

    while (true)
    {
        SDL_Event event;
        commands(event);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 153, 0, 255);
        SDL_RenderFillRect(renderer, &mesa);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &linhaCentral);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &raquete1);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &raquete2);
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderFillRect(renderer, &bola);
        SDL_RenderCopy(renderer, pikachuTexture, NULL, &pikachuRect);
        SDL_RenderPresent(renderer);
        raquete2Movement();
        ballMovement();
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(pikachuTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
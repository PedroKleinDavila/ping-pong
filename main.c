#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        "Interface Gráfica usando SDL",
        100, 100,
        1280, 720,
        SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    //  SDL_Texture *img = IMG_LoadTexture(renderer, "pikachu.png");

    SDL_Rect quad1, mesa, linhaCentral, raquete1, raquete2;
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

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP && raquete1.y > mesa.y)
                {
                    raquete1.y -= 10;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && raquete1.y < mesa.y + mesa.h - raquete1.h)
                {
                    raquete1.y += 10;
                }
            }
        }

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
        // SDL_RenderCopy(renderer, img, NULL, &quad1);
        SDL_RenderPresent(renderer);
    }

    //  SDL_DestroyTexture(img);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

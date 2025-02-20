#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

void close_sdl(SDL_Window* window) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        printf("SDL can not disable compositor bypass!\n");
        return 0;
    }
#endif

    // Create window
    SDL_Window *window = SDL_CreateWindow("Basic C SDL project",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());


        // Quit SDL
        close_sdl(window);

        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        printf("Renderer could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
        close_sdl(window);
    }

    // Declare rect of square
    SDL_Rect squareRect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    squareRect.w = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    squareRect.h = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

    // Square position: In the middle of the screen
    squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
    squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;


    // Event loop exit flag
    bool quit = false;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        SDL_PollEvent(&e);
        //
        // User requests quit
        if(e.type == SDL_QUIT)
        {
            quit = true;
        }

        int min, max, random_number;

        // Seed the random number generator with the current time
        srand(time(NULL));

        // Define the range
        min = 0;
        max = 255;

        // Generate a random number within the range
        random_number = (rand() % (max - min + 1)) + min;

        // Print the random number
        printf("Random number between %d and %d: %d\n", min, max, random_number);


        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        // Set renderer color red to draw the square
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, random_number, 0xFF);

        // Draw filled square
        SDL_RenderFillRect(renderer, &squareRect);

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderDrawPoint(renderer, x + random_number, x);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);

   close_sdl(window);

    return 0;
}
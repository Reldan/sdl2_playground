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

typedef struct {
    int x;
    int y;
} vec2_t;

typedef struct
{
    int size;
    int height[256];
} ceiling_t;

typedef struct {
    int x;
    int y;
} drop_t;

typedef struct {
    ceiling_t ceiling;
    int drops_count;
    drop_t drops[20000];
} scene_t;

ceiling_t C_Init()
{
    ceiling_t ceiling;
    ceiling.size = 256;
    for (int i = 0; i < ceiling.size; i++) {
        // Define the range
        int min = 0;
        int max = 255;

        // Generate a random number within the range
        char random_number = (rand() % (max - min + 1)) + min;
        ceiling.height[i] = random_number;
    }
    return ceiling;
}

scene_t S_Init()
{
    scene_t scene;
    scene.ceiling = C_Init();
    scene.drops_count = 20000;
    for  (int i = 0; i < scene.drops_count; i++) {
        scene.drops[i].x = 0;
        scene.drops[i].y = 1000;
    }
    return scene;
}

void C_AddSteam(scene_t *scene) {
    int free_drop = -1;
    ceiling_t *ceiling = &scene->ceiling;
    for (int i = 0; i < ceiling->size; i++) {
        if (rand() % 2 == 1) {
            ceiling->height[i] = ceiling->height[i] + 1;
        }
        if (ceiling->height[i] > 100) {
            ceiling->height[i] = 0;
            for (;free_drop < scene->drops_count; free_drop++) {
                if (free_drop > 0 && scene->drops[free_drop].y >= 500) {
                    scene->drops[free_drop].y = 100;
                    scene->drops[free_drop].x = i;
                    break;
                }
            }
            if (free_drop == scene->drops_count) {
                printf("Free drops\n");
            }
        }
    }
    for (int i = 0; i < scene->drops_count; i++) {
        if (scene->drops[i].y < 500) {
            scene->drops[i].y = scene->drops[i].y + 1;
        }
    }
}

void close_sdl(SDL_Window* window) {
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
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

    scene_t scene = S_Init();

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

        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        int width = SCREEN_WIDTH / scene.ceiling.size;
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xAA, 0xFF);

        for (int i = 0; i < scene.ceiling.size; i++) {
            int height = scene.ceiling.height[i];

            // Declare rect of square
            SDL_Rect squareRect;

            squareRect.x = i * width;
            squareRect.y = 0;

            squareRect.w = width;
            squareRect.h = height;

            SDL_RenderDrawRect(renderer, &squareRect);
        }

        for  (int i = 0; i < scene.drops_count; i++) {
            drop_t drop = scene.drops[i];
            if  (drop.y < 500) {
                int height = 10;

                // Declare rect of square
                SDL_Rect squareRect;

                squareRect.x = drop.x * width;
                squareRect.y = drop.y;

                squareRect.w = width;
                squareRect.h = height;
                SDL_RenderDrawRect(renderer, &squareRect);
            }
        }


        // Update screen
        SDL_RenderPresent(renderer);
        C_AddSteam(&scene);
        SDL_Delay(10);
    }

    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    close_sdl(window);

    return 0;
}
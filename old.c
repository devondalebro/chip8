//Using SDL and standard IO
#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_surface.h"
#include <stdio.h>

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window *g_window = NULL;
SDL_Surface *g_surface = NULL;
SDL_Surface *g_out = NULL;

int init(void) {
    int success = 1;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not be initialised. SDL_ERROR: %s\n", SDL_GetError());
        success = 0;
    } else {
        g_window = SDL_CreateWindow("SDL Tutorial", 
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                    SCREEN_WIDTH, SCREEN_HEIGHT, 
                                    SDL_WINDOW_SHOWN);
        if (g_window == NULL) {
            printf("Window could not be created. SDL_ERROR: %s\n", SDL_GetError());
            success = 0;
        } else {
            g_surface = SDL_GetWindowSurface(g_window);
        }
    }
    
    return success;
}

int load_media(void) {
    int success = 1;

    SDL_FillRect(g_surface, NULL, SDL_MapRGB(g_surface->format, 0xFF, 0xFF, 0xFF));

    if (g_surface == NULL) {
        printf("Media could not be loaded. SDL_ERROR: %s\n", SDL_GetError());
        success = 0;
    }

    return success;
}

void close(void) {
    SDL_FreeSurface(g_surface);
    g_surface = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    SDL_Quit();
}

int main(void) {
    if (!init()) {
        printf("SDL not initialised\n");
        return 1;
    }

    if (!load_media()) {
        printf("Media not loaded\n");
        return 1;
    }

    SDL_UpdateWindowSurface(g_window);

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    close();
}

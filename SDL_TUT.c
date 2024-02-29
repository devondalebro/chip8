//Using SDL and standard IO
#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_error.h"
#include "include/SDL2/SDL_events.h"
#include "include/SDL2/SDL_render.h"
#include "include/SDL2/SDL_video.h"

#include <stdio.h>
#include <stdbool.h>

//Screen dimension constants
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

int main(void)
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    
    //The surface contained by the window

    //Initialize SDL
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                  SCREEN_WIDTH * 20, SCREEN_HEIGHT * 20, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_RenderSetScale(renderer, 20, 20);

            bool display[SCREEN_HEIGHT][SCREEN_WIDTH] = {0};

            display[10][10] = true;
            display[20][20] = true;

            for (int j = 0; j < SCREEN_HEIGHT; j++) {
                for (int i = 0; i < SCREEN_WIDTH; i++) {
                    if (display[j][i] == true) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    } else { 
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    }
                    SDL_RenderDrawPoint(renderer, i, j);
                }
            }
            SDL_RenderPresent(renderer);

            //Hack to get window to stay up
            SDL_Event e; 
            int quit = 0; 
            while(!quit){ 
                while( SDL_PollEvent( &e ) ){ 
                    if( e.type == SDL_QUIT ) {
                        quit = 1; 
                    } 
                }
            }
        }
    }

    //Destroy window
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer(renderer);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}


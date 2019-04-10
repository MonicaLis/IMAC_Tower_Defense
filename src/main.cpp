#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "map_algo.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1000;


int main(int argc, char ** argv)
{
    bool mum = load_map("data/carte.itd");
    cout<<mum<<endl;

    /*
    bool quit = false;
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_Window * window = SDL_CreateWindow("IMAC TOWER DEFENSE",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_WIDTH, 0);
 
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    //SDL_Surface * image = IMG_Load("images/sam.ppm");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
 
    while (!quit)
    {
        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
 
        //SDL_Rect dstrect = { 5, 5, 320, 240 };
        //SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
 
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit(); */
 
    return 0;
}
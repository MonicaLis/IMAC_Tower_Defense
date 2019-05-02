#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"


int main(int argc, char ** argv)
{
    //loading and verifying the map
    bool verify_map = load_map("data/carte.itd");
    cout << verify_map << endl;

    //create the graph out of the given nodes and therefore the map
    Graph map = create_graph();
    create_map_ppm(map);
    Image* map_image = load("doc/MAP.ppm");

    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_Window * window = SDL_CreateWindow("IMAC TOWER DEFENSE",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 800, 0);
 
    /*==============LOAD MAP BACKGROUND=================================*/
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = IMG_Load("images/space.ppm");
    if(!image) cout << "IMG_Load error: "<<IMG_GetError()<<endl;
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_Rect dstrect = { 100, 100, 600, 600 };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    /*==============DRAW PATH==========================================*/
    int i,j;
    Pixel white = create_pixel(255,255,255);
    SDL_Surface * brick;
    SDL_Texture * texture_brick;
    SDL_Rect size_brick;
    for (i=0; i<500; i++)
    {
        for (j=0; j<600; j++)
        {
            Pixel current = get_pixel(i,j,map_image);
            //if the pixel is white then it's the path and we need to create it on the image map
            if (are_they_equal( current, white ))
            {
                brick = IMG_Load("images/brick.ppm");
                if(!brick) cout << "IMG_Load error: "<<IMG_GetError()<<endl;
                texture_brick = SDL_CreateTextureFromSurface(renderer, brick);
                size_brick = { i+100, j+100, 10, 10 }; //location, width, height
                SDL_RenderCopy(renderer, texture_brick, NULL, &size_brick);
            }
        }
    }
    int x=0;
    int y=0;
    while (!quit)
    {
        //Uint32 startTime = SDL_GetTicks();
        //glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW); //transformations s'appliqunt sur la bonne matrice
        glLoadIdentity(); //remet en etat initial 

        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
        }
        
        SDL_RenderPresent(renderer);
        SDL_GL_SwapWindow(window);

        //Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Evenements utilisateurs */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            
            switch(e.type) 
            {
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                  //  Tower tower=new Tower;

                    glPushMatrix();
                    glColor3f(150,25,160);
                        glTranslated(e.button.x,e.button.y,0);
                        glScalef(20,20,20);
                        drawTower(1);
                    glPopMatrix();

                   
                    cout << "clic en "<< e.button.x<< " " << e.button.y<< endl;
                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                     cout << "touche "<< e.key.keysym.sym<< endl;
                    break;
                    
                default:
                    break;
            }
        }
    }
 
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_brick);
    SDL_FreeSurface(image);
    SDL_FreeSurface(brick);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
 
    SDL_Quit();
 
    return 0;
}
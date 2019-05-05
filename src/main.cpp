#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "player.h"

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char ** argv)
{
   // TTF_Init();
   
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


    /*==============PLAYER INIT==========================================*/
    Player player;
    int money = player.get_money();
    cout << "Argent disponbile : "<<money<<endl;
    
    /*==============TEXT INIT==========================================*/

   // TTF_Font *font; 
   //font = TTF_OpenFont( "font/roboto.ttf", 28 );
   // SDL_Surface *screen;
   // SDL_Color color={255,0,0};
   // SDL_Surface *text_surface;
  
  //if(!font){
   //  cout << "Font error: "<<TTF_GetError()<<endl;
 // } 
 //   text_surface=TTF_RenderText_Solid(font,"helloworld",color);
  
    while (!quit)
    {
     
        Uint32 startTime = SDL_GetTicks();
        //glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW); //transformations s'appliqunt sur la bonne matrice
        glLoadIdentity(); //remet en etat initial 
         
        SDL_RenderPresent(renderer);
        SDL_GL_SwapWindow(window);
      
        //  SDL_WaitEvent(&event);
        
        /* Evenements utilisateurs */
       
        while (SDL_PollEvent(&event)) {
            
            switch(event.type) 
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                {
                    if(money>0){
                        //CONTRUCT TOWER
                        Tower newTower;

                        Pixel whiteTower = create_pixel(255,255,255);
                        SDL_Surface * tower;
                        SDL_Texture * texture_tower;
                        SDL_Rect size_tower;
                        Pixel currentTower = get_pixel(event.button.x,event.button.y,map_image);
                        //if the pixel is white then it's the path and we can't creat a tower
                        if (!are_they_equal( currentTower, whiteTower ))
                        {
                            tower = IMG_Load("images/tower.ppm");
                            if(!tower) cout << "IMG_Load error: "<<IMG_GetError()<<endl;
                            texture_tower = SDL_CreateTextureFromSurface(renderer, tower);
                            size_tower = { event.button.x-25, event.button.y-25, 50,50 }; //location, width, height
                            SDL_RenderCopy(renderer, texture_tower, NULL, &size_tower);
                        }
                        
                        money=player.get_money()-newTower.get_cost();
                        player.set_money(money);
                        cout << "Argent disponbile : "<<money<<endl;
                    }
                    else{
                        cout << "Pas d'argent disponible"<<endl;
                    }
                }
                   // cout << "clic en "<< event.button.x<< " " << event.button.y<< endl;}
                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:
                     cout << "touche "<< event.key.keysym.sym<< endl;
                    break;
                    
                default:
                    break;
            }

        }
        
     
        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
       
    }

    /*==============CLOSE WINDOW==========================================*/

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_brick);
    SDL_FreeSurface(image);
    SDL_FreeSurface(brick);
  //  SDL_FreeSurface(text_surface);
  //  SDL_FreeSurface(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  //  TTF_CloseFont(font);
  //  TTF_Quit();
    SDL_Quit();
 
    return 0;
}
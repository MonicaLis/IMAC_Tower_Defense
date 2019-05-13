
#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include "init.h"
#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char **argv) {

    /* Init SDL, OpenGL, Debug and Glad */
    SDL_Window* window = init();
    if (window == nullptr) {
        cout << "Error window init" << endl;
    }

    /*loading and verifying the map*/
    bool verify_map = load_map("data/carte.itd");
    cout << verify_map << endl;


    /*==============PLAYER INIT==========================================*/
    Player player;
    int money = player.get_money();
    cout << "Argent disponbile : "<<money<<endl;

    GLuint texturePath=initTexturePath();
    GLuint textureTower=initTextureTower();
    GLuint textureMonster=initTextureMonster();

    std::vector<Tower*> towers;
    std::vector<Monster*> monsters;
    std::vector<Monster*> supr ;

    /* Boucle d'affichage */
    bool loop = true;
    bool wave=false;
    int numberWave=0;

    /*create the graph out of the given nodes and therefore the map*/
    Graph map = create_graph();
    verify_path (map);
    //will be used by drawPath(...)
    Image* img_map = create_map_ppm(map); 
    //work out where monsters go
    int monsters_enter_x = map.get_node(0).get_coordinates().get_p_x();
    int monsters_enter_y = map.get_node(0).get_coordinates().get_p_y();
    int monsters_exit_x = map.get_node(1).get_coordinates().get_p_x();
    int monsters_exit_y = map.get_node(1).get_coordinates().get_p_y();

    while (loop) {

        Uint32 startTime = SDL_GetTicks();

        /* code to draw */
        glClear(GL_COLOR_BUFFER_BIT);
        //draw map and path
        display_map();
        draw_path(img_map);

         /* Update entities */
        for (Tower* tower : towers) {
            tower->drawTower();
        }
        for (Monster* monster : monsters) {
            monster->drawMonster();
            if (monsters.size() > 0){
                int numberX = rand() % 5;   
                int numberY = rand() % 5;      // v1 in the range 0 to 99
                int Posx=monster->get_x()+numberX;
                int Posy=monster->get_y()+numberY;
                monster->set_x(Posx);
                monster->set_y(Posy);
            }
            
        }
        
        
        int time=0;
       
        

       if(wave && monsters.size()>0){
            
            for (Tower* tower : towers) {
                int loopMonster=0; 
                for (Monster* monster : monsters) {
                    time+=1;
                    int compareX= tower->get_x()-monster->get_x();
                    int compareY= tower->get_y()-monster->get_y();
                    if(monsters.size()>0 && compareX<100 && compareY<100 && time>=3){
                        if(monster->get_life_points()<=0){
                            supr.push_back(monster);
                            monsters.erase(monsters.begin()+loopMonster);
                        }
                        if(monster->get_life_points()>0){
                        int life=monster->get_life_points()-2;
                        monster->set_life_points(life);
                        cout << "Monster life : "<<life<<endl;
                        time=0;
                        }
                    }
                    
                    loopMonster+=1;
                }
            
            }
        }
        if(monsters.size()<=0){
            wave=false;
        }
       
       
        if(!supr.empty()){
            for (Monster* toSupr : supr) {
            supr.erase(supr.begin(), supr.end());
            delete toSupr;
            cout << "Monstre tué" <<endl;
            }
        
        }
   
        /* Swap front and back buffers */
        SDL_GL_SwapWindow(window);

        /* Loop for events */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            /* User closes window */
            if (e.type == SDL_QUIT)
            {
                loop = false;
                break;
            }

            switch (e.type)
            {
                /* click */
                case SDL_MOUSEBUTTONUP:
                {
                    int x_conversion = e.button.x;
                    int y_conversion = e.button.y;
                    to_ppm_coordinates(x_conversion, y_conversion);

                    // cout << "clicked in "<< e.button.x<< " " << e.button.y<< endl;
                    // cout << "converted:" << x_conversion << " " << y_conversion <<endl;

                    if(money>0 && !wave){

                        bool valid_zone;    
                        Tower* newTower = new Tower(e.button.x,e.button.y, textureTower, img_map, valid_zone);
                        if (valid_zone)
                        {
                            towers.push_back(newTower);
                            cout<<"Tower built"<<endl;
                            money=player.get_money()-newTower->get_cost();
                            player.set_money(money);
                            cout << "Argent disponbile : "<<money<<endl;
                        }
                    }
                    
                    if(wave==true){
                        cout << "No right to build during a wave"<<endl;
                    }
                    if(money<=0){
                        cout << "No money available"<<endl;
                    }
                
                }
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == 'w') {
                        printf("New wave\n");
                        wave = true;
                        
                        numberWave+=1;
                        
                        for(int i=1; i<=numberWave; i++){
                            int life;
                            Monster* newMonster= new Monster(
                                    monsters_enter_x, monsters_enter_y, textureMonster);
                            life=newMonster->get_life_points()+2;
                            newMonster->set_life_points(life);
                            monsters.push_back(newMonster);
                            
                        }
                        

                    }
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;

                default:
                    break;
            }
        }


        /* Calcul du temps ecoule */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if (elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associees a la SDL */
    SDL_DestroyWindow(window);
    SDL_Quit();

    //  PAS SURE DE CETTE PARTIE NORMALEMENT CES STRUCTS SE DETRUISENT TOUTES SEULES SI T'AS FAIT DES DESTRUCTEURS
    for (Tower* tower : towers) {
        delete tower;
    }
      for (Monster* monster : monsters) {
        delete monster;
    }

    return EXIT_SUCCESS;
}

#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stb_image/stb_image.h>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
//#include <SDL2/SDL_ttf.h>


#include "init.h"
#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"


static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void text();

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

    BEGIN:

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
    int enter_x = map.get_node(0).get_coordinates().get_p_x();
    int enter_y = map.get_node(0).get_coordinates().get_p_y();
    //to_sdl_coordinates(enter_x, enter_y);
    int exit_x = map.get_node(1).get_coordinates().get_p_x();
    int exit_y = map.get_node(1).get_coordinates().get_p_y();
    //to_sdl_coordinates(exit_x, exit_y);
    int node_3_x = map.get_node(3).get_coordinates().get_p_x();
    int node_3_y = map.get_node(3).get_coordinates().get_p_y();
    //to_sdl_coordinates(node_3_x, node_3_y);
    int node_4_x = map.get_node(4).get_coordinates().get_p_x();
    int node_4_y = map.get_node(4).get_coordinates().get_p_y();
    //to_sdl_coordinates(node_4_x, node_4_y);
    int node_2_x = map.get_node(2).get_coordinates().get_p_x();
    int node_2_y = map.get_node(2).get_coordinates().get_p_y();
    //to_sdl_coordinates(node_2_x, node_2_y);


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
            cout<<"random is "<<random<<endl;

            if (monster->get_path() == 0)
            {   
                cout<<"mon "<<endl;
                //to go from the entrance to P3
                monster->move(enter_x, 2, 1, 1, 0, node_3_x-30, node_2_x);
                //go to N2 and finally the exit
                monster->move(node_2_x, 1, 0, 1, 0, node_2_x, exit_x);
            }
            else 
            {
                cout<<"here "<<endl;
                //to go from the entrance to N4 and then N2
                monster->move(enter_x, 2, 3, 1, -2, node_4_x-15, node_2_x-20);
                //go to N2 and finally the exit
                monster->move(node_2_x-20, 1, 0, 1, 0, node_2_x-20, exit_x-20);
            }
            if (monster->get_x() == exit_x)
            {
                cout << "Game over"<<endl;
                for (Tower* tower : towers) {
                    delete tower;
                }
                for (Monster* monster : monsters) {
                    delete monster;
                }
                for (Monster* toSupr : supr) {
                    delete toSupr;
                }
                goto BEGIN;
            }
        }         
        
        
        int time=0;
       
        

       if(wave && monsters.size()>0){
            
            for (Tower* tower : towers) 
            {
                int loopMonster=0; 
                for (Monster* monster : monsters) 
                {
                    time+=1;
                    int compareX= tower->get_x()-monster->get_x();
                    int compareY= tower->get_y()-monster->get_y();
                    if(monsters.size()>0 && compareX<100 && compareY<100 && time>=3)
                    {
                        if(monster->get_life_points()<=0)
                        {
                            supr.push_back(monster);
                            monsters.erase(monsters.begin()+loopMonster);
                        }
                        if(monster->get_life_points()>0)
                        {
                            int life=monster->get_life_points()-2;
                            monster->set_life_points(life);
                            cout << "Monster's life points : "<<life<<endl;
                            time=0;
                        }
                    }
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
            cout << "Monster killed" <<endl;
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
                            cout << "Available money : "<<money<<endl;
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

                /* Key */
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == 'w') {
                        printf("New wave\n");
                        wave = true;
                        
                        numberWave+=1;
                        
                        for(int i=1; i<=numberWave; i++){
                            int life;
                            Monster* newMonster= new Monster(
                                    enter_x, enter_y, textureMonster);
                            life=newMonster->get_life_points()+2;
                            newMonster->set_life_points(life);
                            monsters.push_back(newMonster);
                        }
                        

                    }
                    break;

                default:
                    break;
            }
        }


        /* Time elapsed */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* if not enough time has passed then pause the program */
        if (elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Free SDL resources */
    SDL_DestroyWindow(window);
    delete_image(img_map);
    texturePath=initTexturePath();
    glDeleteTextures(1, &texturePath);
    glDeleteTextures(1, &textureTower);
    glDeleteTextures(1, &textureMonster);
    SDL_Quit();

    for (Tower* tower : towers) {
        delete tower;
    }
      for (Monster* monster : monsters) {
        delete monster;
    }

    for (Monster* toSupr : supr) {
        delete toSupr;
    }

    return EXIT_SUCCESS;
}




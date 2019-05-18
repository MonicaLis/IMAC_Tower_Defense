
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

#include "init.h"
#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"
#include "buildings_algo.h"
#include "buildings_graphic.h"


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

    /*images' paths*/
    const char* radar_path = "images/radar.png";
    const char* factory_path = "images/arme3.png";
    const char* munitions_path = "images/arme4.png";

    /*Variables*/
    int typeBuilding = 5; //to mean that the player hasn't chosen yet
    int pos_x, pos_y;
    int life;
    int tempGO=0;

    /*INIT TEXTURE*/
    GLuint textureTower=initTextureTower();
    GLuint textureMonster=initTextureMonster();
    GLuint textureFire=initTextureFire();
    GLuint textureRadar=initTextureBuilding(radar_path);
    GLuint textureFactory=initTextureBuilding(factory_path);
    GLuint textureMunitions=initTextureBuilding(munitions_path);
   
    BEGIN:
   /*INIT PLAYER*/
    Player player;
    int money = player.get_money();
    cout << "Available money : "<<money<<endl;

    /*INIT ENTITIES LISTS*/
    vector<Tower*> towers;
    vector<Monster*> monsters;
    vector<Monster*> supr;
    vector<Building*> buildings;

    /* Boucle d'affichage */
    bool loop = true;
    bool wave=false;
    int numberWave=0;

    /*create the graph out of the given nodes and therefore the map*/
    Graph map = create_graph();
    verify_path (map);
    //get the ppm image to place towers, buildings, etc.
    Image* img_map = create_map_ppm(map); 
    //work out where monsters go
    int enter_x = map.get_node(0).get_coordinates().get_p_x();
    int enter_y = map.get_node(0).get_coordinates().get_p_y();
    int exit_x = map.get_node(1).get_coordinates().get_p_x();
    int exit_y = map.get_node(1).get_coordinates().get_p_y();
    int node_3_x = map.get_node(3).get_coordinates().get_p_x();
    int node_3_y = map.get_node(3).get_coordinates().get_p_y();
    int node_4_x = map.get_node(4).get_coordinates().get_p_x();
    int node_4_y = map.get_node(4).get_coordinates().get_p_y();
    int node_2_x = map.get_node(2).get_coordinates().get_p_x();
    int node_2_y = map.get_node(2).get_coordinates().get_p_y();

    while (loop) {
       
        Uint32 startTime = SDL_GetTicks();
        glClear(GL_COLOR_BUFFER_BIT);
       
        //draw map and path
        //draw_path(img_map); //no need for this now that we have the path drawn on the map
        display_map();
        /*INIT VARIABLE*/
        float x = 0;
        float y = 0;
        int time=0;
        bool valid_zone;
        bool success = false;

         /* Update towers */
        for (Tower* tower : towers) {
            tower->drawTower();
        }

        /* Update buildings */
        for (Building* building : buildings) {
            building->drawBuilding();
        }

        /*Update Monster*/
         for (Monster* monster : monsters) {
        
            monster->drawMonster();

            if (monster->get_path() == 0)
            {   
                //to go from the entrance to P3
                monster->move(enter_x, 2, 1, 1, 0, node_3_x-30, node_2_x);
                //go to N2 and finally the exit
                monster->move(node_2_x, 1, 0, 1, 0, node_2_x, exit_x);
            }
            else 
            {
                //to go from the entrance to N4 and then N2
                monster->move(enter_x, 2, 3, 1, -2, node_4_x-15, node_2_x-20);
                //go to N2 and finally the exit
                monster->move(node_2_x-20, 1, 0, 1, 0, node_2_x-20, exit_x-20);
            }
            if (monster->get_x() == exit_x)
            {
                cout << "Game over"<<endl;
                display_gameover();
                
                
                     if(tempGO<35){
                    tempGO++;
                    }
                    else{
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
        }         
       
        
       //WIN CONDITION
       if(numberWave==3 && wave==false && monsters.size()<=0){
            cout << "You won!"<<endl;
            
             display_gameover();
             if(tempGO<35){
                    tempGO++;
                    }
                    else{
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
        
        //TOWER ATTACK MONSTER
       if(wave && monsters.size()>0){
            
            for (Tower* tower : towers) {
                int loopMonster=0; 
                for (Monster* monster : monsters) 
                {
                    tower_attacks_monsters(success, money, time, loopMonster, tower, monster, monsters, supr, player);
                    if (success) 
                    {
                        tower->drawFire(textureFire);
                    }
                }
            }
        }

        //Wave is false when no monster
        if(monsters.size()<=0){
            wave=false;
        }
       
       //Delete monsters who are dead
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

                     //cout << "clicked in "<< e.button.x<< " " << e.button.y<< endl;
                     //cout << "converted:" << x_conversion << " " << y_conversion <<endl;

                    choose_building(e.button.x, e.button.y, typeBuilding);

                    //BUILD A BUILDING
                    /*TOWER*/
                    if( (money>0) && (!wave) && (typeBuilding == -1)){
                        Tower* newTower = new Tower(e.button.x, e.button.y, textureTower, img_map, valid_zone);
                        if (newTower->get_cost() > money) 
                        {
                            cout<<"You don't have enough money"<<endl;
                            valid_zone = false;
                            break;
                        }
                        if (valid_zone)
                        {
                            towers.push_back(newTower);
                            cout<<"Tower built of type "<<newTower->get_type()<<endl;
                            money = player.get_money() - newTower->get_cost();
                            player.set_money(money);
                            cout << "Available money : "<<money<<endl;
                        }
                    }
                    /*RADAR*/
                    if( (money>0) && (!wave) && (typeBuilding == 0))
                    {
                        Building* newBuilding = new Building(textureRadar, 0, e.button.x, e.button.y, img_map, valid_zone);
                        //if the player has enough money, if the zone is valid, then the building will be built
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) ) add_building(buildings, towers, newBuilding);
                    }
                    /*FACTORY*/
                    if( (money>0) && (!wave) && (typeBuilding == 1))
                    {
                        Building* newBuilding = new Building(textureFactory, 1, e.button.x, e.button.y, img_map, valid_zone);
                        after_chose_building(newBuilding, valid_zone, &player, money);
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) ) add_building(buildings, towers, newBuilding);
                    }
                    /*MUNITIONS*/
                    if( (money>0) && (!wave) && (typeBuilding == 2))
                    {
                        Building* newBuilding = new Building(textureMunitions, 2, e.button.x, e.button.y, img_map, valid_zone);
                        after_chose_building(newBuilding, valid_zone, &player, money);
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) ) add_building(buildings, towers, newBuilding);
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
                    //NEW WAVE : create monster
                    if (e.key.keysym.sym == 'w') {
                        wave = true;
                        numberWave+=1;
                        cout << "New wave no "<<numberWave<<endl;
                        for(int i=1; i<=numberWave; i++){
                            Monster* newMonster= new Monster(
                                    enter_x, enter_y, textureMonster, numberWave);
                            monsters.push_back(newMonster);
                            life=newMonster->get_life_points()+life;
                            newMonster->set_life_points(life);
                        }
                        life++;
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
    glDeleteTextures(1, &textureFire);
    glDeleteTextures(1, &textureTower);
    glDeleteTextures(1, &textureMonster);
    glDeleteTextures(1, &textureRadar);
    glDeleteTextures(1, &textureFactory);
    glDeleteTextures(1, &textureMunitions);
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

    for (Building* building : buildings) {
        delete building;
    }

    return EXIT_SUCCESS;
}

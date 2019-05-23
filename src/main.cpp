
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
#include <sstream>

#include "init.h"
#include "map_algo.h"
#include "map_graphic.h"
#include "towers_algo.h"
#include "towers_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"
#include "interface.h"
#include "buildings_algo.h"
#include "buildings_graphic.h"


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
    GLuint textureMap=init_map();
    GLuint textureWin=display_win();
    GLuint textureGO=display_gameover();
   
    BEGIN:
   /*INIT PLAYER*/
    Player player;
    int money = player.get_money();
    GLuint textureMoney=display_money(10);

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
       
        glClear(GL_COLOR_BUFFER_BIT);

        drawMap(textureMap);
        drawMoney(textureMoney);

        /*INIT VARIABLE*/
        float x = 0;
        float y = 0;
        int time = 0;
        bool valid_zone;
        bool success = false;

        if(towers.size()>0){
             /* Update towers */
            for (Tower* tower : towers) {
                tower->drawTower();
            }
        }
        
        if(buildings.size()>0){
            /* Update buildings */
            for (Building* building : buildings) {
                building->drawBuilding();
            }
        }
        
        if(monsters.size()>0){
            /*Update Monster*/
            for (Monster* monster : monsters) {
        
                if (monster->get_life_points() > 0) monster->drawMonster();

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

                /*GAME OVER*/
                if (monster->get_x() == exit_x)
                { 
                    drawGO(textureGO);
                    if(tempGO<35){
                        tempGO++;
                    }
                    else{
                        delete_all(towers, monsters, supr, buildings);
                        glDeleteTextures(1, &textureMoney);
                        goto BEGIN;
                    }   
                }
            }  
        }  
       
        
       //WIN CONDITION
        if(numberWave==5 && wave==false && monsters.size()==0){            
            drawWin(textureWin);
            if(tempGO<35){
                tempGO++;
            }
            else{
                delete_all(towers, monsters, supr, buildings);
                glDeleteTextures(1, &textureMoney);
                goto BEGIN;
            }     
       }
        
        //TOWER ATTACK MONSTER
       if(wave && monsters.size()>0){
            
            for (Tower* tower : towers) {
                int loopMonster = 0; 
                for (Monster* monster : monsters) 
                {
                    tower_attacks_monsters(success, money, time, loopMonster, tower, monster, monsters, supr, player, numberWave);
                    textureMoney=display_money(player.get_money());
                    //if the tower shot, draw fire
                    if (success) tower->drawFire(textureFire); 
                }
            }
        }
        //doing this inside the first FOR causes memory issues
        int i = 0;
        for (Monster* monster : monsters) rid_monsters(i, monster, monsters, supr);

        //Wave is false when no monster
        if(monsters.size()<=0){
            wave=false;
        }
       
       //DO WE REALLY NEED TO DELETE THIS LIST WHEN WE DELETE IT AT THE END?
    //    //Delete monsters who are dead
    //     if(!supr.empty()){
    //             for (Monster* toSupr : supr) {
    //             supr.erase(supr.begin(), supr.end());
    //             delete toSupr;
    //         }
    //     }

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
                          //  glDeleteTextures(1, &textureMoney);
                             textureMoney=display_money(money);
                            player.set_money(money);
                            cout << "Available money : "<<money<<endl;
                        }
                    }
                    /*RADAR*/
                    if( (money>0) && (!wave) && (typeBuilding == 0))
                    {
                        Building* newBuilding = new Building(textureRadar, 0, e.button.x, e.button.y, img_map, valid_zone);
                        //if the player has enough money, if the zone is valid, then the building will be built
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) )
                        {
                            add_building(buildings, towers, newBuilding);
                        } 
                        textureMoney=display_money(player.get_money());
                    }
                    /*FACTORY*/
                    if( (money>0) && (!wave) && (typeBuilding == 1))
                    {
                        Building* newBuilding = new Building(textureFactory, 1, e.button.x, e.button.y, img_map, valid_zone);
                        after_chose_building(newBuilding, valid_zone, &player, money);
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) ){
                            add_building(buildings, towers, newBuilding);   
                        } 
                        textureMoney=display_money(player.get_money());
                    }
                    /*MUNITIONS*/
                    if( (money>0) && (!wave) && (typeBuilding == 2))
                    {
                        Building* newBuilding = new Building(textureMunitions, 2, e.button.x, e.button.y, img_map, valid_zone);
                        after_chose_building(newBuilding, valid_zone, &player, money);
                        if ( after_chose_building(newBuilding, valid_zone, &player, money) ){
                            add_building(buildings, towers, newBuilding);
                        } 
                        textureMoney=display_money(player.get_money());
                    }
                    
                    if(wave==true){
                        cout << "No right to build during a wave"<<endl;
                    }
                    if(money<=0){
                        cout << "No money available"<<endl;
                    }
                    if( (e.button.x>=825) && (e.button.x<=990) && (e.button.y>=658) && (e.button.y<=695)){
                        goto BEGIN;
                    }
                     //NEW WAVE : create monster
                    if( (e.button.x>=10) && (e.button.x<=240) && (e.button.y>=560) && (e.button.y<=600)){
                        new_wave(wave, numberWave, textureMonster, enter_x, enter_y, monsters);
                     }
                
                }
                    break;

                /* Key */
                case SDL_KEYDOWN:
               
                    
                    if (e.key.keysym.sym == 27) {
                        loop=false;
                        break;
                    }
                    else if (e.key.keysym.sym == 'q') {
                        loop=false;
                        break;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    //delete_all(towers, monsters, supr, buildings);
     /* Free SDL resources */
    SDL_DestroyWindow(window);
    delete_image(img_map);
    glDeleteTextures(1, &textureFire);
    glDeleteTextures(1, &textureTower);
    glDeleteTextures(1, &textureMonster);
    glDeleteTextures(1, &textureRadar);
    glDeleteTextures(1, &textureFactory);
    glDeleteTextures(1, &textureMunitions);
    glDeleteTextures(1, &textureMap);
    glDeleteTextures(1, &textureWin);
    glDeleteTextures(1, &textureGO);
    glDeleteTextures(1, &textureMoney);
    SDL_Quit();

    return EXIT_SUCCESS;
}
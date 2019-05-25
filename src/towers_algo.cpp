#include "towers_algo.h"
#include "towers_graphic.h"
#include "map_graphic.h"
#include "player.h"

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

Tower::Tower(int Cx, int Cy, GLuint newTexture, Image* I, bool &valid_zone)
{
    cost = 2;
    pace = 0.5;
    x=Cx;
    y=Cy;
    texture=newTexture;

    int i,j;
    Pixel color = create_pixel(110,0,110);
    to_ppm_coordinates(Cx, Cy);
    valid_zone = true; 

    //randomly choose a type for the tower
    int random = rand() % 20; //from 0 to 19
    if (random >=0 && random <= 5) type = 0;  
    if (random >5 && random <= 10) type = 1;    
    if (random >10 && random <= 15) type = 2;    
    if (random >15 && random <= 19) type = 3;  
    set_type(type);

    //checking whether the tower is far enough from other elements
    //to do this we check if a slightly larger circle's perimeter is in a constructible zone
    //when i=j=0 we're checking whether the centre is in a constructible zone
    for (i=-11; i<11; i=i+11)
    {
        for (j=-11; j<11; j=j+11)
        {
            if ( sqrt( i*i + j*j ) <= 11 )
            {
                if ( ( type_position(Cx + i, Cy + j, I) != 1) || ( type_position(Cx - i, Cy + j, I) != 1) 
                        || ( type_position(Cx + i, Cy - j, I) != 1) ) 
                {
                    valid_zone = false;
                    break;
                }
                //set_pixel(I, color, Cx+i,Cy+j);
            }
        }
    }

    //check if the tower isn't outside of the map
    if (Cx > 495  || Cy > 295 || Cx < 5 || Cy < 5) valid_zone = false;

    if (valid_zone)
    {
        //cout<<"Valid zone for tower to be built"<<endl;

        //to create 20px circles
        for (i=-10; i<10; i++)
        {
            for (j=-10; j<10; j++)
            {
                if ( sqrt( i*i + j*j ) <= 10 ) set_pixel(I, color, Cx + i, Cy + j);
            }
        }
    } 

    //just to check if towers are well represented as circles at the right place
    //save(I, "doc/lolol.ppm"); 
}

Tower::~Tower()
{
   
}

int Tower::get_power()
{
    return power;
}

void Tower::set_power(int pow)
{
    power = pow;
}

int Tower::get_range()
{
    return range;
}

void Tower::set_range(int rng)
{
    range = rng;
}

int Tower::get_type()
{
    return type;
}

void Tower::set_type(int type)
{
    switch(type) 
    { 
        case 0: //high power and low pace 
            power = 4;
            pace = 6;
            range = 60;
            break; 
        case 1: //low range, low power, high pace
            range = 40;
            power = 1;
            pace = 1;
            break; 
        case 2: //low power, low range, high pace
            power = 1;
            range = 30;
            pace = 6;
            break; 
        case 3: //high range, high pace, low power
            power = 2;
            range = 60;
            pace = 7;
            break; 
    }
}

int Tower::get_cost()
{
    return cost;
}

void Tower::set_cost(int c)
{
    cost = c;
}

int Tower::get_pace()
{
    return pace;
}

void Tower::set_pace(int p)
{
    pace = p;
}
int Tower::get_x()
{
    return x;
}

void Tower::set_x(int Cx)
{
    x = Cx;
}
int Tower::get_y()
{
    return y;
}

void Tower::set_y(int Cy)
{
    y = Cy;
}

GLuint Tower::get_texture(){
    return texture;
}

void Tower::set_texture(GLuint newTexture){
    texture=newTexture;
}

/**********************************OTHER FUNCTIONS*********************************************/



void to_ppm_coordinates(int &x, int &y)
{
    //to go from (20,65) to (0,0)
    //to go from (750,540) SDL to ppm coordinates (500,300) 
    x = (x-20) * 0.68;
    y = (y-65) * 0.63;
}

void to_sdl_coordinates(int &x, int &y)
{
    //to go from ppm coordinates (500,300) to (750,540) SDL 
    x = 1.5*x + 20;
    y = 0.95*y + 65;
}

//where towers kill monsters based on: their range, monsters' resistance, monsters' life points
void tower_attacks_monsters(bool &success, int &money, int &times_shot, int &loopMonster, Tower* tower, 
    Monster* monster, vector<Monster*> &monsters, vector<Monster*> &supr, Player &player, int &nb_wave)
{
    success = false;
    int tower_power = tower->get_power();
    int range = tower->get_range();
    int resistance_monster = monster->get_resistance();

    int monster_x = monster->get_x();
    int monster_y = monster->get_y();

    int compareX= tower->get_x()- monster_x;
    int compareY= tower->get_y()- monster_y;

    //if there's a monster in the tower's range 
    if(monsters.size()>0 && compareX<range && compareY<range){

        success = true;
        times_shot++;
        
        //first we need to shoot the monster until he loses all resistance
        if (times_shot < resistance_monster && resistance_monster > 0) 
        {
            monster->set_resistance(resistance_monster - 1);
        }
        else
        {
            if (monster->get_life_points() > 0){
                //this is where the tower's power has an effect
                int life = monster->get_life_points() - tower_power;
                if (life <= 0) life = 0;
                monster->set_life_points(life);
                times_shot=0;
            }
            if (monster->get_life_points() == 0){
                cout<<"Monster killed!"<<endl;
                money = player.get_money() + 2*nb_wave;
                player.set_money(money);
                monster->set_life_points(-1);
            }
        }
    }
    loopMonster+=1;
}


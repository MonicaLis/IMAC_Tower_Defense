#include "towers_algo.h"
#include "towers_graphic.h"
#include "map_graphic.h"
#include "player.h"

#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

    //type: 0-red, 1-green, 2-yellow, 3-blue
    int power, range, type, cost,x,y; //puissance, portée
    float pace; //cadence
    GLuint texture;

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

    //checking whether the tower is far enough from other elements
    //to do this we check if a slightly larger circle's perimeter is in a constructible zone
    //when i=j=0 we're checking whether the centre is in a constructible zone
    for (i=-12; i<12; i=i+12)
    {
        for (j=-12; j<12; j=j+12)
        {
            if ( sqrt( i*i + j*j ) <= 12 )
            {
                if ( type_position(Cx + i, Cy + j, I) < 1 ) 
                {
                    valid_zone = false;
                    break;
                }
                set_pixel(I, color, Cx+i,Cy+j);
            }
        }
    }

    //check if the tower isn't outside of the map
    if (Cx > 500) valid_zone = false;

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

    //randomly choose a type for the tower
    int random = rand() % 20; //from 0 to 19
    if (random >=0 && random <= 5) type = 0;  
    if (random >5 && random <= 10) type = 1;    
    if (random >10 && random <= 15) type = 2;    
    if (random >15 && random <= 19) type = 3;  
    set_type(type);

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
            power = 30;
            pace = 1;
            range = 200;
            break; 
        case 1: //low range, low power, high pace
            range = 190;
            power = 15;
            pace = 3;
            break; 
        case 2: //low power, low range, high pace
            power = 20;
            range = 150;
            pace = 3;
            break; 
        case 3: //high range, high pace, low power
            power = 15;
            range = 260;
            pace = 2;
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

float Tower::get_pace()
{
    return pace;
}

void Tower::set_pace(float p)
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

/**********************************OTHER FUNCTIONS*********************************************/


void set_texture(GLuint newTexture){
    texture=newTexture;
}

void to_ppm_coordinates(int &x, int &y)
{
    //to go from (650,650) SDL to ppm coordinates (500,300) 
    x = (x - 50) * 0.83;
    y = (y - 50) * 0.5;
}

void to_sdl_coordinates(int &x, int &y)
{
    //to go from ppm coordinates (500,300) to (650,650) SDL 
    x = 1.2*x + 50;
    y = y*2 + 50;
}

//where towers kill monsters based on: their range, monsters' resistance, monsters' life points
void tower_attacks_monsters(bool &success, int &money, int &time, int &loopMonster, Tower* tower, Monster* monster, vector<Monster*> &monsters, vector<Monster*> &supr, Player &player)
{
    time+=1;
    success = false;
    int monster_x = monster->get_x();
    int monster_y = monster->get_y();
    to_sdl_coordinates(monster_x, monster_y);

    int compareX= tower->get_x()- monster_x;
    int compareY= tower->get_y()- monster_y;
    int range = tower->get_range();
    int resistance_monster = monster->get_resistance();

    if(monsters.size()>0 && compareX<range && compareY<range && time>resistance_monster){
        success = true;
        if (monster->get_life_points() <= 0){
            money = player.get_money()+3;
            player.set_money(money);
            cout << "Available money:"<<money<<endl;
            supr.push_back(monster);
            monsters.erase(monsters.begin()+loopMonster);
        }
        if (monster->get_life_points()>0){
            int life = monster->get_life_points()-2;
            monster->set_life_points(life);
            cout << "Monster's life points : "<<life<<endl;
            time=0;
        }
    }
    loopMonster+=1;
}
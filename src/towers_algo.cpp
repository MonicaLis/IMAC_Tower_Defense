#include "towers_algo.h"
#include "towers_graphic.h"


#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

    //type: 0-red, 1-green, 2-yellow, 3-blue
    int power, range, type, cost,x,y; //puissance, portée
    float pace; //cadence
    GLuint texture;

Tower::Tower(int Cx, int Cy,GLuint newTexture)
{
    power = 3;
    range = 3;
    type = 0;
    cost = 2;
    pace = 0.5;
    x=Cx;
    y=Cy;
    texture=newTexture;
    cout << "Tower créé !"<< endl;
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
            power = 7;
            pace = 0.1;
            break; 
        case 1: //low range, low power, high pace
            range = 2;
            power = 2;
            pace = 0.8;
            break; 
        case 2: //low power, low range, high pace
            power = 2;
            range = 3;
            pace = 0.7;
            break; 
        case 3: //high range, high pace, low power
            power = 1;
            range = 5;
            pace = 0.8;
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

GLuint get_texture(){
    return texture;
}

void set_texture(GLuint newTexture){
    texture=newTexture;
}
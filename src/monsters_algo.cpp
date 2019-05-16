#include "monsters_algo.h"

#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

    //type: 0-red, 1-green, 2-yellow, 3-blue
    int life_points, resistance, speed,xM,yM; //puissance, portée
    GLuint textureM;

Monster::Monster(int Cx, int Cy,GLuint newTexture)
{
    life_points = 5;
    resistance = 1;
    xM=Cx;
    yM=Cy;
    textureM=newTexture;
    cout << "Monster created !"<< endl;
    int random = rand() % 5;
    if (random % 2 == 0) choose_path = 1;
    else choose_path = 0;
    random = rand() % 3;
    if (random % 2 == 0) 
    {   
        type = 1; 
        speed = 2;
    }
    else 
    {
        type = 2; 
        speed = 1;
    }
}

Monster::~Monster()
{

}

int Monster::get_type()
{
    return type;
}

int Monster::get_path()
{
    return choose_path;
}


int Monster::get_life_points()
{
    return life_points;
}

void Monster::set_life_points(int points)
{
    life_points = points;
}

int Monster::get_resistance()
{
    return resistance;
}

void Monster::set_resistance(int res)
{
    resistance = res;
}

int Monster::get_speed()
{
    return speed;
}

void Monster::set_speed(int sp)
{
    speed = sp;
}
int Monster::get_x()
{
    return xM;
}

void Monster::set_x(int Cx)
{
    xM = Cx;
}
int Monster::get_y()
{
    return yM;
}

void Monster::set_y(int Cy)
{
    yM = Cy;
}

GLuint Monster::get_texture(){
    return textureM;
}

void Monster::set_texture(GLuint newTexture){
    textureM=newTexture;
}
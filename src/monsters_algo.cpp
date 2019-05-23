#include "monsters_algo.h"

#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

Monster::Monster(int Cx, int Cy,GLuint newTexture, int n_wave)
{
    life_points = 1;
    resistance = 1 + n_wave;
    xM=Cx;
    yM=Cy;
    textureM=newTexture;
    cout << "Monster created ! wave ("<< n_wave << ")" << endl;
    int random = rand() % 5;
    if (random % 2 == 0) choose_path = 1;
    else choose_path = 0;
    random = rand() % 10;
    if (random % 3 == 0) //less chances of getting a super quick monster
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

void new_wave(bool &wave, int &numberWave, GLuint texture, int x, int y, vector<Monster*> &monsters)
{
    wave = true;
    numberWave+=1;
    cout<<"New wave no "<<numberWave<<endl;
    int random, i;
    for (i=0; i<10; i++)
    {
        Monster* newMonster= new Monster(x +rand()%20, y+ rand()%15, texture, numberWave);
        monsters.push_back(newMonster);
        int life = newMonster->get_life_points();
        newMonster->set_life_points(life*numberWave);
    }
}
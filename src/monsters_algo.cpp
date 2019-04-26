#include "monsters_algo.h"

#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

Monster::Monster()
{
    life_points = 2;
    resistance = 1;
    speed = 1;
    dead = false;
}

Monster::~Monster()
{
    life_points = 0;
    resistance = 0;
    speed = 0;
    dead = true;
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
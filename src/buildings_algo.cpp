#include "buildings_algo.h"
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>
#include "map_graphic.h"
#include "towers_algo.h"


Building::Building(GLuint texture_t, int type_t, int x_t, int y_t, Image* I, bool &valid_zone)
{
    x = x_t;
    y = y_t;
    type = type_t; 
    switch(type)
    {
        case 0: //radar
            range = 200;
            cost = 5;
            break; 
        case 1: //factory
            range = 220;
            cost = 2;
            break;
        case 2: //munitions
            range = 180;
            cost = 4;
    }
    texture = texture_t;

    int i, j;
    Pixel color = create_pixel(130, 50, 100);
    to_ppm_coordinates(x_t, y_t);
    valid_zone = true;

    //checking whether the building is far enough from other elements
    //to do this we check if a slightly larger circle's perimeter is in a constructible zone
    //when i=j=0 we're checking whether the centre is in a constructible zone
    for (i=-15; i<15; i=i+15)
    {
        for (j=-15; j<15; j=j+15)
        {
            if ( sqrt( i*i + j*j ) <= 15 )
            {
                if ( type_position(x_t + i, y_t +j, I) < 1 )
                {
                    valid_zone = false;
                    break;
                } 
                //set_pixel(I, color, x_t+i, y_t+j);
            }
        }
    }

    //check if the building isn't outside of the map
    if (x_t > 500  || y_t > 300) valid_zone = false;

    if (valid_zone)
    {
        //to create squares on ppm
        for (i=-15; i<15; i++)
        {
            for (j=-15; j<15; j++)
            {
                set_pixel(I, color, x_t + i, y_t + j);
            }
        }
    } 

    //just to check if buildings are well represented as squares at the right place
    //save(I, "doc/lolol.ppm"); 
}

Building::~Building()
{

}

int Building::get_type()
{
    return type;
}

void Building::set_type(int type_t)
{
    type = type_t;
}

int Building::get_range()
{
    return range;
}

void Building::set_range(int rng)
{
    range = rng;
}

int Building::get_cost()
{
    return cost;
}

void Building::set_cost(int cost_t)
{
    cost = cost_t;
}

void Building::impact(Tower* tower)
{
    int compareX = x - tower->get_x();
    int compareY = y - tower->get_y();
    int tower_range, tower_power, tower_pace;

    if (compareX <= range && compareY <= range)
    {
        switch (type)
        {
            case 0: //radar
                tower_range = tower->get_range();
                tower->set_range(1.25 * tower_range);
                cout<<"tower's range went from "<<tower_range<<" to "<<tower->get_range()<<endl;
                break; 
            case 1: //factory
                tower_power = tower->get_power();
                tower->set_power(1.25 * tower_power);
                cout<<"tower's power went from "<<tower_power<<" to "<<tower->get_power()<<endl;
                break;
            case 2: //munitions
                tower_pace = tower->get_pace();
                tower->set_pace(1.25 * tower_pace); 
                cout<<"tower's pace went from "<<tower_pace<<" to "<<tower->get_pace()<<endl;
                break;   
        }
    }
}

void add_building(vector<Building*> &buildings, vector<Tower*> &towers, Building* newBuilding)
{
    buildings.push_back(newBuilding);
    for (Tower* tower : towers) {
        newBuilding->impact(tower);
    }
}
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
            range = 20;
            cost = 5;
            break; 
        case 1: //factory
            range = 15;
            cost = 2;
            break;
        case 2: //munitions
            range = 18;
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
    for (i=-range; i<range; i=i+range)
    {
        for (j=-range; j<range; j=j+range)
        {
            if ( sqrt( i*i + j*j ) <= range )
            {
                if ( type_position(x_t + i, y_t +j, I) < 1 )
                {
                    valid_zone = false;
                    break;
                } 
                set_pixel(I, color, x_t+i, y_t+j);
            }
        }
    }

    //check if the building isn't outside of the map
    if (x_t > 500) valid_zone = false;

    if (valid_zone)
    {
        //to create squares on ppm
        for (i=-range; i<range; i++)
        {
            for (j=-range; j<range; j++)
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

float Building::get_range()
{
    return range;
}

void Building::set_range(float rng)
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



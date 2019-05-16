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


Building::Building(GLuint texture_t, int type_t, int x_t, int y_t, Image* I, int &valid_zone)
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
            range = 10;
            cost = 4;
    }
    texture = texture_t;

    int i, j;
    Pixel color = create_pixel(30, 50, 200);

    //checking whether the building is far enough from other elements
    //to do this we check if a slightly larger circle's perimeter is in a constructible zone
    //when i=j=0 we're checking whether the centre is in a constructible zone
    for (i=-range; i<range; i=i+range)
    {
        for (j=-range; j<range; j=j+range)
        {
            if ( sqrt( i*i + j*j ) <= range )
            {
                if ( type_position(x + i, y +j, I) < 1 ) valid_zone = false;
                set_pixel(I, color, x+i,y+j);
            }
        }
    }

/*
    //check if the building isn't outside of the map
    if (x > 500) valid_zone = false;

    if (valid_zone)
    {
        cout<<"Valid zone for the building to be built"<<endl;

        //to create 20px circles
        for (i=-10; i<10; i++)
        {
            for (j=-10; j<10; j++)
            {
                if ( sqrt( i*i + j*j ) <= 10 ) set_pixel(I, color, Cx + i, Cy + j);
            }
        }
    } 
    else cout<<"Invalid zone for tower to be built, try again"<<endl; */

    //just to check if towers are well represented as circles at the right place
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



/*
les centrales et les installations. Graphique-
ment, les bâtiments sont également représentés par des sprites mais à base carrée d’au moins vingt
pixels (taille laissée à votre appréciation). Notez que ces carrés ont une largeur et une hauteur impaire
ce qui permet de définir sans ambiguı̈té un centre à celui-ci. C’est à partir de ce centre que les portées
sont calculées.
Comme pour les tours, on doit veiller à ce que le placement des bâtiments soient valides c’est à dire
ne recouvre pas ni un chemin (segment large de 30 pixel), ni une tour (disque). Mêmes remarques que
pour le placement des tours : on souhaite éviter un test pixel à pixel.
*/
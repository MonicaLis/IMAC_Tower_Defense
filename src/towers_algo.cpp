#include "towers_algo.h"
#include "towers_graphic.h"
#include "map_graphic.h"

#include <stdlib.h>
#include <iostream>

using namespace std;


/**********************************STRUCTURE METHODS*********************************************/

    //type: 0-red, 1-green, 2-yellow, 3-blue
    int power, range, type, cost,x,y; //puissance, portée
    float pace; //cadence
    GLuint texture;

Tower::Tower(int Cx, int Cy,GLuint newTexture, Image* I, bool &valid_zone)
{
    power = 3;
    range = 3;
    type = 0;
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
                if ( type_position(Cx + i, Cy +j, I) < 1 ) valid_zone = false;
                set_pixel(I, color, Cx+i,Cy+j);
            }
        }
    }

    //check if the tower isn't outside of the map
    if (Cx > 500) valid_zone = false;

    if (valid_zone)
    {
        cout<<"Valid zone for tower to be built"<<endl;

        //to create 20px circles
        for (i=-10; i<10; i++)
        {
            for (j=-10; j<10; j++)
            {
                if ( sqrt( i*i + j*j ) <= 10 ) set_pixel(I, color, Cx + i, Cy + j);
            }
        }
    } 
    else cout<<"Invalid zone for tower to be built, try again"<<endl;

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

void to_ppm_coordinates(int &x, int &y)
{
    //to go from (650,650) SDL to ppm coordinates (500,300) 
    x = (x - 50) * 0.83;
    y = (y - 50) * 0.5;
}
#ifndef TOWERS_ALGO_H
#define TOWERS_ALGO_H

#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

#include "map_graphic.h"

using namespace std;

typedef struct Tower{

private:
    //type: 0-red, 1-green, 2-yellow, 3-blue
    int power, range, type, cost, x, y; //puissance, portée
    GLuint texture;
    float pace; //cadence, 10e de sec

public:
    Tower(int Cx, int Cy,GLuint newTexture, Image* I, bool &valid_zone);
    ~Tower();
    int get_power();
    void set_power(int pow);
    int get_range();
    void set_range(int rng);
    int get_type();
    void set_type(int type);
    int get_cost();
    void set_cost(int c);int get_x();
    void set_x(int Cx);
    int get_y();
    void set_y(int Cy);
    GLuint get_texture();
    void set_texture(GLuint newTexture);
    void drawTower();
    float get_pace();
    void set_pace(float p);
    
}Tower;

void to_ppm_coordinates(int &x, int &y);


#endif 
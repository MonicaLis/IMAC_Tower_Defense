#ifndef MONSTERS_ALGO_H
#define MONSTERS_ALGO_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>


using namespace std;

typedef struct Monster{

private:
    int life_points, resistance, speed,xM,yM;
    bool dead;
    GLuint textureM;
    bool choose_path; 

public:
    Monster(int Cx, int Cy,GLuint newTexture);
    ~Monster();
    int get_life_points();
    void set_life_points(int points);
    int get_path();
    int get_resistance();
    void set_resistance(int res);
    int get_speed();
    void set_speed(int sp);
    int get_x();
    void set_x(int Cx);
    int get_y();
    void set_y(int Cy);
    GLuint get_texture();
    void set_texture(GLuint newTexture);
    void drawMonster();
    void move(int initial_pos_x, int before_x, int before_y, int after_x, int after_y, int destination1_x,int destination2_x);
}Monster;


#endif 
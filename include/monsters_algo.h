#ifndef MONSTERS_ALGO_H
#define MONSTERS_ALGO_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>
#include <vector>


using namespace std;

typedef struct Monster{

private:
    int life_points, resistance, speed, xM, yM;
    GLuint textureM;
    bool choose_path; 
    int type; //type 1 is slower than type 2

public:
    Monster(int Cx, int Cy,GLuint newTexture, int n_wave);
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
    int get_type();
    void move(int initial_pos_x, int before_x, int before_y, int after_x, int after_y, int destination1_x,int destination2_x);
}Monster;


void new_wave(bool &wave, int &numberWave, GLuint texture, int x, int y, vector<Monster*> &monsters);


#endif 
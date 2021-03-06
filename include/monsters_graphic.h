#ifndef MONSTERS_GRAPHIC_H
#define MONSTERS_GRAPHIC_H

#include "monsters_algo.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

GLuint initTextureMonster();
void drawMonster();
void rid_monsters(int &i, Monster* monster, vector<Monster*> &monsters, vector<Monster*> &supr);


#endif 

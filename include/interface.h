#ifndef INTERFACE_H
#define INTERFACE_H
 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "towers_algo.h"
#include "buildings_algo.h"
#include "monsters_algo.h"


#include <vector>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

GLuint display_win();
void drawWin(GLuint texture);
GLuint display_gameover();
void drawGO(GLuint texture);
void delete_all(vector<Tower*> &towers, vector<Monster*> &monsters, vector<Monster*> &supr, vector<Building*> &buildings);


 
#endif 

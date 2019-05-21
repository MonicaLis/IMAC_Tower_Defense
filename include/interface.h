#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

GLuint display_money(int i);
void drawMoney(GLuint texture);
GLuint display_win();
void drawWin(GLuint texture);
GLuint display_gameover();
void drawGO(GLuint texture);



#endif 

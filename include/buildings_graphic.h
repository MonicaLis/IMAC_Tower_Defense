#ifndef BUILDINGS_GRAPHIC_H
#define BUILDINGS_GRAPHIC_H

#include "buildings_algo.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

GLuint initTextureBuilding( const char* filename);
void choose_building(int x, int y, int &type_building);
bool after_chose_building(Building* building, bool valid_zone, Player* player, int &money);


#endif 

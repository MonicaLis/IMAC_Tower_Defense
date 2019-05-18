#ifndef BUILDINGS_ALGO_H
#define BUILDINGS_ALGO_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "map_graphic.h"
#include "towers_algo.h"
#include <vector>


typedef struct Building{

private:
    int type; //0 for a radar, 1 for an armamant factory, 2 for munitions
    float range; //(portée)
    int cost;
    GLuint texture;
    int x, y;

public:
    Building(GLuint texture_t, int type_t, int x_t, int y_t, Image* I, bool &valid_zone);
    ~Building();
    int get_type();
    void set_type(int type_t);
    float get_range();
    void set_range(float rng);
    int get_cost();
    void set_cost(int cost_t);
    void drawBuilding();
    void impact(Tower* tower);
    
}Building;

void add_building(vector<Building*> &buildings, vector<Tower*> &towers, Building* newBuilding);


#endif

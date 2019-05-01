#ifndef MAP_GRAPHIC_H
#define MAP_GRAPHIC_H

#include "map_algo.h"


/*******************************FROM C PROJECT**********************************/


typedef struct Pixel{
    unsigned char red, green, blue;
}Pixel;

typedef struct Image{

        Pixel* data;
        int height;
        int width;

}Image;


//functions from our Minigimp project
Image* load(const char* filename);
int save(Image* I,const char* filename);

void delete_image(Image* I);
Image* create_image(int w, int h);

Pixel get_pixel(int x, int y, Image* I);
void set_pixel(Image* I, Pixel p, int i, int j);
void fill_image(Image* I, unsigned char r, unsigned char g, unsigned char b);
void display_pixel(Pixel p);
Pixel create_pixel(unsigned char r, unsigned char g, unsigned char b);  
bool are_they_equal(Pixel pi1, Pixel pi2);


/*******************************NEW FUNCTIONS**********************************/

void create_map_ppm(Graph graph);
void draw_line_ppm(int x0, int y0, int x1, int y1, Image* I);

#endif 
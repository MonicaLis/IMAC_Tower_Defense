#include "map_graphic.h"

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <list>

using namespace std;


void drawline(int x0, int y0, int x1, int y1, Image* I)
{
    int dx, dy, p, x, y;
    Pixel white = create_pixel(255,255,255);
 
	dx=x1-x0;
	dy=y1-y0;
 
	x=x0;
	y=y0;
 
	p=2*dy-dx;
 
	while(x<x1)
	{
		if(p>=0)
		{
            set_pixel(I, white, x, y);
			y=y+1;
			p=p+2*dy-2*dx;
		}
		else
		{
			set_pixel(I, white, x, y);
			p=p+2*dy;
		}
		x=x+1;
	}
}


void create_map_ppm(Graph graph)
{
    //we create a 500x250 image of color (120,180,180)
    Image* img_map = create_image(500,250);
    fill_image(img_map, 120, 180, 180);

    int i,j,k;
    Pixel black = create_pixel(0,0,0);
    Pixel red = create_pixel(200,0,0);
    Pixel green = create_pixel(0,200,0);
    Pixel white = create_pixel(255,255,255);
    Pixel color;
    

    //PATHS
    //from N0 to N3: (10,20) to (200,103)
    j = 20;
    int descent = 0;
    for (i=10; i<200; i++)
    {
        for (k=-5; k<=5; k++)
        {
            set_pixel(img_map, white, i + k, j + k);
        }
        descent ++;
        if ( (descent == 2) && (j<104))
        {   
            j++; 
            descent = 0;
        }
        cout << j << endl;
    }

    //from N3 to N2: (200,103) to (300,103)
    for (i=200; i<300; i++)
    {
        for (k=-5; k<=5; k++)
        {
            set_pixel(img_map, white, i + k, j + k);
        }
    }

    //from N2 to N1: (300,103) to (454,103)
    for (i=300; i<454; i++)
    {
        for (k=-5; k<=5; k++)
        {
            set_pixel(img_map, white, i + k, j + k);
        }
    }

    //from N4 to N2: (200,206) to (300,103)
    j = 206;
    for (i=200; i<300; i++)
    {
        for (k=-5; k<=5; k++)
        {
            set_pixel(img_map, white, i + k, j + k);
        }
        j--;
        cout << j << endl;
    }

    //NODES
    for (j=0; j<5; j++)
    {
        int coordinate_x = graph.get_node(j).get_coordinates().get_p_x();
        int coordinate_y = graph.get_node(j).get_coordinates().get_p_y();

        switch(graph.get_node(j).get_nature()) 
        { 
            case 1: 
                color = green;
                break; 
            case 2: 
                color = red;
                break; 
            default:
                color = black;
        }

        //we want to draw 30px-diameter circles around the nodes
        for (i=-15; i<15; i++)
        {
            for (k=-15; k<15; k++)
            {
                if ( sqrt( i*i + k*k ) < 15 )
                {
                    set_pixel(img_map, color, coordinate_x + i, coordinate_y + k);
                }
            }
        } 

    } 
    
    save(img_map, "doc/MAP.ppm");
}

/**********************************FUNCTIONS FROM C PROJECT*********************************************/

Image* load(const char* filename)
{
    char buff[16];
    Image *img;
    FILE *fp;
    int c, rgb_comp_color;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }
    
    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }
    
    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }
    
    //alloc memory form image
    img = (Image *)malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }
    
    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }
    
    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }
    
    //check rgb component depth
    if (rgb_comp_color!= 255) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }
    
    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (Pixel*)malloc(img->width * img->height * 3 * sizeof(unsigned char));
    
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    //read pixel data from file
    if (fread(img->data, 3 * img->width, img->height, fp) != img->height) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }
    
    fclose(fp);
    return img;
}
    





/*this function opens a file, prints:
    P6
    x y
    max
 and then all the other pixel information from image I into the file.
 */
int save(Image* I, const char* filename)
{
    FILE* fp = fopen(filename, "wb"); //write, binary
    if (!fp)
    {
        printf("failed to save image\n");
        return -1;
    }
    
    //write the header file
    //image format
    fprintf(fp, "P6\n");
    
    //comments
    //fprintf(fp, "# Created by an IMAC1 student\n");
    
    //image size
    fprintf(fp, "%d %d\n",I->width,I->height);
    
    // rgb component depth
    fprintf(fp, "255\n");
    
    //pixel data
    fwrite(I->data, 3 * I->width, I->height, fp);
    
    fclose(fp);
    printf("image saved\n");
    return 0;
}

void delete_image(Image* I)
{
    if (I)
    {
        free(I->data);
        free(I);
        printf("image deleted\n");
    }
}

Image* create_image(int w, int h)
{
    Image* I = (Image*)malloc(sizeof(Image));
    if (!I)
    {
        printf("allocation of memory for the image failed\n");
    }
    I->width = w;
    I->height = h;
    //3 times unsigned char because 3 RGB components per pixel
    I->data = (Pixel*)malloc(I->width * I->height * 3 * sizeof(unsigned char));
    if (!I->data)
    {
        printf("allocation of memory for image->data failed\n");
    }
    return I;
}


Pixel get_pixel(int x, int y, Image* I)
{
    return I->data[I->width*y+x]; //because we count pixels from the left to the right, and from the top to bottom
}


void set_pixel(Image* I, Pixel p, int i, int j)
{
    I->data[I->width*j+i].red= p.red;
    I->data[I->width*j+i].green= p.green;
    I->data[I->width*j+i].blue= p.blue;
}


void fill_image(Image* I, unsigned char r, unsigned char g, unsigned char b)
{
    int i,j;
    Pixel p = create_pixel(r,g,b);
    for (j=0; j<I->height; j++)
    {
        for (i=0; i<I->width; i++)
        {
            set_pixel(I,p,i,j);
        }
    }
}

Pixel create_pixel(unsigned char r, unsigned char g, unsigned char b)
{
    Pixel p;
    p.red = r;
    p.green = g;
    p.blue = b;
    return p;
}


void display_pixel(Pixel p)
{
    printf("red: %u green: %u blue: %u\n", p.red, p.green, p.blue);
}

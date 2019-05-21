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

#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

const int X_DIMENSION_IMG_MAP = 500;
const int Y_DIMENSION_IMG_MAP = 300;


/**********************************FUNCTIONS TO CREATE PPM MAP*********************************************/

//BRESENHAM ALGO used here again
void draw_line_ppm(int x0, int y0, int x1, int y1, Image* I)
{
    int dx, dy; //width and height of bounding box
    int x, y; //current point
    int sx, sy; //-1 or 1
    int err, e2; //loop-carried value and temporary variable
    int right, down; //bool
    Pixel white = create_pixel(255,255,255);
    int k, j;

    dx = x1 - x0;
    right = dx > 0;
    if (!right) dx = -dx;
    dy = y1 - y0;
    down = dy > 0;
    if (down) dy = -dy;
    err = dx + dy;
    x = x0;
    y = y0;

    for (;;) //loops forever
    {
        //create a larger path
        for (k=-14; k<=14; k++)
        {
            for (j=-14; j<=14; j++)
            {
                set_pixel(I, white, x+k, y+j);
            }
        }
        if ((x == x1) && (y == y1)) break; //reached the end
        e2 = err << 1; //err*2
        if (e2 > dy)
        {
            err += dy;
            if (right) x++;
            else x--;
        }
        if (e2 < dx)
        {
            err += dx;
            if (down) y++;
            else y--;
        }
    }
}


Image* create_map_ppm(Graph graph)
{
    //we create a 500x300 image of color (120,180,180)
    Image* img_map = create_image(X_DIMENSION_IMG_MAP, Y_DIMENSION_IMG_MAP);
    fill_image(img_map, 120, 180, 180);

    int i,j,k;
    Pixel black = create_pixel(0,0,0);
    Pixel red = create_pixel(200,0,0);
    Pixel green = create_pixel(0,200,0);
    Pixel white = create_pixel(255,255,255);
    Pixel color;
    

    //PATHS

    //from N0 to N3: (10,20) to (200,103)
    draw_line_ppm(graph.get_node(0).get_coordinates().get_p_x(), 
                    graph.get_node(0).get_coordinates().get_p_y(),
                    graph.get_node(3).get_coordinates().get_p_x(), 
                    graph.get_node(3).get_coordinates().get_p_y(), img_map);

    //from N3 to N2: (200,103) to (300,103)
    draw_line_ppm(graph.get_node(3).get_coordinates().get_p_x(), 
                    graph.get_node(3).get_coordinates().get_p_y(),
                    graph.get_node(2).get_coordinates().get_p_x(), 
                    graph.get_node(2).get_coordinates().get_p_y(), img_map);

    //from N2 to N1: (300,103) to (454,103)
    draw_line_ppm(graph.get_node(2).get_coordinates().get_p_x(), 
                    graph.get_node(2).get_coordinates().get_p_y(),
                    graph.get_node(1).get_coordinates().get_p_x(), 
                    graph.get_node(1).get_coordinates().get_p_y(), img_map);

    //from N0 to N4: (10,20) to (200,280)
    draw_line_ppm(graph.get_node(0).get_coordinates().get_p_x(), 
                    graph.get_node(0).get_coordinates().get_p_y(),
                    graph.get_node(4).get_coordinates().get_p_x(), 
                    graph.get_node(4).get_coordinates().get_p_y(), img_map);

    //from N4 to N2: (200,280) to (300,103)
    draw_line_ppm(graph.get_node(4).get_coordinates().get_p_x(), 
                    graph.get_node(4).get_coordinates().get_p_y(),
                    graph.get_node(2).get_coordinates().get_p_x(), 
                    graph.get_node(2).get_coordinates().get_p_y(), img_map);

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
                if ( sqrt( i*i + k*k ) <= 15 )
                {
                    set_pixel(img_map, color, coordinate_x + i, coordinate_y + k);
                }
            }
        } 

    } 
    
    save(img_map, "doc/MAP.ppm");
    return img_map;
}

/**********************************FUNCTIONS TO DISPLAY MAP IN WINDOW***************************************/



GLuint init_map()
{
    /* loading image */
    const char image_path[] = "images/ui.png";
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(image_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
    if (nullptr == image)
    {
        cerr<<"issue loading image of map"<<endl;
    }
    // Autorisation de l'affichage des textures
    glEnable(GL_TEXTURE_2D);

    /* Initialisation de la texture */
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return texture_id;
}

void drawMap(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        glTranslated(500,350,0);
        glScalef(1000,700,0);
        glRotatef(180,1,0,0);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(-0.5f, -0.5f);   // bas gauche
            glTexCoord2f(1, 1); glVertex2f(0.5f, -0.5f);    // bas droite
            glTexCoord2f(1, 0); glVertex2f(0.5f, 0.5f);     // haut droite
            glTexCoord2f(0, 0); glVertex2f(-0.5f, 0.5f);    // haut gauche
        glEnd();
    glPopMatrix();
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0); 
}

/**********************************FUNCTIONS TO DISPLAY PATH IN WINDOW****************************************/

//if 2: building, if 1: constructible zone, if 0: path
int type_position(int x, int y, Image* I)
{
    Pixel color_construct = create_pixel(120, 180, 180);
    Pixel color_tower = create_pixel(110,0,110);
    if ( are_they_equal( get_pixel(x, y, I), color_construct) ) return 1;
    if ( are_they_equal( get_pixel(x, y, I), color_tower) ) return 2;
    else return 0;
}

//this was used at the beginning but now we created a path on the map outside of the program
GLuint initTexturePath(){
    /* Chargement de l'image */
    const char image_path[] = "images/brick.png";
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(image_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
  
    // Autorisation de l'affichage des textures
    glEnable(GL_TEXTURE_2D);

    /* Initialisation de la texture */
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    return texture_id;
}

void draw_path(Image* I)
{
    int i, j;
    GLuint texturePath = initTexturePath();
    glBindTexture(GL_TEXTURE_2D, texturePath);

    //+40 so we don't load the image on every pixel otherwise it doesn't look like anything and it takes ages to load
    for (i=0; i<X_DIMENSION_IMG_MAP; i=i+15)
    {
        for (j=0; j<Y_DIMENSION_IMG_MAP; j=j+15)
        {
            //if it's not a constructible zone nor a tower we can draw a path
            if( type_position(i, j, I) == 0)
            {
                glPushMatrix();
                    glLoadIdentity();
                    //we do *1.4 to scale to space image
                    // + so it's centered
                    glTranslated(i*1.4 + 40,j*1.4 + 100,0); 
                    glScalef(20,20,0);
                    glBegin(GL_QUADS);
                        glTexCoord2f(0, 1); glVertex2f(-0.5f, -0.5f);   // down left
                        glTexCoord2f(1, 1); glVertex2f(0.5f, -0.5f);    // down right
                        glTexCoord2f(1, 0); glVertex2f(0.5f, 0.5f);     // up right
                        glTexCoord2f(0, 0); glVertex2f(-0.5f, 0.5f);    // up left
                    glEnd();
                glPopMatrix();
            }
        }
    }
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0); 
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

bool are_they_equal(Pixel pi1, Pixel pi2)
{
    if ( (pi1.red == pi2.red) && (pi1.green == pi2.green) && (pi1.blue == pi2.blue) )
    {
        return true;
    }
    else return false;
}

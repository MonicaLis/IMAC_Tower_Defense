#include "monsters_graphic.h"
#include "map_algo.h"
#include "map_graphic.h"
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

#include "map_algo.h"
#include "map_graphic.h"
#include "monsters_algo.h"
#include "monsters_graphic.h"
#include "player.h"


GLuint initTextureMonster(){
    /* Chargement de l'image */
    const char image_path[] = "images/ship1.png";
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

void Monster::drawMonster(){
    glBindTexture(GL_TEXTURE_2D, textureM);
    glPushMatrix();
        glLoadIdentity();
        //*1.4 and +100 to adapt to size of path on screen
        glTranslated(xM*1.4 + 40,yM*1.4 + 100,0); 
        glScalef(50,50,0);
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

//x, y steps of monster before destination1_x, and x, y step after
//when it reaches destination2 it needs to stop
void Monster::move(int initial_pos_x, int before_x, int before_y, int after_x, int after_y, int destination1_x,int destination2_x)
{
    //don't do anything if the monster hasn't arrived yet 
    //and don't do anything if it has arrived to its final destination
    if ( (get_x() >= initial_pos_x) && (get_x() < destination2_x) )
    {
        int pos_x, pos_y;
        if (get_x() < destination1_x)
        {
            pos_x = get_x() + before_x; 
            pos_y = get_y() + before_y;
            set_x(pos_x);
            set_y(pos_y);
        }
        if (get_x() >= destination1_x)
        {
            pos_x = get_x() + after_x; 
            pos_y = get_y() + after_y;
            set_x(pos_x);
            set_y(pos_y);
        }
    }
}

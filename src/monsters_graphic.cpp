#include "monsters_graphic.h"
#include "map_algo.h"
#include "map_graphic.h"
#include <cmath>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <SDL2/SDL_image.h>
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
        glTranslated(xM,yM,0);
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
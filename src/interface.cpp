#include "interface.h"
#include "towers_algo.h"
#include "buildings_algo.h"
#include "monsters_algo.h"


#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>
#include <SDL2/SDL.h>
#include <sstream>
#include <iostream>

GLuint display_win()
{
    /* loading image */
    const char image_path[] = "images/youwon.png";
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(image_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
    if (nullptr == image)
    {
        cerr<<"issue loading image of winning"<<endl;
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


void drawWin(GLuint texture)
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

GLuint display_gameover()
{
    /* loading image */
    const char image_path[] = "images/gameover.png";
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(image_path, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
    if (nullptr == image)
    {
        cerr<<"issue loading image of GO"<<endl;
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return texture_id;
 }

void drawGO(GLuint texture)
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

void delete_all(vector<Tower*> &towers, vector<Monster*> &monsters, vector<Monster*> &supr, vector<Building*> &buildings)
{
    if (!towers.empty())
    {
        for (Tower* tower : towers) {
            delete tower;
        }
    }
    if (!monsters.empty())
    {
        for (Monster* monster : monsters) {
            delete monster;
        }
    }
    if (!supr.empty())
    {
        for (Monster* toSupr : supr) {
            delete toSupr;
        }
    }
    if (!buildings.empty())
    {
        for (Building* building : buildings) {
            delete building;
        }
    }
}
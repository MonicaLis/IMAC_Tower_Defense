#include "interface.h"

#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>

void display_money(int i)
{
    if(i<0){
        i=0;
    }

    std::string nb=std::to_string(i);
    
    const char *pchar=nb.c_str();
    const char *begin="images/money/Money _ ";
    const char *end=".png";
    if(i>50){
        i=50;
       const char *end="+.png"; 
    }
    char result[50];   // array to hold the result.
    strcpy(result,begin); // copy string one into the result.
    strcat(result,pchar); 
    strcat(result,end); 
    /* loading image */
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(result, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
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

     glBindTexture(GL_TEXTURE_2D, texture_id);
        glPushMatrix();
            glTranslated(150,30,0);
            glScalef(242,20,0);
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

void display_win()
{
    /* loading image */
    const char image_path[] = "images/youwon.png";
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

     glBindTexture(GL_TEXTURE_2D, texture_id);
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

void display_gameover()
{
    /* loading image */
    const char image_path[] = "images/gameover.png";
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

     glBindTexture(GL_TEXTURE_2D, texture_id);
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

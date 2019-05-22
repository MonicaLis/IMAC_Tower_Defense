#include "buildings_graphic.h"
#include "buildings_algo.h"
#include "player.h"
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>


GLuint initTextureBuilding( const char* filename)
{
    /* Chargement de l'image */
    int imgWidth, imgHeight, imgChannels;
    unsigned char *image = stbi_load(filename, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
  
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


void Building::drawBuilding()
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
        glLoadIdentity();
        //*1.4 and +100 to adapt to size of path on screen
        //glTranslated(x*1.4 + 40,y*1.4 + 100, 0); 
        glTranslated(x,y,0);
        glScalef(50,50,0);
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

void choose_building(int x, int y, int &type_building)
{
    //Chose Tower
    if( (x>=825) && (x<=950) && (y>=15) && (y<=125)){
        type_building=-1;
        cout << "You chose a tower" <<endl;
    }
    //Chose radar
    if( (x>=825) && (x<=950) && (y>=150) && (y<=240)){
        type_building=0;
        cout << "You chose a radar" <<endl;
    }
    //Chose factory
    if( (x>=825) && (x<=950) && (y>=285) && (y<=400)){
        type_building=1;
        cout << "You chose a factory" <<endl;
    }
    //Chose weapon supplies
    if( (x>=825) && (x<=950) && (y>=450) && (y<=575)){
        type_building=2;
        cout << "You chose munitions stock" <<endl;
    }
}

bool after_chose_building(Building* building, bool valid_zone, Player* player, int &money)
{
    if (building->get_cost() > money) 
    {
        cout<<"You don't have enough money"<<endl;
        valid_zone = false;
    }
    if (valid_zone)
    {     
        cout<<"=== Building built ==="<<endl;
        money = money- building->get_cost();
        player->set_money(money);
        cout << "Available money : "<<money<<endl;
    }
    return valid_zone;
}
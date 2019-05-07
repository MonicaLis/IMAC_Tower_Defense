#ifndef MONSTERS_ALGO_H
#define MONSTERS_ALGO_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>


using namespace std;

typedef struct Monster{

private:
    int life_points, resistance, speed,xM,yM;
    bool dead;
    GLuint textureM;

public:
    Monster(int Cx, int Cy,GLuint newTexture);
    ~Monster();
    int get_life_points();
    void set_life_points(int points);
    int get_resistance();
    void set_resistance(int res);
    int get_speed();
    void set_speed(int sp);
    int get_x();
    void set_x(int Cx);
    int get_y();
    void set_y(int Cy);
    GLuint get_texture();
    void set_texture(GLuint newTexture);
    void drawMonster();
}Monster;

/*
es monstres disposent de points de vie et d’une résistance propre à chacun des types de tour (voir
section suivante). Les monstres arrivent par groupes de dix. À chaque nouvelle vague, les monstres
voient leur point de vie et/ou leur résistance augmenter. C’est à vous de définir cette augmentation.
De même, chaque monstre tué rapporte de l’argent au joueur. Initialement, les monstres de la première
vague rapportent 5 unités d’argent. À chaque nouvelle vague, les monstres rapporteront plus d’argent.
Vous devrez implémenter au moins 2 types différents de monstre ayant des caractéristiques distinctes
notamment de vitesse.
*/

#endif 
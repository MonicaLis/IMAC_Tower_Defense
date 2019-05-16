#ifndef BUILDINGS_ALGO_H
#define BUILDINGS_ALGO_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "map_graphic.h"


typedef struct Building{

private:
    int type; //0 for a radar, 1 for an armamant factory, 2 for munitions
    float range; //(portée)
    int cost;
    GLuint texture;
    int x, y;

public:
    Building(GLuint texture_t, int type_t, int x_t, int y_t, Image* I, int &valid_zone);
    ~Building();
    int get_type();
    void set_type(int type_t);
    float get_range();
    void set_range(float rng);
    int get_cost();
    void set_cost(int cost_t);
    void drawBuilding();
    
}Building;


/*
Trois installations peuvent être construites par le joueur : le radar, l’usine d’armement, et le stock de
munitions. Ces bâtiments sont eux aussi munis d’une portée. Le radar permet d’augmenter de 25%
la portée de toutes les tours situées ... à portée du radar. L’usine d’armement permet d’augmenter
de 25% la puissance des tours à portée. Enfin, le stock de munitions permet d’augmenter de 25% la
cadence de tir des tours à portée. Le coût et la portée de ces installations est laissé à votre appréciation.
les centrales et les installations. Graphique-
ment, les bâtiments sont également représentés par des sprites mais à base carrée d’au moins vingt
pixels (taille laissée à votre appréciation). Notez que ces carrés ont une largeur et une hauteur impaire
ce qui permet de définir sans ambiguı̈té un centre à celui-ci. C’est à partir de ce centre que les portées
sont calculées.
Comme pour les tours, on doit veiller à ce que le placement des bâtiments soient valides c’est à dire
ne recouvre pas ni un chemin (segment large de 30 pixel), ni une tour (disque). Mêmes remarques que
pour le placement des tours : on souhaite éviter un test pixel à pixel.
*/


#endif

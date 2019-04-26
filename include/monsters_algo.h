#ifndef MONSTERS_ALGO_H
#define MONSTERS_ALGO_H

#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct Monster{

private:
    int life_points, resistance, speed;
    bool dead;

public:
    Monster();
    ~Monster();
    int get_life_points();
    void set_life_points(int points);
    int get_resistance();
    void set_resistance(int res);
    int get_speed();
    void set_speed(int sp);
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
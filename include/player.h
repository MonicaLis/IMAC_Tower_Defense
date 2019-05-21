#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <stb_image/stb_image.h>


typedef struct Player{

private:
    int money; //argent du joueur

public:
    Player();
    ~Player();
    int get_money();
    void set_money(int mon);  

    
}Player;

#endif
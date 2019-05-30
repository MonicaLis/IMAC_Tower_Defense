#ifndef PLAYER_H
#define PLAYER_H


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
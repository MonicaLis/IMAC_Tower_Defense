#include "player.h"
#include "interface.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/**********************************STRUCTURE METHODS*********************************************/

    int money;

Player::Player()
{
    money=6;
}

Player::~Player()
{
    money = 0;
}

int Player::get_money()
{
    return money;
}

void Player::set_money(int mon)
{
    money = mon;
}

#include "player.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/**********************************STRUCTURE METHODS*********************************************/

    int money;

Player::Player()
{
    money=10;
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

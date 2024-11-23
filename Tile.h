#pragma once

class Player;

class Tile
{
public:
    Weapon weapon = NO_WEAPON;
    Armor armor = NO_ARMOR;

    Player* player = nullptr;
    int item = 0;
    bool deadBody = false;
};
#pragma once

#include <Windows.h>

#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <vector>
#include <random>
#include <string>

#include "Time.h"
#include "Random.h"

struct Point
{
    int x;
    int y;

    bool operator==(const Point& other)
    {
        if (this->x == other.x && this->y == other.y)
            return true;
        else
            return false;
    }

    bool operator!=(const Point& other) { return !(*this == other); }

    int distance(Point other)
    {
        int distanceX = x - other.x;
        int distanceY = y - other.y;
        
        return (distanceX * distanceX) + (distanceY * distanceY);
    }
};

enum Entity
{
    PLAYER = 1,
    DEAD_PLAYER,
    WEAPON,
    ARMOR
};

enum Weapon
{
    NO_WEAPON = 0,
    CLUB = 5,
    DAGGER = 10,
    SWORD = 15,
    CHAMPIONS_GREATSWORD = 25,
    MJOLNIR = 35,
    STORMBREAKER = 50
};

enum Armor
{
    NO_ARMOR = 0,
    BRONZE = 20,
    IRON = 30,
    STEEL = 40,
    MITHRIL = 60,
    RUNE = 100,
    DRAGON = 200
};

enum GameState
{
    MAIN_MENU,
    MAP_SIZE_MENU,
    BATTLE,
    SETTINGS_MENU,
    LEGEND_MENU,
    EXIT
};

enum PlayerState
{
    AIMLESS,
    GOTO_ITEM,
    GOTO_ENEMY,
    FIGHT
};

#define PRINT(text, color) { SetConsoleTextAttribute(hOut, color); std::wcout << text; SetConsoleTextAttribute(hOut, 15); }
#define PRINT_AT(text, color, x, y) { SetConsoleCursorPosition(hOut, { short(x), short(y) }); PRINT(text, color); }

#define CURRENT_TIME gTime->Current()
#define RANDOM(min, max) gRandom->GenerateNumber(min, max) // Auto detects type
#define RANDOM_T(type, min, max) gRandom->GenerateNumber<type>(min, max) // Explicitly declare type

// Game Options //
#define GAME_SPEED 0.3
#define PLAYER_COUNT 150
#define MAP_WIDTH 130
#define MAP_HEIGHT 35
#define SEARCH_RANGE_WEAPONS 20
#define SEARCH_RANGE_ARMOR 20

// Spawn Percentages // - Total chance of spawning on the battlefield per round
#define CLUB_SPAWN 50.0f
#define DAGGER_SPAWN 35.0f
#define SWORD_SPAWN 15.0f

#define BRONZE_SPAWN 50.0f
#define IRON_SPAWN 25.0f
#define STEEL_SPAWN 15.0f
#define MITHRIL_SPAWN 7.5f
#define RUNE_SPAWN 2.0f
#define DRAGON_SPAWN 0.5f

extern unsigned int mapWidth;
extern unsigned int mapHeight;
extern unsigned int playerCount;
extern unsigned int armorSpawns;
extern unsigned int weaponSpawns;
extern unsigned int playerBoost;
extern unsigned int championBoost;

extern HANDLE hOut;

extern Time* gTime;
extern Random* gRandom;

typedef unsigned int uint;

using namespace std;
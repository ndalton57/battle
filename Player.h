#pragma once

class Game;
class Tile;
typedef std::vector<std::vector<Tile>> Map;

enum SearchLimit
{
    HORIZONTAL_LEFT,
    HORIZONTAL_RIGHT,
    VERTICAL_UP,
    VERTICAL_DOWN
};

class Player
{
    int armorHealth = 0;

    Point destination = { -1, -1 };
    PlayerState state = AIMLESS;
    Player* enemy = nullptr;
    Map& map;
    Game* game;

    void Attack(Player* player, bool receiving = false);

    void Die();

    bool DropItem(int itemType, int dropRange = 1, bool onDeath = false);
    
    bool Move(Point destination, bool stuck = false);

    bool ProcessTurn();

    Point Search(int searchType, int searchRange = 10000, std::pair<SearchLimit, int> = { HORIZONTAL_LEFT, -1 });

    bool WithinRange(Point enemyPlayer);

public:

    bool turnUsed = false; // Attack and/or move will utilize a turn
    bool champion = false;
    int health = 100;
    int kills = 0;
    int winStreak = 0;

    Armor armor = NO_ARMOR;
    Weapon weapon = NO_WEAPON;

    Point location = { -1, -1 };

    Player() = delete;

    Player(Map* map, Game* game) : map(*map), game(game) {}

    void Reset();

    bool Update();

    void Win();
};
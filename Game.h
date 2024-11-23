#pragma once

class Player;
class Tile;
typedef std::vector<std::vector<Tile>> Map;

class Game
{
    bool skipTurn = false;
    double lastTime = CURRENT_TIME;

    int battleCount = 0;

    int fastForward = 1;

    Map map;
    std::vector<Player*> players;
    std::vector<Point> updates;

    void GenerateItems();

    void GeneratePlayers();
    
    void FullRender();

    void ResetMap();

    void ResetPlayers();

public:

    int alivePlayers = 0;
    Player* champion = nullptr;

    Game();

    void UpdateArmor(Point tileLocation);

    bool HoveringArmorCheck(Point tileLocation);

    bool PlayerCheck(Point tileLocation);

    bool IsTileEmpty(Point tileLocation);

    void RenderTile(Point location);

    void Initialize();

    void PlayRound();

    void RenderStats();
};
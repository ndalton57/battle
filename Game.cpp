#include "stdafx.h"
#include "Game.h"

#include "Tile.h"
#include "Player.h"

Game::Game()
{
    playerCount = min(playerCount, (mapWidth * mapHeight)); // Cap player count to full screen minus bottom row
    alivePlayers = playerCount;

    if (armorSpawns == 0) // Use default value
        armorSpawns = (mapWidth * mapHeight) / 150;

    if (weaponSpawns == 0)
        weaponSpawns = (mapWidth * mapHeight) / 150;

    // Allocate Map //
    map.resize(mapWidth);
    for (unsigned int i = 0; i < mapWidth; i++)
        map[i].resize(mapHeight);

    // Allocate Players //
    players.reserve(playerCount);
    for (unsigned int i = 0; i < playerCount; i++)
        players.push_back(new Player(&map, this));

    PRINT_AT(L" ", 11 | 16, mapWidth - 1, mapHeight);
    PRINT_AT(L" ", 15, 0, 0);
}

void Game::GenerateItems()
{
    float dragonChance = DRAGON_SPAWN;
    float runeChance = RUNE_SPAWN + dragonChance;
    float mithrilChance = MITHRIL_SPAWN + runeChance;
    float steelChance = STEEL_SPAWN + mithrilChance;
    float ironChance = IRON_SPAWN + steelChance;
    float bronzeChance = BRONZE_SPAWN + ironChance;

    float swordChance = SWORD_SPAWN;
    float daggerChance = DAGGER_SPAWN + swordChance;
    float clubChance = CLUB_SPAWN + daggerChance;

    // Generate Armor //
    for (unsigned int i = 0; i < armorSpawns; i++)
    {
        unsigned int spawnAttempts = 0;
        while (spawnAttempts++ < mapWidth * mapHeight)
        {
            int x = RANDOM_T(uint, 0, mapWidth - 1);
            int y = RANDOM_T(uint, 0, mapHeight - 1);

            if (map[x][y].item == 0 && map[x][y].player == nullptr)
            {
                float roll = RANDOM(0.0f, 100.0f);

                if (roll <= dragonChance)
                    map[x][y].armor = DRAGON;
                else if (roll <= runeChance)
                    map[x][y].armor = RUNE;
                else if (roll <= mithrilChance)
                    map[x][y].armor = MITHRIL;
                else if (roll <= steelChance)
                    map[x][y].armor = STEEL;
                else if (roll <= ironChance)
                    map[x][y].armor = IRON;
                else if (roll <= bronzeChance)
                    map[x][y].armor = BRONZE;

                map[x][y].item = ARMOR;

                break;
            }
        }
    }

    // Generate Weapons //
    for (unsigned int i = 0; i < weaponSpawns; i++)
    {
        unsigned int spawnAttempts = 0;
        while (spawnAttempts++ < mapWidth * mapHeight)
        {
            int x = RANDOM_T(uint, 0, mapWidth - 1);
            int y = RANDOM_T(uint, 0, mapHeight - 1);

            if (map[x][y].item == 0 && map[x][y].player == nullptr)
            {
                float roll = RANDOM(0.0f, 100.0f);

                if (roll <= swordChance)
                    map[x][y].weapon = SWORD;
                else if (roll <= daggerChance)
                    map[x][y].weapon = DAGGER;
                else if (roll <= clubChance)
                    map[x][y].weapon = CLUB;

                map[x][y].item = WEAPON;

                break;
            }
        }
    }
}

void Game::GeneratePlayers()
{
    for (unsigned int i = 0; i < playerCount; i++)
    {
        while (true)
        {
            Point playerSpawn = {RANDOM_T(int, 0, mapWidth - 1), RANDOM_T(int, 0, mapHeight - 1)};

            if (IsTileEmpty(playerSpawn)) // Valid spawn point
            {
                players[i]->location = playerSpawn;
                map[playerSpawn.x][playerSpawn.y].player = players[i];

                break;
            }
        }
    }
}

void Game::ResetMap()
{
    for (unsigned int y = 0; y < mapHeight; y++)
    {
        for (unsigned int x = 0; x < mapWidth; x++)
        {
            map[x][y].item = 0;
            map[x][y].armor = NO_ARMOR;
            map[x][y].weapon = NO_WEAPON;
            map[x][y].player = nullptr;
            map[x][y].deadBody = false;
        }
    }
}

void Game::ResetPlayers()
{
    for (unsigned int i = 0; i < playerCount; i++)
        players[i]->Reset();
}

void Game::Initialize()
{
    ResetPlayers();
    ResetMap();

    GeneratePlayers();
    GenerateItems();

    FullRender();
    Sleep(250);
}

void Game::UpdateArmor(Point tileLocation)
{
    int x = tileLocation.x;
    int y = tileLocation.y;

    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return;

    if (PlayerCheck({x - 1, y}) == false)
        RenderTile({x - 1, y});
    if (PlayerCheck({x + 1, y}) == false)
        RenderTile({x + 1, y});
}

bool Game::HoveringArmorCheck(Point tileLocation)
{
    int x = tileLocation.x;
    int y = tileLocation.y;

    if (map[x][y].player != nullptr)
        return false;

    if (x - 1 >= 0 && map[x - 1][y].player != nullptr && map[x - 1][y].player->armor != NO_ARMOR) // Right armor piece check
        return true;
    else if (x + 1 < mapWidth && map[x + 1][y].player != nullptr && map[x + 1][y].player->armor != NO_ARMOR) // Left armor piece check
        return true;
    else
        return false;
}

bool Game::PlayerCheck(Point tileLocation)
{
    int x = tileLocation.x;
    int y = tileLocation.y;

    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return false;

    if (map[x][y].player == nullptr)
        return false;
    else
        return true;
}

bool Game::IsTileEmpty(Point tileLocation)
{
    int x = tileLocation.x;
    int y = tileLocation.y;

    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return false;

    if (map[x][y].item == 0 && map[x][y].player == nullptr && map[x][y].deadBody == false && HoveringArmorCheck(tileLocation) == false)
        return true;
    else
        return false;
}

void Game::RenderTile(Point location)
{
    if (skipTurn == true)
        return;

    // Render priority: Player->Player Armor->Items->Dead Bodies
    // Although items and dead bodies should never logically occupy the same space

    int x = location.x;
    int y = location.y;

    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) // Bounds check
        return;

    if (map[x][y].player != nullptr) // Render player
    {
        Player *player = map[x][y].player;
        std::wstring playerIcon;
        int playerColor;

        float healthPercentage;

        if (player->health > 0)
        {
            healthPercentage = (float)player->health / (float)(100 + playerBoost);
            healthPercentage *= 100;
        }
        else
            healthPercentage = 0;

        if (healthPercentage >= 95 && player->winStreak >= 25)
            playerIcon = L"Ω";
        else if (healthPercentage >= 95)
            playerIcon = L"☺";
        else if (healthPercentage >= 85)
            playerIcon = L"9";
        else if (healthPercentage >= 75)
            playerIcon = L"8";
        else if (healthPercentage >= 65)
            playerIcon = L"7";
        else if (healthPercentage >= 55)
            playerIcon = L"6";
        else if (healthPercentage >= 45)
            playerIcon = L"5";
        else if (healthPercentage >= 35)
            playerIcon = L"4";
        else if (healthPercentage >= 25)
            playerIcon = L"3";
        else if (healthPercentage >= 15)
            playerIcon = L"2";
        else if (healthPercentage > 0)
            playerIcon = L"1";
        else
            playerIcon = L"☻";

        if (player->health <= 0)
            playerColor = 12;
        else if (player->weapon == NO_WEAPON)
            playerColor = 15;
        else if (player->weapon == CLUB)
            playerColor = 14;
        else if (player->weapon == DAGGER)
            playerColor = 8;
        else if (player->weapon == SWORD)
            playerColor = 9;
        else if (player->weapon == CHAMPIONS_GREATSWORD)
            playerColor = 10;
        else if (player->weapon == MJOLNIR)
            playerColor = 13;
        else if (player->weapon == STORMBREAKER)
            playerColor = 11;

        PRINT_AT(playerIcon.c_str(), playerColor, x, y);

        if (player->armor != NO_ARMOR)
            UpdateArmor({x, y});
    }
    else if (map[x][y].deadBody) // Render dead bodies
        PRINT_AT(L"☻", 12, x, y)
    else if (x + 1 < mapWidth && map[x + 1][y].player != nullptr && map[x + 1][y].player->armor != NO_ARMOR) // Left armor bracket
    {
        Player *player = map[x + 1][y].player;
        int armorColor;

        if (player->armor == BRONZE)
            armorColor = 14;
        else if (player->armor == IRON)
            armorColor = 8;
        else if (player->armor == STEEL)
            armorColor = 15;
        else if (player->armor == MITHRIL)
            armorColor = 9;
        else if (player->armor == RUNE)
            armorColor = 11;
        else if (player->armor == DRAGON)
            armorColor = 12;

        PRINT_AT(L"[", armorColor, x, y);
    }
    else if (x - 1 >= 0 && map[x - 1][y].player != nullptr && map[x - 1][y].player->armor != NO_ARMOR) // Right armor bracket
    {
        Player *player = map[x - 1][y].player;
        int armorColor;

        if (player->armor == BRONZE)
            armorColor = 14;
        else if (player->armor == IRON)
            armorColor = 8;
        else if (player->armor == STEEL)
            armorColor = 15;
        else if (player->armor == MITHRIL)
            armorColor = 9;
        else if (player->armor == RUNE)
            armorColor = 11;
        else if (player->armor == DRAGON)
            armorColor = 12;

        PRINT_AT(L"]", armorColor, x, y);
    }
    else if (map[x][y].item != 0) // Render items
    {
        if (map[x][y].weapon == CLUB)
            PRINT_AT(L"!", 14, x, y)
        else if (map[x][y].weapon == DAGGER)
            PRINT_AT(L"!", 8, x, y)
        else if (map[x][y].weapon == SWORD)
            PRINT_AT(L"!", 9, x, y)
        else if (map[x][y].weapon == CHAMPIONS_GREATSWORD)
            PRINT_AT(L"!", 10, x, y)
        else if (map[x][y].weapon == MJOLNIR)
            PRINT_AT(L"!", 13, x, y)
        else if (map[x][y].weapon == STORMBREAKER)
            PRINT_AT(L"!", 11, x, y)
        else if (map[x][y].armor == BRONZE)
            PRINT_AT(L"+", 14, x, y)
        else if (map[x][y].armor == IRON)
            PRINT_AT(L"+", 8, x, y)
        else if (map[x][y].armor == STEEL)
            PRINT_AT(L"+", 15, x, y)
        else if (map[x][y].armor == MITHRIL)
            PRINT_AT(L"+", 9, x, y)
        else if (map[x][y].armor == RUNE)
            PRINT_AT(L"+", 11, x, y)
        else if (map[x][y].armor == DRAGON)
            PRINT_AT(L"+", 12, x, y)
    }
    else
        PRINT_AT(L" ", 15, x, y)
}

void Game::FullRender()
{
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
            RenderTile({x, y});
    }

    PRINT_AT(L"Speed: ", 11 | 16, 0, mapHeight);

    if (champion != nullptr)
    {
        PRINT_AT(L", Champion's Health: ", 11 | 16, 9, mapHeight);
        PRINT_AT(std::to_wstring(champion->health), 11 | 16, 30, mapHeight);

        PRINT_AT(L", Champion's kills: ", 11 | 16, 33, mapHeight);
        PRINT_AT(std::to_wstring(champion->kills), 11 | 16, 53, mapHeight)
    }

    PRINT_AT(L"<- -> = Slow Down/Fast Forward, Space = Skip Round", 11 | 16, mapWidth - 51, mapHeight);
}

void Game::RenderStats()
{
    PRINT_AT(std::wstring(L"x") + std::to_wstring(fastForward), 11 | 16, 7, mapHeight);

    // Render Info //
    if (champion != nullptr)
    {
        // Speed //
        // Champion Health //
        if (champion->health < 10)
            PRINT_AT(std::to_wstring(champion->health) + L"  ", 11 | 16, 30, mapHeight)
        else if (champion->health < 100)
            PRINT_AT(std::to_wstring(champion->health) + L" ", 11 | 16, 30, mapHeight)
        else if (champion->health >= 100)
            PRINT_AT(std::to_wstring(champion->health), 11 | 16, 30, mapHeight)

        // Champion Kills //
        if (champion->kills < 10)
            PRINT_AT(std::to_wstring(champion->kills) + L"  ", 11 | 16, 53, mapHeight)
        else if (champion->kills < 100)
            PRINT_AT(std::to_wstring(champion->kills) + L" ", 11 | 16, 53, mapHeight)
        else if (champion->kills >= 100)
            PRINT_AT(std::to_wstring(champion->kills), 11 | 16, 53, mapHeight)
    }
    else if (battleCount > 0)
    {
        PRINT_AT(L"  ", 11 | 16, 30, mapHeight);
        PRINT_AT(L"  ", 11 | 16, 53, mapHeight);
    }
}

void Game::PlayRound()
{
    alivePlayers = playerCount;
    while (alivePlayers > 1)
    {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            skipTurn = true;
        else if (GetAsyncKeyState(VK_RIGHT) & 0x1 || GetAsyncKeyState(VK_OEM_PERIOD) & 0x1)
            fastForward += 1;
        else if (GetAsyncKeyState(VK_LEFT) & 0x1 || GetAsyncKeyState(VK_OEM_COMMA) & 0x1)
            fastForward -= 1;

        if (fastForward < 1)
            fastForward = 1;
        else if (fastForward > 5)
            fastForward = 5;

        // Cap Game Speed //
        double currentTime = CURRENT_TIME;
        if (CURRENT_TIME - lastTime < (GAME_SPEED / fastForward) && skipTurn == false)
        {
            Sleep(1);
            continue;
        }

        lastTime = CURRENT_TIME;

        // Simulate Players //
        for (unsigned int i = 0; i < players.size(); i++)
            players[i]->Update();

        // Reset Turns //
        for (unsigned int i = 0; i < players.size(); i++)
            players[i]->turnUsed = false;

        RenderStats();
    }

    // Crown Champion //
    for (unsigned int i = 0; i < players.size(); i++)
    {
        if (players[i]->health > 0)
            players[i]->Win();
    }

    if (skipTurn == false)
        Sleep(1750);

    skipTurn = false;
    battleCount++;
}
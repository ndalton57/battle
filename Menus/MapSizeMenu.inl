#pragma once

class MapSizeMenu
{
    bool firstUpdate = true;

    int selection = 0;

    std::wstring battleString = L"Battle";
    std::wstring arrowString = L"-->";
    int startingX = (mapWidth / 2) + (battleString.length() / 2) + 3 + arrowString.length() + 3;
    int startingY = (int)(mapHeight / 2.5);

    std::wstring smallString = L"Small";
    std::wstring mediumString = L"Medium";
    std::wstring largeString = L"Large";
    std::wstring hugeString = L"Huge";

    void Launch();

    void ClearAll();
    void ClearMenu();
    void ClearSelection();
    void HighlightSelection();

public:

    MapSizeMenu() = default;

    GameState Update();
};

inline void MapSizeMenu::Launch()
{
    PRINT_AT(arrowString, 15, startingX - arrowString.length() - 3, startingY);
    
    PRINT_AT(smallString, 15, startingX, startingY);
    PRINT_AT(mediumString, 15, startingX, startingY + 1);
    PRINT_AT(largeString, 15, startingX, startingY + 2);
    PRINT_AT(hugeString, 15, startingX, startingY + 3);

    HighlightSelection();

    Sleep(250);
}

inline void MapSizeMenu::ClearAll()
{
    for (unsigned int y = 0; y < mapHeight; y++)
    {
        for (unsigned int x = 0; x < mapWidth; x++)
        {
            PRINT_AT(L" ", 15, x, y);
        }
    }
}

inline void MapSizeMenu::ClearMenu()
{
    ClearSelection();

    PRINT_AT(L"               ", 15, startingX - arrowString.length() - 3, startingY);
    PRINT_AT(L"         ", 15, startingX, startingY + 1);
    PRINT_AT(L"         ", 15, startingX, startingY + 2);
    PRINT_AT(L"         ", 15, startingX, startingY + 3);

}

inline void MapSizeMenu::ClearSelection()
{
    if (selection == 0)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY);
        PRINT_AT(L" ", 15, startingX + smallString.length() + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 1);
        PRINT_AT(L" ", 15, startingX + mediumString.length() + 1, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 2);
        PRINT_AT(L" ", 15, startingX + largeString.length() + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 3);
        PRINT_AT(L" ", 15, startingX + hugeString.length() + 1, startingY + 3);
    }
}

inline void MapSizeMenu::HighlightSelection()
{
    if (selection == 0)
    {
        PRINT_AT(L"[", 15, startingX - 2, startingY);
        PRINT_AT(L"]", 15, startingX + smallString.length() + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(L"[", 15, startingX - 2, startingY + 1);
        PRINT_AT(L"]", 15, startingX + mediumString.length() + 1, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(L"[", 15, startingX - 2, startingY + 2);
        PRINT_AT(L"]", 15, startingX + largeString.length() + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(L"[", 15, startingX - 2, startingY + 3);
        PRINT_AT(L"]", 15, startingX + hugeString.length() + 1, startingY + 3);
    }
}

inline GameState MapSizeMenu::Update()
{
    if (firstUpdate)
    {
        firstUpdate = false;
        Launch();
    }

    if (GetAsyncKeyState(VK_UP) & 0x1)
    {
        ClearSelection();
        selection--;

        if (selection < 0)
            selection = 3;
        if (selection > 3)
            selection = 0;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x1)
    {
        ClearSelection();
        selection++;

        if (selection < 0)
            selection = 3;
        if (selection > 3)
            selection = 0;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE))
    {
        ClearAll();

        if (selection == 0)
        {
            mapWidth = 130;
            mapHeight = 35;

            if (playerCount == 0)
                playerCount = 20;
        }
        else if (selection == 1)
        {
            mapWidth = 180;
            mapHeight = 50;

            if (playerCount == 0)
                playerCount = 60;
        }
        else if (selection == 2) // Full screen
        {
            mapWidth = 240;
            mapHeight = 60;

            if (playerCount == 0)
                playerCount = 100;
        }
        else if (selection == 3) // Full screen
        {
            mapWidth = 1000;
            mapHeight = 1000;

            if (playerCount == 0)
                playerCount = 150;
        }

        return BATTLE;
    }
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_BACK) || GetAsyncKeyState(VK_LEFT))
    {
        ClearMenu();
        firstUpdate = true;
        selection = 0;
        return MAIN_MENU;
    }

    return MAP_SIZE_MENU;
}
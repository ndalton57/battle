#pragma once

class MainMenu
{
    bool firstUpdate = true;

    int selection = 0;
    int startingY = int(mapHeight / 2.5);

    std::wstring battleString = L"Battle";
    std::wstring settingsString = L"Settings";
    std::wstring legendString = L"Legend";
    std::wstring exitString = L"Exit";

    std::wstring randomText;

    void Launch();

    void ClearSelection();
    void HighlightSelection();

public:

    MainMenu() = default;

    GameState Update();
};

inline void MainMenu::ClearSelection()
{
    if (selection == 0)
    {
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (battleString.length() / 2)) - 2, startingY);
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (battleString.length() / 2)) + battleString.length() + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (settingsString.length() / 2)) - 2, startingY + 1);
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (settingsString.length() / 2)) + settingsString.length() + 1, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (legendString.length() / 2)) - 2, startingY + 2);
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (legendString.length() / 2)) + legendString.length() + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (exitString.length() / 2)) - 2, startingY + 3);
        PRINT_AT(L" ", 15, ((mapWidth / 2) - (exitString.length() / 2)) + exitString.length() + 1, startingY + 3);
    }
}

inline void MainMenu::HighlightSelection()
{
    if (selection == 0)
    {
        PRINT_AT(L"[", 15, ((mapWidth / 2) - (battleString.length() / 2)) - 2, startingY);
        PRINT_AT(L"]", 15, ((mapWidth / 2) - (battleString.length() / 2)) + battleString.length() + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(L"[", 15, ((mapWidth / 2) - (settingsString.length() / 2)) - 2, startingY + 1);
        PRINT_AT(L"]", 15, ((mapWidth / 2) - (settingsString.length() / 2)) + settingsString.length() + 1, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(L"[", 15, ((mapWidth / 2) - (legendString.length() / 2)) - 2, startingY + 2);
        PRINT_AT(L"]", 15, ((mapWidth / 2) - (legendString.length() / 2)) + legendString.length() + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(L"[", 12, ((mapWidth / 2) - (exitString.length() / 2)) - 2, startingY + 3);
        PRINT_AT(L"]", 12, ((mapWidth / 2) - (exitString.length() / 2)) + exitString.length() + 1, startingY + 3);
    }
}

inline void MainMenu::Launch()
{
    for (unsigned int y = 0; y < mapHeight; y++)
    {
        for (unsigned int x = 0; x < mapWidth; x++)
        {
            if (x == mapWidth - 1 && y == mapHeight - 1) // Corner check
                continue;
            else
                PRINT_AT(L" ", 15, x, y);
        }
    }

    PRINT_AT(battleString, 15, (mapWidth / 2) - (battleString.length() / 2), startingY);
    PRINT_AT(settingsString, 15, (mapWidth / 2) - (settingsString.length() / 2), startingY + 1);
    PRINT_AT(legendString, 15, (mapWidth / 2) - (legendString.length() / 2), startingY + 2);
    PRINT_AT(exitString, 12, (mapWidth / 2) - (exitString.length() / 2), startingY + 3);

    HighlightSelection();
}

inline GameState MainMenu::Update()
{
    if (firstUpdate)
    {
        Launch();
        firstUpdate = false;
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
        if (selection == 0)
        {

            return MAP_SIZE_MENU;
        }
        else if (selection == 1)
            return SETTINGS_MENU;
        else if (selection == 2)
            return LEGEND_MENU;
        else if (selection == 3)
            return EXIT;
    }

    return MAIN_MENU;
}
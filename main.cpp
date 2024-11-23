#include "stdafx.h"
#include "Game.h"
#include "Tile.h"
#include "Player.h"
#include "Menus/MainMenu.inl"
#include "Menus/MapSizeMenu.inl"
#include "Menus/SettingsMenu.inl"
#include "Menus/LegendMenu.inl"

unsigned int mapWidth = MAP_WIDTH;
unsigned int mapHeight = MAP_HEIGHT;
unsigned int playerCount = 0;
unsigned int armorSpawns = 0;
unsigned int weaponSpawns = 0;
unsigned int playerBoost = 0;
unsigned int championBoost = 0;

unsigned int maxWidth;
unsigned int maxHeight;

void CenterWindow()
{
    RECT screenRect;
    GetWindowRect(GetDesktopWindow(), &screenRect);

    RECT consoleRect;
    GetWindowRect(GetConsoleWindow(), &consoleRect);

    int startX = (screenRect.right / 2) - ((consoleRect.right - consoleRect.left) / 2);
    int startY = (screenRect.bottom / 2) - ((consoleRect.bottom - consoleRect.top) / 2);
    SetWindowPos(GetConsoleWindow(), NULL, startX, startY, NULL, NULL, SWP_NOSIZE);
}

void SetFontSize(int fontSize)
{
    CONSOLE_FONT_INFOEX fontSettings;

    fontSettings.cbSize = sizeof(fontSettings);
    fontSettings.nFont = 0;
    fontSettings.dwFontSize.X = 0;
    fontSettings.dwFontSize.Y = fontSize;
    fontSettings.FontFamily = FF_DONTCARE;
    fontSettings.FontWeight = FW_NORMAL;
    wcscpy_s(fontSettings.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(hOut, FALSE, &fontSettings);
}

void ResizeWindow()
{
    SetFontSize(14);

    // Cap To Fullscreen //
    COORD fullscreen = GetLargestConsoleWindowSize(hOut);

    maxWidth = fullscreen.X;
    maxHeight = fullscreen.Y;

    mapWidth = min(mapWidth, maxWidth);
    mapHeight = min(mapHeight, maxHeight);

    // Set Console Size And Buffer //
    COORD size = { (short)mapWidth , (short)mapHeight };
    SetConsoleScreenBufferSize(hOut, size);

    SMALL_RECT screenInfo = { 0, 0, mapWidth - 1, mapHeight - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &screenInfo);

    if (mapWidth == maxWidth && mapHeight == maxHeight) // Maximize window if big enough
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    else
    {
        Sleep(50);
        CenterWindow();
    }

    mapHeight--; // Reserve one row for info display
}

void EnableUnicode()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false; // Set the cursor visibility
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void FlushBuffers()
{
    GetAsyncKeyState(VK_LEFT);
    GetAsyncKeyState(VK_RIGHT);
    GetAsyncKeyState(VK_UP);
    GetAsyncKeyState(VK_DOWN);
    GetAsyncKeyState(VK_RETURN);
    GetAsyncKeyState(VK_ESCAPE);
    GetAsyncKeyState(VK_SPACE);
    GetAsyncKeyState(VK_BACK);
    GetAsyncKeyState(VK_OEM_COMMA);
    GetAsyncKeyState(VK_OEM_PERIOD);
    GetAsyncKeyState(VK_CONTROL);
}

int main()
{
    ResizeWindow();
    EnableUnicode();

    GameState state = MAIN_MENU;

    MainMenu mainMenu;
    MapSizeMenu mapSizeMenu;
    SettingsMenu settingsMenu;
    LegendMenu legendMenu;

    double lastTime = 0.0;

    while (state != BATTLE)
    {
        // Cap Game Speed //
        double currentTime = CURRENT_TIME;
        while (CURRENT_TIME - lastTime < 0.01667)
            Sleep(1);

        lastTime = CURRENT_TIME;

        if (state == MAIN_MENU)
            state = mainMenu.Update();
        else if (state == MAP_SIZE_MENU)
            state = mapSizeMenu.Update();
        else if (state == SETTINGS_MENU)
            state = settingsMenu.Update();
        else if (state == LEGEND_MENU)
            state = legendMenu.Update();
        else if (state == EXIT)
        {
            SetConsoleCursorPosition(hOut, { short(0), short(mapHeight) });
            return 1;
        }

        FlushBuffers();
    }

    ResizeWindow();

    Game* game = new Game();
    
    while (true)
    {
        game->Initialize();
        game->PlayRound();
        FlushBuffers();
    }
}
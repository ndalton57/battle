#pragma once

class SettingsMenu
{
    bool firstUpdate = true;

    int selection = 0;
    int selected = false;

    std::wstring settingsString = L"Settings";
    std::wstring arrowString = L"-->";
    int startingX = (mapWidth / 2) + (settingsString.length() / 2) + 3 + arrowString.length() + 3;
    int startingY = (int)(mapHeight / 2.5) + 1;

    std::wstring defaultString = L"Default";
    std::wstring noteString = L"Note: Hold Control to change values quicker";
    std::wstring playerCountOverrideString = L"Player Count Override:";
    std::wstring armorSpawnOverrideString = L"Armor Spawn Override:";
    std::wstring weaponSpawnOverrideString = L"Weapon Spawn Override:";
    std::wstring playerHealthBoostString = L"Player Health Boost:";
    std::wstring championHealthBoostString = L"Champion Health Boost:";

    int playerCountOverride = 0;
    int armorSpawnOverride = 0;
    int weaponSpawnOverride = 0;
    int playerBoostOverride = 0;
    int championBoostOverride = 0;

    void Launch();

    void ClearMenu();
    void ClearSelection();
    void ClearValue();
    void HighlightSelection();

public:
    SettingsMenu() = default;

    GameState Update();
};

inline void SettingsMenu::Launch()
{
    PRINT_AT(arrowString, 15, startingX - arrowString.length() - 3, startingY);

    PRINT_AT(noteString, 10, startingX, startingY - 1);
    PRINT_AT(playerCountOverrideString + L" " + ((playerCountOverride > 0) ? std::to_wstring(playerCountOverride) : defaultString), 15, startingX, startingY);
    PRINT_AT(armorSpawnOverrideString + L"  " + ((armorSpawnOverride > 0) ? std::to_wstring(armorSpawnOverride) : defaultString), 15, startingX, startingY + 1);
    PRINT_AT(weaponSpawnOverrideString + L" " + ((weaponSpawnOverride > 0) ? std::to_wstring(weaponSpawnOverride) : defaultString), 15, startingX, startingY + 2);
    PRINT_AT(playerHealthBoostString + L"   " + ((playerBoostOverride > 0) ? std::to_wstring(playerBoostOverride) : defaultString), 15, startingX, startingY + 3);
    PRINT_AT(championHealthBoostString + L" " + ((championBoostOverride > 0) ? std::to_wstring(championBoostOverride) : defaultString), 15, startingX, startingY + 4);

    HighlightSelection();
}

inline void SettingsMenu::ClearMenu()
{
    PRINT_AT(L"                                                       ", 15, startingX - arrowString.length() - 3, startingY);

    PRINT_AT(L"                                                    ", 15, startingX, startingY - 1);
    PRINT_AT(L"                                                    ", 15, startingX - 3, startingY + 1);
    PRINT_AT(L"                                                    ", 15, startingX - 3, startingY + 2);
    PRINT_AT(L"                                                    ", 15, startingX - 3, startingY + 3);
    PRINT_AT(L"                                                    ", 15, startingX - 3, startingY + 4);
}

inline void SettingsMenu::ClearSelection()
{
    if (selected)
        ClearValue();

    if (selection == 0)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY);
        PRINT_AT(L" ", 15, startingX + playerCountOverrideString.length() + 1 + ((playerCountOverride > 0) ? std::to_wstring(playerCountOverride).length() : defaultString.length()) + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 1);
        PRINT_AT(L" ", 15, startingX + armorSpawnOverrideString.length() + 2 + ((armorSpawnOverride > 0) ? std::to_wstring(armorSpawnOverride).length() : defaultString.length()) + 1, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 2);
        PRINT_AT(L" ", 15, startingX + weaponSpawnOverrideString.length() + 1 + ((weaponSpawnOverride > 0) ? std::to_wstring(weaponSpawnOverride).length() : defaultString.length()) + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 3);
        PRINT_AT(L" ", 15, startingX + playerHealthBoostString.length() + 3 + ((playerBoostOverride > 0) ? std::to_wstring(playerBoostOverride).length() : defaultString.length()) + 1, startingY + 3);
    }
    else if (selection == 4)
    {
        PRINT_AT(L" ", 15, startingX - 2, startingY + 4);
        PRINT_AT(L" ", 15, startingX + championHealthBoostString.length() + 1 + ((championBoostOverride > 0) ? std::to_wstring(championBoostOverride).length() : defaultString.length()) + 1, startingY + 4);
    }
}

inline void SettingsMenu::ClearValue()
{
    if (selection == 0)
        PRINT_AT(L"         ", 15, startingX + playerCountOverrideString.length() + 1, startingY)
    else if (selection == 1)
        PRINT_AT(L"         ", 15, startingX + armorSpawnOverrideString.length() + 1, startingY + 1)
    else if (selection == 2)
        PRINT_AT(L"         ", 15, startingX + weaponSpawnOverrideString.length() + 1, startingY + 2)
    else if (selection == 3)
        PRINT_AT(L"         ", 15, startingX + playerHealthBoostString.length() + 1, startingY + 3)
    else if (selection == 4)
        PRINT_AT(L"         ", 15, startingX + championHealthBoostString.length() + 1, startingY + 4)
}

inline void SettingsMenu::HighlightSelection()
{
    if (selection == 0)
    {
        PRINT_AT(((selected) ? L"◄" : L"["), (selected) ? 11 : 15, startingX - 2, startingY);
        PRINT_AT(((selected) ? L"►" : L"]"), (selected) ? 11 : 15, startingX + playerCountOverrideString.length() + 1 + ((playerCountOverride > 0) ? std::to_wstring(playerCountOverride).length() : defaultString.length()) + 1, startingY);

        PRINT_AT(((playerCountOverride > 0) ? std::to_wstring(playerCountOverride) : defaultString), (selected) ? 11 : 15, startingX + playerCountOverrideString.length() + 1, startingY);
    }
    else if (selection == 1)
    {
        PRINT_AT(((selected) ? L"◄" : L"["), (selected) ? 11 : 15, startingX - 2, startingY + 1);
        PRINT_AT(((selected) ? L"►" : L"]"), (selected) ? 11 : 15, startingX + armorSpawnOverrideString.length() + 2 + ((armorSpawnOverride > 0) ? std::to_wstring(armorSpawnOverride).length() : defaultString.length()) + 1, startingY + 1);

        PRINT_AT(((armorSpawnOverride > 0) ? std::to_wstring(armorSpawnOverride) : defaultString), (selected) ? 11 : 15, startingX + armorSpawnOverrideString.length() + 2, startingY + 1);
    }
    else if (selection == 2)
    {
        PRINT_AT(((selected) ? L"◄" : L"["), (selected) ? 11 : 15, startingX - 2, startingY + 2);
        PRINT_AT(((selected) ? L"►" : L"]"), (selected) ? 11 : 15, startingX + weaponSpawnOverrideString.length() + 1 + ((weaponSpawnOverride > 0) ? std::to_wstring(weaponSpawnOverride).length() : defaultString.length()) + 1, startingY + 2);

        PRINT_AT(((weaponSpawnOverride > 0) ? std::to_wstring(weaponSpawnOverride) : defaultString), (selected) ? 11 : 15, startingX + weaponSpawnOverrideString.length() + 1, startingY + 2);
    }
    else if (selection == 3)
    {
        PRINT_AT(((selected) ? L"◄" : L"["), (selected) ? 11 : 15, startingX - 2, startingY + 3);
        PRINT_AT(((selected) ? L"►" : L"]"), (selected) ? 11 : 15, startingX + playerHealthBoostString.length() + 3 + ((playerBoostOverride > 0) ? std::to_wstring(playerBoostOverride).length() : defaultString.length()) + 1, startingY + 3);

        PRINT_AT(((playerBoostOverride > 0) ? std::to_wstring(playerBoostOverride) : defaultString), (selected) ? 11 : 15, startingX + playerHealthBoostString.length() + 3, startingY + 3);
    }
    else if (selection == 4)
    {
        PRINT_AT(((selected) ? L"◄" : L"["), (selected) ? 11 : 15, startingX - 2, startingY + 4);
        PRINT_AT(((selected) ? L"►" : L"]"), (selected) ? 11 : 15, startingX + championHealthBoostString.length() + 1 + ((championBoostOverride > 0) ? std::to_wstring(championBoostOverride).length() : defaultString.length()) + 1, startingY + 4);

        PRINT_AT(((championBoostOverride > 0) ? std::to_wstring(championBoostOverride) : defaultString), (selected) ? 11 : 15, startingX + championHealthBoostString.length() + 1, startingY + 4);
    }
}

inline GameState SettingsMenu::Update()
{
    if (firstUpdate)
    {
        firstUpdate = false;
        Launch();
    }

    bool speedChange = false;
    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
        speedChange = true;

    if (GetAsyncKeyState(VK_UP) & 0x1)
    {
        if (selected)
        {
            selected = false;
            HighlightSelection();
        }

        ClearSelection();
        selection--;

        if (selection < 0)
            selection = 4;
        if (selection > 4)
            selection = 0;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_DOWN) & 0x1)
    {
        if (selected)
        {
            selected = false;
            HighlightSelection();
        }

        ClearSelection();
        selection++;

        if (selection < 0)
            selection = 4;
        if (selection > 4)
            selection = 0;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_LEFT) & 0x1)
    {
        if (selected == false)
            selected = true;

        ClearSelection();

        if (selection == 0)
            playerCountOverride = max(0, playerCountOverride - ((speedChange) ? 10 : 1)); // Set floor of zero
        else if (selection == 1)
            armorSpawnOverride = max(0, armorSpawnOverride - ((speedChange) ? 10 : 1));
        else if (selection == 2)
            weaponSpawnOverride = max(0, weaponSpawnOverride - ((speedChange) ? 10 : 1));
        else if (selection == 3)
            playerBoostOverride = max(0, playerBoostOverride - ((speedChange) ? 10 : 1));
        else if (selection == 4)
            championBoostOverride = max(0, championBoostOverride - ((speedChange) ? 10 : 1));

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_RIGHT) & 0x1)
    {
        if (selected == false)
            selected = true;

        ClearSelection();

        if (selection == 0)
            playerCountOverride += (speedChange) ? 10 : 1;
        else if (selection == 1)
            armorSpawnOverride += (speedChange) ? 10 : 1;
        else if (selection == 2)
            weaponSpawnOverride += (speedChange) ? 10 : 1;
        else if (selection == 3)
            playerBoostOverride += (speedChange) ? 10 : 1;
        else if (selection == 4)
            championBoostOverride += (speedChange) ? 10 : 1;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_RETURN) & 0x1)
    {
        selected = !selected;

        HighlightSelection();
    }
    else if (GetAsyncKeyState(VK_ESCAPE) & 0x1 || GetAsyncKeyState(VK_BACK) & 0x1)
    {
        // Store Overrides //
        playerCount = playerCountOverride;
        armorSpawns = armorSpawnOverride;
        weaponSpawns = weaponSpawnOverride;
        playerBoost = playerBoostOverride;
        championBoost = championBoostOverride;

        // Exit //
        ClearMenu();
        firstUpdate = true;
        selection = 0;
        selected = false;
        return MAIN_MENU;
    }

    return SETTINGS_MENU;
}
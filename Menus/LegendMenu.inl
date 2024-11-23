#pragma once

class LegendMenu
{
    bool firstUpdate = true;

    std::wstring legendString = L"Legend";
    std::wstring arrowString = L"-->";
    int startingX = (mapWidth / 2) + (legendString.length() / 2) + 3 + arrowString.length() + 1;
    int startingY = (int)(mapHeight / 2.5) + 2;

    void Launch();

    void ClearMenu();

public:

    LegendMenu() = default;

    GameState Update();
};

inline void LegendMenu::Launch()
{
    PRINT_AT(arrowString, 15, startingX - arrowString.length() - 1, startingY);

    PRINT_AT(L"!", 14, startingX, startingY);
    PRINT(L" = Club (+5 Attack)", 15);

    PRINT_AT(L"!", 8, startingX, startingY + 1);
    PRINT(L" = Dagger (+10 Attack)", 15);

    PRINT_AT(L"!", 9, startingX, startingY + 2);
    PRINT(L" = Sword (+15 Attack)", 15);

    PRINT_AT(L"!", 10, startingX, startingY + 3);
    PRINT(L" = Champion's Greatsword (+25 Attack, 1 Win)", 15);

    PRINT_AT(L"!", 13, startingX, startingY + 4);
    PRINT(L" = Mjolnir (+35 Attack, 10 Wins)", 15);

    PRINT_AT(L"!", 11, startingX, startingY + 5);
    PRINT(L" = Stormbreaker (+50 Attack, Never misses, 25 Wins)", 15);

    PRINT_AT(L"+", 14, startingX, startingY + 6);
    PRINT(L" = Bronze Armor (+20 Armor)", 15);

    PRINT_AT(L"+", 8, startingX, startingY + 7);
    PRINT(L" = Iron Armor (+30 Armor)", 15);

    PRINT_AT(L"+", 15, startingX, startingY + 8);
    PRINT(L" = Steel Armor (+40 Armor)", 15);

    PRINT_AT(L"+", 9, startingX, startingY + 9);
    PRINT(L" = Mithril Armor (+60 Armor)", 15);

    PRINT_AT(L"+", 11, startingX, startingY + 10);
    PRINT(L" = Rune Armor (+100 Armor)", 15);

    PRINT_AT(L"+", 12, startingX, startingY + 11);
    PRINT(L" = Dragon Armor (+200 Armor)", 15);

    PRINT_AT(L"☺", 15, startingX, startingY + 12);
    PRINT(L" = Player (100 health, Color = Weapon Type)", 15);

    PRINT_AT(L"Ω", 11, startingX, startingY + 13);
    PRINT(L" = Omega Champion (500 health, 25 wins)", 15);

    PRINT_AT(L"[]", 15, startingX - 1, startingY + 14);
    PRINT(L" = Armor (Color = Armor Type)", 15);
}

inline void LegendMenu::ClearMenu()
{
    PRINT_AT(L"                                                                 ", 15, startingX - arrowString.length() - 1, startingY);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 1);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 2);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 3);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 4);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 5);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 6);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 7);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 8);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 9);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 10);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 11);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 12);
    PRINT_AT(L"                                                             ", 15, startingX, startingY + 13);
    PRINT_AT(L"                                                             ", 15, startingX - 1, startingY + 14);
}

inline GameState LegendMenu::Update()
{
    if (firstUpdate)
    {
        Launch();
        firstUpdate = false;
    }

    if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_BACK) || GetAsyncKeyState(VK_LEFT))
    {
        ClearMenu();
        firstUpdate = true;
        return MAIN_MENU;
    }

    return LEGEND_MENU;
}
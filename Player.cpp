#include "stdafx.h"
#include "Player.h"

#include "Tile.h"
#include "Game.h"

void Player::Attack(Player* otherPlayer, bool receiving)
{
    int damage = (int)(RANDOM(0.0f, 1500.0f) / 100.0f); // Wasn't getting a good enough random number on 0-15. Random float from 0 - 1500 should be better

    if (weapon == STORMBREAKER && damage == 0) // Stormbreaker doesn't miss
        damage = 1;

    if (turnUsed == false && damage != 0) // Attack enemy
    {
        damage += weapon;

        if (otherPlayer->armor != NO_ARMOR) // Other player has armor. Hit that first
        {
            if (otherPlayer->armorHealth - damage < 0)
            {
                // Hit the armor with part of the damage, and the health with the rest
                otherPlayer->health -= (damage - otherPlayer->armorHealth);
                otherPlayer->armorHealth = 0;
                otherPlayer->armor = NO_ARMOR; // Break armor

                game->UpdateArmor({ otherPlayer->location });
            }
            else
                otherPlayer->armorHealth -= damage; // Armor takes all the damage
        }
        else
            otherPlayer->health -= damage; // Health takes all the damage 
    }

    if (receiving == false)
        otherPlayer->Attack(this, true);
    else
        turnUsed = true; // Use up otherPlayer's turn

    if (otherPlayer->health <= 0)
        kills++;

    if (health <= 0) // We died
        Die();

    game->RenderTile(location);
}

void Player::Die()
{
    health = 0;
    winStreak = 0;

    if (champion)
        game->champion = nullptr;

    champion = false;

    DropItem(WEAPON, 3, true);
    game->alivePlayers--;

    map[location.x][location.y].player = nullptr;
    map[location.x][location.y].deadBody = true;
}

bool Player::DropItem(int itemType, int dropRange, bool onDeath)
{
    Point dropPoint = { -1, -1 };

    for (int i = 0; i < dropRange; i++) // Always try to drop at a closer range first
    {
        if (itemType == WEAPON && weapon == NO_WEAPON)
            return false;
        else if (itemType == ARMOR && armor == NO_ARMOR)
            return false;

        if (onDeath) // Death drop.. Take enemy position into consideration
        {
            if (location.y == enemy->location.y) // Horizontally aligned
            {
                if (location.x - enemy->location.x > 0) // Enemy was on the left
                {
                    // Attempt to drop behind us, else just find the first empty spot
                    if (game->IsTileEmpty({ location.x + 1, location.y }))
                        dropPoint = { location.x + 1, location.y };
                    else
                        dropPoint = Search(0, i + 1, { HORIZONTAL_RIGHT, location.x });
                }
                else // Enemy was on the right
                {
                    if (game->IsTileEmpty({ location.x - 1, location.y }))
                        dropPoint = { location.x - 1, location.y };
                    else
                        dropPoint = Search(0, i + 1, { HORIZONTAL_LEFT, location.x });
                }
            }
            else // Vertically aligned
            {
                if (location.y - enemy->location.y > 0) // Enemy was above
                {
                    // Attempt to drop below us
                    if (game->IsTileEmpty({ location.x, location.y + 1 }))
                        dropPoint = { location.x, location.y + 1 };
                    else
                        dropPoint = Search(0, i + 1, { VERTICAL_DOWN, location.y });
                }
                else // Enemy was below
                {
                    // Attempt to drop above us
                    if (game->IsTileEmpty({ location.x, location.y - 1 }))
                        dropPoint = { location.x, location.y - 1 };
                    else
                        dropPoint = Search(0, i + 1, { VERTICAL_UP, location.y });
                }
            }
        }
        else // Regular drop
            dropPoint = Search(0, i + 1); // Search for any empty spot in an (i) tile radius

        if (dropPoint.x == -1)
            continue; // Look farther for an empty spot
        else
            break;
    }

    if (dropPoint.x != -1) // Spot found
    {
        if (itemType == WEAPON)
        {
            map[dropPoint.x][dropPoint.y].item = WEAPON;
            map[dropPoint.x][dropPoint.y].weapon = weapon;
            weapon = NO_WEAPON;
        }
        else if (itemType == ARMOR)
        {
            map[dropPoint.x][dropPoint.y].item = ARMOR;
            map[dropPoint.x][dropPoint.y].armor = armor;
            armor = NO_ARMOR;
            armorHealth = 0;
        }

        game->RenderTile(dropPoint); // Update tile

        return true; // Successful drop
    }

    return false;
}

bool Player::Move(Point destination, bool stuck)
{
    int xDelta = destination.x - location.x;
    int yDelta = (destination.y - location.y) * 2;
    
    bool forceVertical = false;
    if (abs(xDelta) < abs(yDelta) && stuck == false)
        forceVertical = true;
    
    if (xDelta < 0 && map[location.x - 1][location.y].player == nullptr && forceVertical == false) // Move left
    {
        map[location.x][location.y].player = nullptr;
        game->RenderTile(location); // Clear player icon from old spot
        game->UpdateArmor(location); // Clear armor from old spot
    
        location.x--; // Move
    
        map[location.x][location.y].player = this;
        game->RenderTile(location); // Draw player icon in new spot
        return true;
    }
    
    if (xDelta > 0 && map[location.x + 1][location.y].player == nullptr && forceVertical == false) // Move right
    {
        map[location.x][location.y].player = nullptr;
        game->RenderTile(location);
        game->UpdateArmor(location);
    
        location.x++;
    
        map[location.x][location.y].player = this;
        game->RenderTile(location);
        return true;
    }
    
    if (yDelta < 0 && map[location.x][location.y - 1].player == nullptr) // Move up
    {
        map[location.x][location.y].player = nullptr;
        game->RenderTile(location);
        game->UpdateArmor(location);
    
        location.y--;
    
        map[location.x][location.y].player = this;
        game->RenderTile(location);
        return true;
    }
    
    if (yDelta > 0 && map[location.x][location.y + 1].player == nullptr) // Move down
    {
        map[location.x][location.y].player = nullptr;
        game->RenderTile(location);
        game->UpdateArmor(location);
    
        location.y++;
    
        map[location.x][location.y].player = this;
        game->RenderTile(location);
        return true;
    }
    
    if (stuck == false)
        Move(destination, true);
    else // If we tried getting unstuck and still couldn't, start swinging
    {
       Point enemyLocation = Search(PLAYER); // The only thing that can cause us to get stuck is another player, so just fight him

       if (enemyLocation.x != -1)
       {
           state = GOTO_ENEMY;
           destination = enemyLocation;
           enemy = map[enemyLocation.x][enemyLocation.y].player;

           return false;
       }
    }
}

Point Player::Search(int searchType, int searchRange, std::pair<SearchLimit, int> limit)
{
    // Construct Search Area //
    int left = location.x - searchRange;
    int right = location.x + searchRange;
    int top = location.y - searchRange;
    int bottom = location.y + searchRange;

    if (searchType == WEAPON || searchType == ARMOR) // Command prompt char is over twice as tall as it is wide
    {
        top += ((location.y - top) / 2);
        bottom -= ((bottom - location.y) / 2);
    }

    // Clip To Map Size //
    if (left < 0)
        left = 0;
    if (right > mapWidth)
        right = mapWidth;
    if (top < 0)
        top = 0;
    if (bottom > mapHeight)
        bottom = mapHeight;

    // Clip To Limits //
    if (limit.second != -1) // Limit exists
    {
        if (limit.first == HORIZONTAL_RIGHT)
            left = limit.second;
        else if (limit.first == HORIZONTAL_LEFT)
            right = limit.second;
        else if (limit.first == VERTICAL_DOWN)
            top = limit.second;
        else if (limit.first == VERTICAL_UP)
            bottom = limit.second;
    }

    std::vector<Point> matches;
    matches.reserve(50);
    for (int y = top; y < bottom; y++)
    {
        for (int x = left; x < right; x++)
        {
            // Determine Search Type //
            if (searchType == WEAPON && map[x][y].weapon > weapon)
                matches.push_back({ x, y });
            else if (searchType == ARMOR && map[x][y].armor > armor)
                matches.push_back({ x, y });
            else if (searchType == PLAYER && map[x][y].player != nullptr && map[x][y].player != this)
                matches.push_back({ x, y });
            else if (searchType == 0 && game->IsTileEmpty({ x, y }))
                return { x, y }; // Return first empty spot available
        }
    }

    if (matches.size() == 0)
        return { -1, -1 };
    else if (matches.size() == 1)
        return matches[0];

    std::vector<Point> culledMatches;
    culledMatches.reserve(matches.size()); // Wasted memory > Slow allocation time
    // Cull Lesser Objects //
    if (searchType == WEAPON || searchType == ARMOR)
    {
        // Scan For Best Item //
        int bestItem = 0;
        for (unsigned int i = 0; i < matches.size(); i++) // First loop to scan for the best item
        {
            Point item = matches[i];
            if (map[item.x][item.y].armor + map[item.x][item.y].weapon > bestItem) // Armor and weapon can't exist in the same space, so one will always be zero
                bestItem = map[item.x][item.y].armor + map[item.x][item.y].weapon; // No use in having separate checks for each
        }

        for (unsigned int i = 0; i < matches.size(); i++) // Second loop to cull the lesser items
        {
            Point item = matches[i];
            if (map[item.x][item.y].armor + map[item.x][item.y].weapon == bestItem)
                culledMatches.push_back(item);
        }
    }
    else // Player searches don't need culling
        culledMatches = matches;

    std::vector<int> distances;
    distances.reserve(culledMatches.size());

    // Calculate Distances //
    for (unsigned int i = 0; i < culledMatches.size(); i++)
        distances.push_back(culledMatches[i].distance(location));

    // Find Shortest Distance //
    int shortest = INT_MAX;
    int index = 0;
    for (unsigned int i = 0; i < distances.size(); i++)
    {
        if (distances[i] < shortest)
        {
            shortest = distances[i];
            index = i;
        }
    }

    return culledMatches[index];
}

bool Player::WithinRange(Point enemyPlayer)
{
    if (location.x == enemyPlayer.x && abs(location.y - enemyPlayer.y) == 1) // If they're one tile away while aligned on the x-axis
        return true;
    else if (location.y == enemyPlayer.y && abs(location.x - enemyPlayer.x) == 1) // If they're one tile away while aligned on the y-axis
        return true;
    else
        return false;
}

bool Player::ProcessTurn()
{
    switch (state)
    {
    // If we're aimless, that usually means we either just got out of a fight, or
    // the battle has just started. Either way, we need to search for something to do
    // Search Priority: Find Weapon -> Find Armor -> Find Enemy
    case AIMLESS:
    {
        // Try to find better weapons and armor. Only one champion-level weapon can exist on the battlefield, so 
        // if we have one, don't bother looking for another. Similarly, dragon is the best armor, so no need to upgrade
        if (weapon < CHAMPIONS_GREATSWORD || armor < DRAGON)
        {
            Point weaponLocation = Search(WEAPON, SEARCH_RANGE_WEAPONS); // Will return the best weapon in range that's better than your current weapon
            Point armorLocation = Search(ARMOR, SEARCH_RANGE_ARMOR); // Will return best armor in range

            if (weaponLocation.x != -1 && armorLocation.x != -1) // Found both. Determine which to go after
            {
                state = GOTO_ITEM;

                if (map[weaponLocation.x][weaponLocation.y].weapon >= CHAMPIONS_GREATSWORD) // If we find anything above a sword, forget the armor and go get it
                    destination = weaponLocation;
                else if (map[armorLocation.x][armorLocation.y].armor >= MITHRIL) // Mithril and above takes precedance over sword and below
                    destination = armorLocation;
                else // Just go to the closest
                {
                    int distanceToWeapon = location.distance(weaponLocation);
                    int distanceToArmor = location.distance(armorLocation);

                    if (distanceToWeapon < distanceToArmor)
                        destination = weaponLocation;
                    else
                        destination = armorLocation;
                }

                return false; // We've found something to do, but we still need to tell the update function that we haven't used our turn yet
            }
            else if (weaponLocation.x != -1) // Just weapon found
            {
                state = GOTO_ITEM;
                destination = weaponLocation;

                return false;
            }
            else if (armorLocation.x != -1) // Just armor found
            {
                state = GOTO_ITEM;
                destination = armorLocation;

                return false;
            }
        }

        // We're either fully decked out, or there just aren't any better items around us. Search for something to kill
        Point enemyLocation;
        for (unsigned int i = 0; i < mapWidth; i++)
        {
            enemyLocation = Search(PLAYER, i + 1);

            if (enemyLocation.x != -1) // Enemy found
            {
                state = GOTO_ENEMY;
                destination = enemyLocation;
                enemy = map[enemyLocation.x][enemyLocation.y].player;

                return false;
            }
        }
        
        // No enemies left, and no gear to loot
        return true;
    }
    // Heading towards our destination to pick up an item
    case GOTO_ITEM:
    {
        if (map[destination.x][destination.y].item == 0) // Someone stole our shit
        {
            state = AIMLESS;

            return false;
        }

        // Item is still there and we haven't arrived yet. Keep moving
        if (location != destination)
        {
            bool stuck = Move(destination);
            if (stuck == false) // We're stuck
                return false;
        }

        if (location == destination) // We've arrived at our destination
        {
            state = AIMLESS;

            // Pick Up Item //
            if (map[location.x][location.y].item == WEAPON)
            {
                DropItem(WEAPON, 3);
                weapon = map[location.x][location.y].weapon;
            }
            else if (map[location.x][location.y].item == ARMOR)
            {
                DropItem(ARMOR, 3);
                armor = map[location.x][location.y].armor;
                armorHealth = armor;
            }

            map[location.x][location.y].item = 0; // Clear the item from the map
            map[location.x][location.y].weapon = NO_WEAPON;
            map[location.x][location.y].armor = NO_ARMOR;

            game->RenderTile(location);
        }

        return true;
    }
    // Heading towards our destination to kill an enemy. If we're walking and see a good item, pick it up. Similar to aimless
    case GOTO_ENEMY:
    {
        if (enemy->health == 0) // Someone else killed him
        {
            state = AIMLESS;
            enemy = nullptr;

            return false;
        }

        // Enemy is still alive
        destination = enemy->location; // Update our destination in case the enemy moved since last update

        if (WithinRange(enemy->location)) // If we've reached him, engage him
        {
            state = FIGHT;
            
            if (enemy->state != FIGHT) // Engage enemy, unless he's fighting someone else. Just repeatedly stab him in the back in that case..
            {
                enemy->state = FIGHT;
                enemy->enemy = this;
            }

            return false;
        }

        // Enemy is still alive, but we still haven't reached him. Look around us for some better gear on our way to the enemy
        if (weapon < CHAMPIONS_GREATSWORD || armor < DRAGON) // Look for weapon and armor upgrades on the way to the enemy
        {
            Point weaponLocation = Search(WEAPON, SEARCH_RANGE_WEAPONS);
            Point armorLocation = Search(ARMOR, SEARCH_RANGE_ARMOR);

            if (weaponLocation.x != -1 && armorLocation.x != -1)
            {
                state = GOTO_ITEM;

                if (map[weaponLocation.x][weaponLocation.y].weapon >= CHAMPIONS_GREATSWORD)
                    destination = weaponLocation;
                else if (map[armorLocation.x][armorLocation.y].armor >= MITHRIL)
                    destination = armorLocation;
                else
                {
                    int distanceToWeapon = location.distance(weaponLocation);
                    int distanceToArmor = location.distance(armorLocation);

                    if (distanceToWeapon < distanceToArmor)
                        destination = weaponLocation;
                    else
                        destination = armorLocation;
                }

                return false;
            }
            else if (weaponLocation.x != -1)
            {
                state = GOTO_ITEM;
                destination = weaponLocation;

                return false;
            }
            else if (armorLocation.x != -1)
            {
                state = GOTO_ITEM;
                destination = armorLocation;

                return false;
            }
        }

        bool stuck = Move(destination);
        if (stuck == false)
            return false;

        return true;
    }
    // FIGHT!
    case FIGHT:
    {
        if (enemy->health <= 0) // Check if he's alive
        {
            state = AIMLESS;
            enemy = nullptr;

            return false;
        }

        if (WithinRange(enemy->location) == false) // Check if he's still within range
        {
            state = GOTO_ENEMY;

            return false;
        }

        if (enemy->state != FIGHT)
        {
            enemy->state = FIGHT;
            enemy->enemy = this;
        }

        Attack(enemy);

        return true;
    }
    }
}

void Player::Reset()
{
    // Reset Health //
    if (winStreak >= 25) // Omega Champion
        health = 500;
    else if (winStreak >= 10) // Alpha Champion
        health = 400;
    else if (winStreak >= 5)
        health = 300;
    else if (winStreak >= 3)
        health = 250;
    else if (winStreak == 2)
        health = 200;
    else if (winStreak == 1)
        health = 150;
    else
        health = 100;

    health += playerBoost;

    // Reset Weapon //
    if (winStreak >= 25)
        weapon = STORMBREAKER;
    else if (winStreak >= 10)
        weapon = MJOLNIR;
    else if (winStreak > 0)
        weapon = CHAMPIONS_GREATSWORD;
    else
        weapon = NO_WEAPON; // Unarmed

    armorHealth = armor;

    if (champion == true)
        health += championBoost;

    kills = 0;
    turnUsed = false;
    location = { -1, -1 };
    destination = { 0, 0 };
    state = AIMLESS;
    enemy = nullptr;
}

bool Player::Update()
{
    if (health == 0)
        return false;

    int updateAttempts = 0;
    while (updateAttempts < 10 && turnUsed == false)
    {
        turnUsed = ProcessTurn();
    }

    return (health > 0);
}

void Player::Win()
{
    winStreak++;
    champion = true;
    game->champion = this;
}
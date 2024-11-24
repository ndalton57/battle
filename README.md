# Battle
A little 2 day ASCII art 'game' I made back in 2017

It runs in the terminal, but I made this when I was still using Windows 7, so I don't think it supports the shiny new Windows 11 terminal. You'll have to use command prompt or powershell

![battle](https://github.com/user-attachments/assets/5b702808-9986-4bb1-aa18-487494f55222)

You don't really interact with anything. You just watch a bunch of characters battle it out. There's a legend in the main menu that describes all the symbols

`☺` = Character

`!` = Weapon on the ground (Character color changes based on weapon type equipped)

`+` = Armor on the ground (Color indicates rarity/strength)

`[]` = Equipped armor

The beginning of each round is usually a mad dash to grab the most powerful weapons and armor

Weapons are dropped on death, and other characters will upgrade both armor and weapons if they're close enough to see them

You can use the `,` and `.` to speed up the simulation, and `Space` to fast forward to the end of the round

Last man standing will be green the next round, and they'll be given an additional +50 health along with a more powerful weapon (which can be dropped if they're killed)

Repeat wins will grant the winner additional bonuses (allowing them to snowball power a bit)

Large maps are tougher to get repeat winners, so you can adjust the bonuses in the menu

# Install
You can just run the `battle.exe` from the published release.

Or you can point your own compiler at the `.cpp` files. There's like 5 of them total, so I didn't bother making a build config

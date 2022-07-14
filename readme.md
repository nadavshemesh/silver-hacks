
# Infogrames/ Spiralhouse Silver Hacks

## Introduction
Simple and fun hacks made for the game '[Silver](https://en.wikipedia.org/wiki/Silver_(video_game))'.

I provided a cheat engine's table and a cpp program, eventually the program will catch up to the table.

_The c++ program is for windows only (sorry non-windows users)._


<img src="./examples/grand_fuge_edit.gif" />

### Check out more GIFS [Here](./examples/examples.md)

#

## On this version of the cheat engine's table, you can:
    0. Control the player's stats, make magic and abilities infinite. (Alt+1 to freeze, edit values in the table)
    1. Get all items (ALT+I, see the note below)
    2. Teleport all chars in the room to wherever you want (ALT+B)
    3. Teleport your character (ALT+T)
    4. Teleport from any room/ level to any room/ level (Via the table)
    5. Unlock the doors whenever you want (Alt+2)
    6. Save whenever you want (ALT+S)
    7. Vac all enemies to match the player's position (ALT+V)
    8. Change the room's state (Via the table)
    9. Create any enemy you'd like (ALT+E with spawn type in the table)
    10. Summon characters (Via the table & using the summon skill) - Uncheck before exiting a room!

    Important to note:
    ------------------
    Hacks number 1, 9 and 6 are injected to a game loop, therefore a 'Used' flag is set to one so that it wont trigger infinitely (otherwise the game will crash), after you use a hack like this, press its key combination again to deactivate it (or deactivate via the table by unchecking it) and then press ALT+Q to reset the flag back to 0 (otherwise the hacks wont work the next time).

    Another important note:
    -----------------------
    Currently, the hack "Get all items" supplies the player will all items without checking what items he has first, so getting an item that a player cannot obtain twice (like a magic spell) will cause a crash. therefore you should use this hack before you obtain such an item in the game (or you can edit the script on the table, here are [all the items by number](https://github.com/nadavshemesh/silver-reversing-sct/blob/master/docs/catalog/items.md))

## On this version of the program (win_launcher.exe) you can:
    1. Teleport your character (CTRL+T)
    2. Teleport all chars in the room to wherever you want (CTRL+B)
    2. Teleport all enemies in the room to the player's position (CTRL+V)
    3. Unlock the doors whenever you want (CTRL+D)

## Whats next
I plan to make .sct injection, so that together with [this project](https://github.com/nadavshemesh/silver-reversing-sct) you will be able to write script files (basically do anything the game does) and activate those whenever you want. (basically writing your own cheats and mods)

## Build

Assuming you're in this readme.md directory:
```
gcc win_launcher.cpp -lstdc++ -o .\win_launcher.exe
```
_Note: currently, the program must be run with admin privileges in order to work_

## Run
Double click on win_launcher.exe

Or from a command line:
```
.\win_launcher.exe
```
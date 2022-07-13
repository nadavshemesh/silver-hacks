
# Infogrames/ Spiralhouse Silver Hacks

## Introduction
Simple and fun hacks made for the game '[Silver](https://en.wikipedia.org/wiki/Silver_(video_game))'.

I provided a cheat engine's table and a cpp program, eventually the program will catch up to the table.


<img src="./examples/grand_fuge_edit.gif" />

### Check out more GIFS [Here](./examples/examples.md)

#

## On this version of the cheat engine's table, you can:
    0. Control the player's stats, make magic and abilities infinite. (Alt+1)
    1. Get all items (ALT+I)
    2. Teleport all chars in the room to wherever you want (ALT+T)
    3. Teleport from any room/ level to any room/ level (Via the table)
    4. Save whenever you want (ALT+S)
    5. Teleport your character (ALT+T)
    6. Vac all enemies to match the player's position (ALT+V)
    7. Change the room's state (Via the table)
    8. Create any enemy you'd like (Via the table)
    9. Summon any character you'd like (Via the table)
    10. Unlock the doors whenever you want (Alt+2)

## On this version of the program (win_launcher.exe, it's for windows only, sorry) you can:
    1. Teleport your character (CTRL+T)
    2. Teleport all chars in the room to wherever you want (CTRL+B)
    2. Teleport all enemies in the room to the player's position (CTRL+V)
    3. Unlock the doors whenever you want (CTRL+D)

## The ultimate goal of this project
Is to make .sct injection, so that together with [this project](https://github.com/nadavshemesh/silver-reversing-sct) you could write script files (basically do anything the game does) and activate those whenever you want.

## Build

Assuming you're in the readme directory:
```
gcc win_launcher.cpp -lstdc++ -o .\win_launcher.exe
```
_Note: the program must be run with admin privileges in order to work_

## Run
Double click on win_launcher.exe

or from a command line:
```
.\win_launcher.exe
```
# 304pacman
<p align="center">
  <img src="https://github.com/payet-s/304pacman/blob/master/sd/apps/pacman304/icon.png">
</p>
<p align="center">
  A C++ ASCII art Pac-man project on Wii
</p>

Dependencies
------------
 * [DevkitPPC](http://devkitpro.org/wiki/Getting_Started/devkitPPC) (cross-compilation)
 * [SDL Wii](https://github.com/dborth/sdl-wii)

Build
-----
Run ```make```.
Two valid executable files will be generated:
 1. __304pacman.dol__
 2. __304pacman.elf__

Rename them **boot.dol** and **boot.elf** and replace the old [boot.elf](https://github.com/payet-s/304pacman/blob/master/sd/apps/pacman304/boot.elf)
file by one of them if you want to run it on Wii through the **Homebrew Channel**.

How to launch
-------------
1. Emulator
 * [Dolphin Emulator](https://www.dolphin-emu.org/)
 * Open (CTRL+O)
 * Browse to the [boot.elf](https://github.com/payet-s/304pacman/blob/master/sd/apps/pacman304/boot.elf) or a valid **dol**/**elf** file
2. Wii
 * Copy the [sd folder contents](https://github.com/payet-s/304pacman/tree/master/sd) to the root of your external device (SD/USB)
 * Put your external device into your Wii and run the __Homebrew Channel__
 * Choose __304pacman__ and __Launch__

Game Controls (Wii Remote)
--------------------------
| Button                                           | Description  |
|--------------------------------------------------|--------------|
| <kbd>+</kbd>                                     | Pause button |
| <kbd>-</kbd>                                     | Reset button |
| <kbd>HOME</kbd>                                  | Exit button  |
| <kbd>←</kbd><kbd>↑</kbd><kbd>→</kbd><kbd>↓</kbd> | Move buttons |

Caption
-------
| Character    | Description |
|--------------|-------------|
| <kbd> </kbd> | Space       |
| <kbd>.</kbd> | Pacgum      |
| <kbd>P</kbd> | Player      |
| <kbd>F</kbd> | Ghost       |

How to play
-----------
Choose a map with the <kbd>←</kbd> and <kbd>→</kbd> arrows.
Press <kbd>A</kbd> to confirm your choice.
Then avoid the ghost and grab all pacgums of the map to win.

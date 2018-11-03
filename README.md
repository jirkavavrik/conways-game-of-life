# conways-game-of-life

DOWNLOAD: https://github.com/jirkavavrik/conways-game-of-life/releases/latest

This app simulates J. H. Conway's cellular automaton "Game of life", see https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life for comprehensive article about this.

Cells are generated randomly at start (50% are live, 50% dead), however you can toggle each cell's state by clicking on it (I recommend doing this when the game is paused -you can pause with spacebar or the Pause/Break key).
The default grid size is 34*45 cells, you can anytime change it by simply resizing the window of the app (this resets the cells to random state again).

You can control speed of simulation with up and down arrow keys.

Program never ends, you have to terminate it by pressing Q, Alt+F4 or the close button.

You can press F1 anytime for help and info.


## BUILD INFO: (v3.0 and later)

-Windows 64-bit binaries are distributed with each release

Linux build: (also works on Cygwin)
1. install libSDL2-dev - either as a package or from https://www.libsdl.org/download-2.0.php
2. enter the 'src' directory
3. `make`
4. run with `make run`

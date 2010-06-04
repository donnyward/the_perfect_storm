#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"

typedef struct gameModule
{
/*
-Current level and score
-Whether or not the game is paused
-Whether the game is playing or in menu
-Pointer to next tetromino (the one that will be put up when the current one is cleared)
-2dimensional array of pointers to blocks (each element in the array represents an x,y on the playing grid, null if no block there).
-Pointer to the current tetromino (the one thats falling right now)
*/
	int level;
	int score;
	boolean paused;
	tetromino * next;
	tetromino * current;
	block * spaces[SIZE_X][SIZE_Y];
} game;

/*
game_getScore  returns the current score of the player (integer)
game_getLevel  returns the current level the player is on (integer)
game_getBlockAtPos  returns a pointer to the block that is at a specific coordinate (using the 2d array). Returns null if no block there.
game_setBlockToPos  takes a pointer to a block, x, and y coordinates, and attempts to place that block at this position in the array, will also remove block from previous position if move is possible. Returns 1 on successful set, 0 on fail. fails when a block is already occupying the intended destination.
game_removeBlockAtPos  takes x and y coordinates, and removes the block from that point on the array list.
game_draw  gets the current score, level, position of all blocks, debug messages (if applicable) and draws them to the screen.
game_getInput  awaits player input, and either rotates the tetromino or moves the tetromino (else do nothing)
game_pause - pauses game, suspends everything, pops up menu with "resume" or "quit"
game_clear - ends current game. an argument will be passed to tell the game if the game ended because the player quit or because the player lost (or the player won)..
game_create - loads the game part of the game, and begins game play.
*/

#endif

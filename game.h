#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"
#include "tetromino.h"
//#include "block.h"


typedef enum
{
	reason_quit, //player quit
	reason_loss, //player lost
} gameOverReason_t;


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
	block * pos[SIZE_X][SIZE_Y];
} gameModule;

//returns the current score of the player (integer)
int g_getScore();

//returns the current level the player is on (integer)
int g_getLevel();

//returns a pointer to the block that is at a specific coordinate (using the 2d array).
//Returns null if no block there.
block * g_getBlockAtPos(int x, int y);

//takes a pointer to a block, x, and y coordinates, and attempts to place that block at
//this position in the array, will also remove block from previous position if move is possible.
//Returns 1 on successful set, 0 on fail. fails when a block is already occupying the intended destination.
boolean g_setBlockToPos(block * b, int x, int y);

//takes a pointer to a block and removes it from wherever it was. false if it was nowhere (i.e. just created)
boolean g_removeBlockFromPos(block * b);

//gets the current score, level, position of all blocks and draws them to the screen.
void g_draw();

//awaits player input, and either rotates the tetromino or moves the tetromino (else do nothing)
void g_doInput();

//pauses game, suspends everything, pops up menu with "resume" or "quit"
void g_pause();

//ends current game. an argument will be passed to tell the game if the game ended
//because the player quit or because the player lost (or the player won)..
void g_clear(gameOverReason_t r);

//shut down the program
void g_end();

//loads the game part of the game, and begins game play.
void g_create();

//initial booting of the game, inits sdl and creates the screen, also sets initial values
//for the gameModule and menu.ends with the menu displayed waiting for input
boolean g_boot();

//takes a .bmp file and draws it on the screen
boolean draw_bmp(char * filename);

void debug_msg(char * msg); //prints messages to terminal if debug mode is enabled



#endif

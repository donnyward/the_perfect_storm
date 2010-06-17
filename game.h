#ifndef __GAME_H__
#define __GAME_H__

#include "types.h"
#include "tetromino.h"
//#include "block.h"
#include "SDL/SDL.h"




typedef struct gameModule
{
/*
-Current level and score
-Whether or not the game is paused
-Whether the game is playing or in menu
-Pointer to next tetromino (the one that will be put up when the current one is cleared)
-2dimensional array of pointers to blocks (each element in the array represents an x,y on the playing grid, null if no block there).
-Pointer to the current tetromino (the one that's falling right now)
*/
	boolean exitGameYet; //when the user wants to exit to dos this will become true
	int level;
	int score;
	int lines;
	gamestate_t state; //state of the game
	tetromino * next;
	tetromino * current;
	block * pos[SIZE_X][SIZE_Y];
} gameModule;

//===============================================
//Game functions below
//===============================================
//returns the current score of the player (integer)
int g_getScore();

//returns the current level the player is on (integer)
int g_getLevel();

//returns true if coords are out of bounds, false if not
boolean g_isLocOutOfBounds(int x, int y);

//returns a pointer to the block that is at a specific coordinate (using the 2d array).
//Returns null if no block there.
block * g_getBlockAtPos(int x, int y);

//takes a pointer to a block, x, and y coordinates, and attempts to place that block at
//this position in the array, will also remove block from previous position if move is possible.
//Returns 1 on successful set, 0 on fail. fails when a block is already occupying the intended destination.
boolean g_setBlockToPos(block * b, int x, int y);

//takes a pointer to a block and removes it from wherever it was. false if it was nowhere (i.e. just created)
boolean g_removeBlockFromPos(block * b);

//ends current game. an argument will be passed to tell the game if the game ended
//because the player quit or because the player lost (or the player won)..
void g_clear(gameOverReason_t r);

//shut down the program
void g_end();

//call this when starting a new game
//sets inital gameModule values, loads the game part of the game, and begins game play.
void g_create();

//loads an image, gets it optimized and ready for blitting
SDL_Surface * g_loadImage(char * filename);

//loads the correct image for a type of tetromino
SDL_Surface * g_loadBlockImage(tetroShape_t type);

//takes a surface (containing a loaded image) and applys it to the main screen surface
//with or without an offset
//the screen must be flipped once all the adding is done
boolean g_addSurface(int x, int y, SDL_Surface * source, SDL_Surface * dest);

void debug_msg(char * msg); //prints messages to terminal if debug mode is enabled

//main game loop. point of no return
void g_loop();

//gets input and makes changes to the game entitys (whether in game or in menu) based on input
void g_handleInput();

//take new input if any and apply changes to the objects in the game based on input and other behaviors
void g_updateGame();

//returns the pixel coordinates of i and j thru x and y.
void g_getImageCoords(short i, short j, short * x, short * y);

//gets the current score, level, position of all blocks and draws them to the screen.
void g_drawGame();

//setup some initial values (not all, just the necessary ones at the time) for gameModule at program startup
//also initializes the 2d array to null
void g_init();

//clears the grid. all blocks are freed from memory, if they are linked to a tetromino, that is cleared first
//result is entire grid is NULL
void g_clearGrid();

//call this when movement down is blocked. sets the tetromino into place (and tetro_clear's it), checks for full lines
//and acts accordingly. then calls next tetromino into play, and creates the next one that waits in stasis
void g_onDownBlocked();

//check to see if a row is filled with blox returns an 
boolean g_checkFullRow(int row);

//void g_flashBlocks()

//drops the blocks starting at startRow and above down count times.
void g_dropAboveBlocksDown(int startRow, int count);

#endif

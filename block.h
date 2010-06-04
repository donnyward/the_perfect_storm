#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "types.h"

struct tetromino; //declaration here because it is used below by block definition

typedef struct block
{
	//coordinates of block
	int x;
	int y;
	tetromino * parent; //pointer to the parent tetromino object
	boolean sleep; //a sleeping block doesnt move or do much of anything
	tetroShape_t type; //the type of block, corresponds to the type of tetromino it is or was a part of (or blank if it is created to fill the screen upon losing)
} block;

//creates a new block. returns a pointer to the new block. returns null if block wasnt created properly
block * block_create(tetroShape_t type, tetromino * parent);

//removes a block *b. returns true if successfully removed. fails if block still has a parent.
boolean block_clear(block * b);

//teleports a block to (x, y). returns true on success. fails if there is another block at the destination, or if the destination is off grid.
boolean block_teleport(block * b, int x, int y);

//moves the block one notch in the direction specified
boolean block_move(block * b, dir_t dir);

//returns the x coordinate of the block
int block_getLocX(block * b);

//returns the y coordinate of the block
int block_getLocY(block * b);

//set the x coordinate of this block, fails if coord is out of bounds
boolean block_setLocX(block * b, int x);

//set the y coordinate of this block, fails if coord is out of bounds
boolean block_setLocY(block * b, int y);

//returns true if block is asleep, false if awake
boolean block_isSleep(block * b);

//put block to sleep, returns false if block was already asleep
boolean block_doSleep(block * b);

//wake up a block, returns false if block was already awake
boolean block_doWake(block * b);

//returns a pointer to the parent of this block
tetromino * block_getParent(block * b);

//sets the parent of a block to the tetromino given in the arguments
void block_setParent(block * b, tetromino * parent);

//sets type of a block to a tetroShape_t type. returns false if a bad type is given.
boolean block_setType(block * b, tetroShape_t type);

//returns the type of a block
tetroShape_t block_getType(block * b);

#endif
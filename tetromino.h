#ifndef __TETROMINO_H__
#define __TETROMINO_H__

//#include "block.h"
#include "types.h"

typedef struct block
{
	//coordinates of block
	int x;
	int y;
	struct tetromino * parent; //pointer to the parent tetromino object
	boolean sleep; //a sleeping block doesnt move or do much of anything
	tetroShape_t type; //the type of block, corresponds to the type of tetromino it is or was a part of (or blank if it is created to fill the screen upon losing)
	dir_t moveDir; //direction to move next move
} block;


//put in a variable that keeps track of what the NEXT move is... so when you call move,
//it will just look where to move next (which is set by g_handleInput()) and then make the move
typedef struct tetromino
{
	block * children[TETRO_SIZE]; //the 4 child blocks that make of this tetromino
	tetroShape_t type;
	boolean sleep;
	dir_t nextMoveX;
	dir_t nextMoveY;
	dir_t nextMoveDir;
	
} tetromino;

//===============================================
//Block prototypes below
//===============================================
//creates a new block. returns a pointer to the new block. returns null if block wasnt created properly
block * block_create(tetroShape_t type, struct tetromino * parent);

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
struct tetromino * block_getParent(block * b);

//sets the parent of a block to the tetromino given in the arguments
void block_setParent(block * b, struct tetromino * parent);

//sets type of a block to a tetroShape_t type. returns false if a bad type is given.
boolean block_setType(block * b, tetroShape_t type);

//returns the type of a block
tetroShape_t block_getType(block * b);


//===============================================
//Tetromino prototypes below
//===============================================
//creates a new tetromino of type, creates its child blocks,
//links them to their parent, and returns a pointer to the newly created tetromino.
//returns null if the tetromino wasn't created properly.
tetromino * tetro_create(tetroShape_t type);

//puts child blocks to sleep, unlinks children from parent, frees tetromino from memory,
boolean tetro_clear(tetromino * t);

//move this tetromino one notch in direction dir, fails if any child block is blocked.
boolean tetro_move(tetromino * t, dir_t dir);

//teleports tetromino to starting position by teleporting its child blocks.
//position is based on it's type. fails if any of the child blocks fail to get into position,
//at which point the game is lost.
boolean tetro_moveStart(tetromino * t);

//rotates the tetromino 90 degrees clockwise. returns false if rotated position is blocked.
boolean tetro_rotate(tetromino * t);

//returns true if tetromino is sleeping, false if awake
boolean tetro_isSleep(tetromino * t);

//makes tetromino sleep, fails if tetromino already asleep
boolean tetro_doSleep(tetromino * t);

//makes tetromino wake, fails if tetromino already awake
boolean tetro_doWake(tetromino * t);

//sets type of a tetro to a tetroShape_t type. returns false if a bad type is given.
boolean tetro_setType(tetromino * t, tetroShape_t type);

//returns the type of a tetromino
tetroShape_t tetro_getType(tetromino * t);

#endif

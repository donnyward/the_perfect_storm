#ifndef __TETROMINO_H__
#define __TETROMINO_H__

#include "block.h"
#include "types.h"

typedef struct tetromino
{
	block * children[TETRO_SIZE]; //the 4 child blocks that make of this tetromino
	tetroShape_t type;
	boolean sleep;
} tetromino;

//creates a new tetromino of type, creates its child blocks,
//links them to their parent, and returns a pointer to the newly created tetromino.
//returns null if the tetromino wasn't created properly.
tetromino * tetro_create(tetroShape_t type);

//puts child blocks to sleep, unlinks children from parent, frees tetromino from memory,
//checks to see if any lines were completed, then moves the next tetromino to the start
//and begins it's descent
void tetro_clear(tetromino * t);

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

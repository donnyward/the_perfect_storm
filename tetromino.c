//#include "block.h"
#include "tetromino.h"
#include "types.h"
#include "stdlib.h" //for malloc and NULL and rand()
#include <stdio.h>
#include "game.h"

//starting coords for tetrominos at the top of the playing field (in abstract coords)
coord_t startCoord[] = 
{
	//I SHAPE
	{ {3, 4, 5, 6}, {16, 16, 16, 16} },
	
	//J SHAPE
	{ {3, 4, 5, 5}, {16, 16, 16, 15} },
	
	//L SHAPE
	{ {3, 4, 5, 3}, {16, 16, 16, 15} },
	
	//O SHAPE
	{ {4, 5, 4, 5}, {16, 16, 15, 15} },
	
	//S SHAPE
	{ {4, 5, 3, 4}, {16, 16, 15, 15} },
		
	//T SHAPE
	{ {3, 4, 5, 4}, {16, 16, 16, 15} },

	//Z SHAPE
	{ {3, 4, 4, 5}, {16, 16, 15, 15} }
};

//these numbers are simply offsets from the pivot block as point of reference (0, 0) is the pivot point
//coords are with y increasing upward
coord_t rotation_I[] = 
{
	//0th rotation coords
	{ {-1,0,1,2}, {0,0,0,0} },
	
	//1th rotation
	{ {0,0,0,0}, {1,0,-1,-2} },
	
	//2nd rotation
	{ {1,0,-1,-2}, {0,0,0,0} },
	
	//3rd rotation
	{ {0,0,0,0}, {-1,0,1,2} }
};

coord_t rotation_J[] = 
{
	//0th rotation coords
	{ {-1,0,1,1}, {0,0,0,-1} },
	
	//1th rotation
	{ {0,0,0,-1}, {1,0,-1,-1} },
	
	//2nd rotation
	{ {1,0,-1,-1}, {0,0,0,1} },
	
	//3rd rotation
	{ {0,0,0,1}, {-1,0,1,1} }
};

coord_t rotation_L[] = 
{
	//0th rotation coords
	{ {-1,0,1,-1}, {0,0,0,-1} },
	
	//1th rotation
	{ {0, 0, 0, -1}, {1,0,-1,1} },
	
	//2nd rotation
	{ {1,0,-1,1}, {0,0,0,1} },
	
	//3rd rotation
	{ {0,0,0,1}, {-1,0,1,-1} }
};
/*
coord_t rotation_O[] = 
{
	//0th rotation coords
	{ {0,1,0,1}, {0,0,-1,-1} },
	
	//1th rotation
	{ {0,1,0,1}, {0,0,-1,-1} },
	
	//2nd rotation
	{ {0,1,0,1}, {0,0,-1,-1} },
	
	//3rd rotation
	{ {0,1,0,1}, {0,0,-1,-1} }
};
*/
coord_t rotation_S[] = 
{
	//0th rotation coords
	{ {0,1,-1,0}, {0,0,-1,-1} },
	
	//1th rotation
	{ {0,0,-1,-1}, {0,-1,1,0} },
	
	//2nd rotation
	{ {0,-1,1,0}, {0,0,1,1} },
	
	//3rd rotation
	{ {0, 0, 1,1}, {0,1,-1,0} }
};

coord_t rotation_T[] = 
{
	//0th rotation coords
	{ {-1,0,1,0}, {0,0,0,-1} },
	
	//1th rotation
	{ {0,0,0,-1}, {1,0,-1,0} },
	
	//2nd rotation
	{ {1,0,-1,0}, {0,0,0,1} },
	
	//3rd rotation
	{ {0,0,0,1}, {-1,0,1,0} }
};

coord_t rotation_Z[] = 
{
	//0th rotation coords
	{ {-1,0,0,1}, {0,0,-1,-1} },
	
	//1th rotation
	{ {0,0,-1,-1}, {1,0,0,-1} },
	
	//2nd rotation
	{ {1,0,0,-1}, {0,0,1,1} },
	
	//3rd rotation
	{ {0,0,1,1}, {-1,0,0,1} }
};

dir_t oppositeDir[] = 
{
	DIR_SOUTH, DIR_SOUTHWEST, DIR_WEST, DIR_NORTHWEST, DIR_NORTH,
	DIR_NORTHEAST, DIR_EAST, DIR_SOUTHEAST, DIR_NONE
};

//===============================================
//Block functions below
//===============================================
block * block_create(tetroShape_t type, tetromino * parent)
{
	//allocate space for block
	block * b = (block *)(malloc(sizeof(block)));

	//set type of block to the one given
	if ( !block_setType(b, type) )
	{
		//error
		return NULL;
	}
	
	//set parent of this block
	block_setParent(b, parent);
	b->x = X_VOID;
	b->y = Y_VOID;
	b->moveDir = DIR_NONE;
	b->sleep = true;
	
	return b;
}


boolean block_clear(block * b)
{
	if (b == NULL)
	{
		debug_msg("[block_clear]: NULL argument passed!\n");
		return false;
	}
	
	if ( block_getParent(b) != NULL ) //block still has a parent, do not proceed
	{
		return false;
	}
	
	free(b);
	return true;
}

boolean block_teleport(block * b, int x, int y)
{
	return g_setBlockToPos(b, x, y);
}


boolean block_move(block * b, dir_t dir)
{
	int x = block_getLocX(b);
	int y = block_getLocY(b);
	
	if ( dir == DIR_WEST )
	{
		x -= 1;
	}
	else if  ( dir == DIR_EAST )
	{
		x += 1;
	}
	else if ( dir == DIR_NORTH )
	{
		y += 1;
	}
	else if ( dir == DIR_SOUTH )
	{
		y -= 1;
	}
	else if ( dir == DIR_NORTHWEST )
	{
		x -= 1;
		y += 1;
	}
	else if ( dir == DIR_SOUTHWEST )
	{
		x -= 1;
		y -= 1;
	}
	else if ( dir == DIR_NORTHEAST )
	{
		x += 1;
		y += 1;
	}
	else if ( dir == DIR_SOUTHEAST )
	{
		x += 1;
		y -= 1;
	}
	else if ( dir == DIR_NONE )
	{
		//do nothing
		return true;
	}
	else //bad dir
	{
		return false;
	}
	
	return block_teleport(b, x, y);
}

int block_getLocX(block * b)
{
	return b->x;
}

int block_getLocY(block * b)
{
	return b->y;
}

//set the x coordinate of this block, fails if coord is out of bounds
boolean block_setLocX(block * b, int x)
{
	if ( g_isLocOutOfBounds(x, NIL) )
	{
		debug_msg("[block_setLocX]: tried to set to an x out of bounds!\n");
		return false;
	}
	
	b->x = x;
	return true;
}

//set the y coordinate of this block, fails if coord is out of bounds
boolean block_setLocY(block * b, int y)
{
	if ( g_isLocOutOfBounds(NIL, y) )
	{
		debug_msg("[block_setLocY]: tried to set to an y out of bounds!\n");
		return false;
	}
	
	b->y = y;
	return true;
}

boolean block_isSleep(block * b)
{
	return b->sleep;
}

boolean block_doSleep(block * b)
{
	if ( block_isSleep(b) ) //already asleep
		return false;
	else
	{
		b->sleep = true;
		return true;
	}
}

boolean block_doWake(block * b)
{
	if ( !block_isSleep(b) ) //already awake
		return false;
	else
	{
		b->sleep = false;
		return true;
	}
}

tetromino * block_getParent(block * b)
{
	return b->parent;
}

void block_setParent(block * b, tetromino * parent)
{
	b->parent = parent;
}

boolean block_setType(block * b, tetroShape_t type)
{
	if ( type == TETRO_RANDOM )
	{
		//
	}
	
	//bad data,
	if ( type != TETRO_I && type != TETRO_J && type != TETRO_L && type != TETRO_O && type != TETRO_S && type != TETRO_T && type != TETRO_Z && type != TETRO_DEAD )
		return false;
	else
	{
		b->type = type;
		return true;
	}
}

tetroShape_t block_getType(block * b)
{
	return b->type;
}

//===============================================
//Tetromino functions below
//===============================================
tetromino * tetro_create(tetroShape_t type)
{
	int i;
	int random;
	tetromino  * t = (tetromino *)(malloc(sizeof(tetromino)));
	block * b;
	
	if ( type == TETRO_RANDOM )
	{
		//random number from 0 to 6
		random = (rand()/(RAND_MAX+1.0)) * (TETRO_SHAPES-1);
		printf("[tetro_create]: random number = %d\n", random);
		type = random;
		
	//	type = TETRO_S;
	}
	
	if ( !tetro_setType(t, type) )
		return NULL;
		
	for ( i = 0; i < TETRO_SIZE; i++ ) //create each child block and assign it to the tetromino
	{
		b = block_create(type, t);
		if ( b == NULL )
			return NULL;
			
		t->children[i] = b;
	}
	
	t->sleep = true;
	t->nextMoveX = DIR_NONE;
	t->nextMoveY = DIR_NONE;
	t->nextMoveDir = DIR_NONE;
	t->rotation = 0;
	t->tryRotate = false;
	
	return t;
}

boolean tetro_clear(tetromino * t)
{
	int i;
	block * b;
	
	for ( i = 0; i < TETRO_SIZE; i++ )
	{
		b = t->children[i];
		if ( !block_doSleep(b) ) //put child to sleep
		{
			printf("[tetro_clear]: one of the children was already asleep! weird\n");
			//return false;
		}
			
		block_setParent(b, NULL); //unlink from parent	
	}
	
	free(t); //remove tetromino from memory

	return true;
}

boolean tetro_move(tetromino * t, dir_t dir)
{
	int i;
	int j;
	int x, y;
	int rot = t->rotation;
	block * b;
	dir_t dX, dY;
	boolean makeTwoMoves = true;
	//boolean done = false;
	dir_t dirForwardOrBack = DIR_NONE; //north = forward, south = back
	
	if ( dir == DIR_NONE )
		return true;
	
//===============================================
//handles diagonal input
	switch (dir)
	{
		case DIR_NORTHWEST:
			dX = DIR_WEST;
			dY = DIR_NORTH;
			break;
		case DIR_NORTHEAST:
			dX = DIR_EAST;
			dY = DIR_NORTH;
			break;
		case DIR_SOUTHWEST:
			dX = DIR_WEST;
			dY = DIR_SOUTH;
			break;
		case DIR_SOUTHEAST:
			dX = DIR_EAST;
			dY = DIR_SOUTH;
			break;
		default:
			makeTwoMoves = false;
			break;
	}
	
	if (makeTwoMoves)
	{
		//move x direction first, then in y direction
		if ( tetro_move(t, dX) )
		{
			if ( tetro_move(t, dY) )
				return true;
			else
				return false; //so we can report g_onDownBlocked()
		}
		return false;
	}
//===============================================


/*
	//make it so either all of the blocks will move in the direction, or none of them will
	//blocks must be moved in the correct order to prevent tetromino from tripping over itself
	//blocks are indexed left to right, top to bottom, 0-3
	//depends on the move direction and rotation
	
moving left:
	rotation 0 = 0-3
	rotation 1 = 3-0
	rotation 2 = 3-0
	rotation 3 = 0-3
	
moving right:
	rotation 0 = 3-0
	rotation 1 = 0-3
	rotation 2 = 0-3
	rotation 3 = 3-0

moving up:
	rotation 0 = 0-3
	rotation 1 = 0-3
	rotation 2 = 3-0
	rotation 3 = 3-0
	
moving down:
	rotation 0 = 3-0
	rotation 1 = 3-0
	rotation 2 = 0-3
	rotation 3 = 0-3
*/
	switch (dir)
	{
		case DIR_WEST:
			if ( rot == 0 || rot == 3 )
				dirForwardOrBack = DIR_NORTH;
			else if ( rot == 1 || rot == 2 )
				dirForwardOrBack = DIR_SOUTH;
			else
				printf("[tetro_move]: weird rotation: %d\n", rot);
			break;
		case DIR_EAST:
			if ( rot == 1 || rot == 2 )
				dirForwardOrBack = DIR_NORTH;
			else if ( rot == 0 || rot == 3 )
				dirForwardOrBack = DIR_SOUTH;
			else
				printf("[tetro_move]: weird rotation: %d\n", rot);
			break;
		case DIR_NORTH:
			if ( rot == 0 || rot == 1 )
				dirForwardOrBack = DIR_NORTH;
			else if ( rot == 2 || rot == 3 )
				dirForwardOrBack = DIR_SOUTH;
			else
				printf("[tetro_move]: weird rotation: %d\n", rot);
			break;
		case DIR_SOUTH:
			if ( rot == 2 || rot == 3 )
				dirForwardOrBack = DIR_NORTH;
			else if ( rot == 0 || rot == 1 )
				dirForwardOrBack = DIR_SOUTH;
			else
				printf("[tetro_move]: weird rotation: %d\n", rot);
			break;
		default:
			printf("[tetro_move]: weird dir, should be non-diagonal: %d\n", dir);
			return false;
			break;
	}

	if ( dirForwardOrBack == DIR_NORTH ) //moves in forward order
	{
		for ( i = 0; i < TETRO_SIZE; i++ )
		{
			b = t->children[i];

			if ( !block_move(b, dir) )
			{
				//go back and unmove all the previous blocks
				for ( j = i-1; j >= 0; j-- )
				{
					b = t->children[j];
					block_move(b, oppositeDir[dir]);
				}
		
				return false;
			}
		}
	}
	else if ( dirForwardOrBack == DIR_SOUTH ) //moves in reverse order
	{
		for ( i = TETRO_SIZE-1; i >= 0; i-- )
		{
			b = t->children[i];

			if ( !block_move(b, dir) )
			{
				//go back and unmove all the previous blocks
				for ( j = i+1; j < TETRO_SIZE; j++ )
				{
					b = t->children[j];
					block_move(b, oppositeDir[dir]);
				}
		
				return false;
			}
		}
	}
	else
	{
		printf("[tetro_move]: weird dirForwardOrBack: %d\n", dirForwardOrBack);
		return false;
	}


	return true;
}

boolean tetro_moveToStart(tetromino * t)
{
	int i;
	int x, y, type;
	block * b;
	block * blockThatWasAlreadyThere;
	boolean result = true;
	
	type = tetro_getType(t);
	
	for ( i = 0; i < TETRO_SIZE; i++ )
	{
		b = t->children[i];
		x = startCoord[type].xCoord[i];
		y = startCoord[type].yCoord[i];
		
		if ( !block_teleport(b, x, y) )
		{
			//put the block there anyway so you see it placed on the screen as the game ends
			result = false;
			g_removeBlockFromPos( g_getBlockAtPos(x, y) );
			block_teleport(b, x, y);
		}
	}
	
	if ( !tetro_doWake(t) )
		debug_msg("[tetro_moveToStart]: tetromino already awake?\n");
	
	return result;
}

//rotates the tetromino 90 degrees clockwise. returns false if rotated position is blocked.
boolean tetro_rotate(tetromino * t)
{
	block * b = NULL, * pivot = NULL; //if the * is not in front of pivot, pivot becomes block (instead of block *)
	int x, y;
	int xDest[TETRO_SIZE], yDest[TETRO_SIZE];
	int i;
	boolean doRotate = true;
	int rotationIndex;
	
	pivot = t->children[t->pivot];
	
	if (pivot == NULL)
		return false;
		
	x = block_getLocX(pivot);
	y = block_getLocY(pivot);
	
	if (t->rotation == 3)
		rotationIndex = 0;
	else
		rotationIndex = t->rotation + 1;

	printf("[tetro_rotate]: starting rotation = %d\n", t->rotation);
	
	//successful rotate means the end positions for all 3 non pivot blocks are NULL
	for ( i = 0; i < TETRO_SIZE; i++ )
	{
		//offset from pivot + x/y coord of pivot
		switch (tetro_getType(t))
		{
			case TETRO_I:
				xDest[i] = rotation_I[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_I[rotationIndex].yCoord[i] + y;
				break;
			case TETRO_J:
				xDest[i] = rotation_J[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_J[rotationIndex].yCoord[i] + y;			
				break;
			case TETRO_L:
				xDest[i] = rotation_L[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_L[rotationIndex].yCoord[i] + y;			
				break;
			case TETRO_O:
				return true;
				break;
			case TETRO_S:
				xDest[i] = rotation_S[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_S[rotationIndex].yCoord[i] + y;			
				break;
			case TETRO_T:
				xDest[i] = rotation_T[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_T[rotationIndex].yCoord[i] + y;			
				break;
			case TETRO_Z:
				xDest[i] = rotation_Z[rotationIndex].xCoord[i] + x;
				yDest[i] = rotation_Z[rotationIndex].yCoord[i] + y;			
				break;
		}

		printf("[tetro_rotate]: getting block at (%d, %d)\n", xDest[i], yDest[i]);
		if ( g_isLocOutOfBounds(xDest[i], yDest[i]) )
			doRotate = false;
		else
			b = g_getBlockAtPos(xDest[i], yDest[i]);
		
		if (b != NULL && b != t->children[0] && b != t->children[1] && b != t->children[2] && b != t->children[3] )
			doRotate = false;
	}

	if (doRotate)
	{
		//S, T, and Z need blocks moved in the correct order to rotate properly
		switch (tetro_getType(t))
		{
			case TETRO_S: //2 -> 3 -> 1
				block_teleport(t->children[2], xDest[2], yDest[2]);
				block_teleport(t->children[3], xDest[3], yDest[3]);
				block_teleport(t->children[1], xDest[1], yDest[1]);
				break;
			case TETRO_T: //0 -> 3 -> 2
				block_teleport(t->children[0], xDest[0], yDest[0]);
				block_teleport(t->children[3], xDest[3], yDest[3]);
				block_teleport(t->children[2], xDest[2], yDest[2]);
				break;
				
			case TETRO_Z: //0 -> 2 -> 3
				block_teleport(t->children[0], xDest[0], yDest[0]);
				block_teleport(t->children[2], xDest[2], yDest[2]);
				block_teleport(t->children[3], xDest[3], yDest[3]);
				break;
			default:
				for ( i = 0; i < TETRO_SIZE; i++ )
				{
					b = t->children[i];
					block_teleport(b, xDest[i], yDest[i]);
				}
				break;
		}
		
		t->rotation = rotationIndex;
			
		printf("[tetro_rotate]: new rotation = %d\n", t->rotation);
	}
	return doRotate;
}


boolean tetro_isSleep(tetromino * t)
{
	return t->sleep;
}

boolean tetro_doSleep(tetromino * t)
{
	int i;
	
	if ( tetro_isSleep(t) )
		return false;
		
	t->sleep = true;
	
	for ( i = 0; i < TETRO_SIZE; i++ )
		block_doSleep(t->children[i]);
		
	return true;
}

boolean tetro_doWake(tetromino * t)
{
	int i;
	
	if ( !tetro_isSleep(t) )
		return false;

	t->sleep = false;
	
	for ( i = 0; i < TETRO_SIZE; i++ )
		block_doWake(t->children[i]);
	return true;
}

boolean tetro_setType(tetromino * t, tetroShape_t type)
{
	switch (type)
	{
		case TETRO_I:
			t->pivot = PIVOT_I;
			break;
		case TETRO_J:
			t->pivot = PIVOT_J;
			break;
		case TETRO_L:
			t->pivot = PIVOT_L;
			break;
		case TETRO_O:
			t->pivot = PIVOT_O;	
			break;
		case TETRO_S:
			t->pivot = PIVOT_S;
			break;
		case TETRO_T:
			t->pivot = PIVOT_T;
			break;
		case TETRO_Z:
			t->pivot = PIVOT_Z;
			break;
		default:
			return false;
			break;
	}
	
	t->type = type;
	return true;
}

tetroShape_t tetro_getType(tetromino * t)
{
	return t->type;
}

//redone to account for rotated tetrominos
void tetro_getRows(tetromino * t, int * upper, int * lower)
{
	int x, y;
	boolean first = true;
	block * b;
	
	//starting from bottom going up
	for ( y = 0; y < SIZE_Y; y++ )
	{
		for ( x = 0; x < SIZE_X; x++ )
		{
			b = g_getBlockAtPos(x, y);
			
			if (b != NULL)
			{
				if ( b == t->children[0] || b == t->children[1] || b == t->children[2] || b == t->children[3] )
				{
					if (first)
					{
						*lower = block_getLocY(b);
						first = false;
					}
					else
						*upper = block_getLocY(b);
				}
			}
		}
	}
}


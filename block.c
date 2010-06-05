#include "block.h"
#include "types.h"
#include "tetromino.h"
#include "stdlib.h"

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
	
	return b;
}


boolean block_clear(block * b)
{
	if ( block_getParent(b) != NULL ) //block still has a parent, do not proceed
	{
		return false;
	}
	
	free(b);
	return true;
}

boolean block_teleport(block * b, int x, int y)
{
//	block * b2;
//	b2 = game_getBlockAtPos(x, y);
/*	
THIS CODE IS ALREADY HANDLED BY game_setBlockToPos
	if ( b2 != NULL ) //another block is already here
	{
		return false;
	}
	else if ( x < X_MIN || x > X_MAX || y < Y_MIN || y > Y_MAX ) //destination is out of bounds
	{
		return false;
	}
*/

	return g_setBlockToPos(b, x, y);
}


boolean block_move(block * b, dir_t dir)
{
	int x = block_getLocX(b);
	int y = block_getLocY(b);
	
	if ( dir == dir_left )
	{
		x -= 1;
	}
	else if  ( dir == dir_right )
	{
		x += 1;
	}
	else if ( dir == dir_up )
	{
		y += 1;
	}
	else if ( dir == dir_down )
	{
		y -= 1;
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
	//bad data,
	if ( type != tetro_I && type != tetro_J && type != tetro_L && type != tetro_O && type != tetro_S && type != tetro_T && type != tetro_Z )
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

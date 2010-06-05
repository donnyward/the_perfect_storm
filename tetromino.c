//#include "block.h"
#include "tetromino.h"
#include "types.h"
#include "stdlib.h" //for malloc and NULL

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

//===============================================
//Tetromino functions below
//===============================================
tetromino * tetro_create(tetroShape_t type)
{
	int i;
	tetromino  * t = (tetromino *)(malloc(sizeof(tetromino)));
	block * b;
	
	if ( !tetro_setType(t, type) )
		return NULL;
		
	for ( i = 0; i < TETRO_SIZE; i++ ) //create each child block and assign it to the tetromino
	{
		b = block_create(type, t);
		if ( b == NULL )
			return NULL;
			
		t->children[i] = b;
	}
	
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
			return false;
			
		block_setParent(b, NULL); //unlink from parent	
	}
	
	free(t); //remove tetromino from memory
	
	//check to see if any lines have been made, if so then clear them and give points
	
	//call next tetromino to the start position and begin its descent
	return true;
}

boolean tetro_move(tetromino * t, dir_t dir)
{
	int i;
	int x, y;
	block * b;
	
	//make it so either all of the blocks will move in the direction, or none of them will
	for ( i = 0; i < TETRO_SIZE; i++ )
	{
		b = t->children[i];
//		x = start_coords[type][i][x][y];
		//block_teleport(b, 
		//x = block_getLocX(b);
		//y = block_getLocY(b);
	}
}

boolean tetro_moveStart(tetromino * t)
{
	int i;
	int x, y, type;
	block * b;
	
	type = tetro_getType(t);
	
	for ( i = 0; i < TETRO_SIZE; i++ )
	{
		b = t->children[i];
		//try writing the data to a binary file?
//		x = start_coords[type][i].x;
//		y = start_coords[type][i].y;
		
		if ( !block_teleport(b, x, y) )
			return false;
	}
	
	return true;
}

//rotates the tetromino 90 degrees clockwise. returns false if rotated position is blocked.
boolean tetro_rotate(tetromino * t)
{
	return true;
}


boolean tetro_isSleep(tetromino * t)
{
	return t->sleep;
}

boolean tetro_doSleep(tetromino * t)
{
	if ( tetro_isSleep(t) )
		return false;
		
	t->sleep = true;
	return true;
}

boolean tetro_doWake(tetromino * t)
{
	if ( !tetro_isSleep(t) )
		return false;

	t->sleep = false;
	return true;
}

boolean tetro_setType(tetromino * t, tetroShape_t type)
{
	//bad data,
	if ( type != tetro_I && type != tetro_J && type != tetro_L && type != tetro_O && type != tetro_S && type != tetro_T && type != tetro_Z )
		return false;
	else
	{
		t->type = type;
		return true;
	}
}

tetroShape_t tetro_getType(tetromino * t)
{
	return t->type;
}

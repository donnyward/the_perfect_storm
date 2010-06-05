#include "block.h"
#include "tetromino.h"
#include "types.h"
#include "stdlib.h" //for malloc and NULL


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

void tetro_clear(tetromino * t)
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

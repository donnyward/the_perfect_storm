#ifndef __TYPES_H__
#define __TYPES_H__

//10x18 playing field
#define SIZE_X 10
#define SIZE_Y 18
#define X_MIN 0
#define X_MAX 9
#define Y_MIN 0
#define Y_MAX 17

//the x and y coordinates of a block not yet in play yet (but created and awaiting entry)
#define X_VOID 99
#define Y_VOID 99

#define TETRO_SIZE 4 //how many blocks make up a tetromino
#define TETRO_SHAPES 7

#define DEBUG_MODE 1 //enables debug messages printed to terminal

//boolean type, accepts true or false
typedef enum
{
	true = 1,
	false = 0,
} boolean;

//the different shapes that tetrominos can be
typedef enum
{
	tetro_I,
	tetro_J,
	tetro_L,
	tetro_O,
	tetro_S,
	tetro_T,
	tetro_Z,
} tetroShape_t;

//directions
typedef enum
{
	dir_left,
	dir_right,
	dir_up,
	dir_down,
} dir_t;

//state of the game, playing, paused, menus...
typedef enum
{
	state_playing,
	state_paused,
	state_menu,
} gamestate_t;

//the start position for each child block of the different shaped tetrominos
//blocks 1-4 go from left to right, top to bottom (like reading)
typedef struct
{
	short x;
	short y;
} pt;
/*
pt start_coords[TETRO_SHAPES][TETRO_SIZE];

start_coords[tetro_I][0].x = 3;
start_coords[tetro_I][0].y = 16;
start_coords[tetro_I][1].x = 4;
start_coords[tetro_I][1].y = 16;
start_coords[tetro_I][2].x = 5;
start_coords[tetro_I][2].y = 16;
start_coords[tetro_I][3].x = 6;
start_coords[tetro_I][3].y = 16;

start_coords[tetro_J][0].x = 3;
start_coords[tetro_J][0].y = 16;
start_coords[tetro_J][1].x = 4;
start_coords[tetro_J][1].y = 16;
start_coords[tetro_J][2].x = 5;
start_coords[tetro_J][2].y = 16;
start_coords[tetro_J][3].x = 5;
start_coords[tetro_J][3].y = 15;

start_coords[tetro_L][0].x = 3;
start_coords[tetro_L][0].y = 16;
start_coords[tetro_L][1].x = 4;
start_coords[tetro_L][1].y = 16;
start_coords[tetro_L][2].x = 5;
start_coords[tetro_L][2].y = 16;
start_coords[tetro_L][3].x = 3;
start_coords[tetro_L][3].y = 15;

start_coords[tetro_O][0].x = 4;
start_coords[tetro_O][0].y = 16;
start_coords[tetro_O][1].x = 5;
start_coords[tetro_O][1].y = 16;
start_coords[tetro_O][2].x = 4;
start_coords[tetro_O][2].y = 15;
start_coords[tetro_O][3].x = 3;
start_coords[tetro_O][3].y = 15;

start_coords[tetro_S][0].x = 4;
start_coords[tetro_S][0].y = 16;
start_coords[tetro_S][1].x = 5;
start_coords[tetro_S][1].y = 16;
start_coords[tetro_S][2].x = 3;
start_coords[tetro_S][2].y = 15;
start_coords[tetro_S][3].x = 4;
start_coords[tetro_S][3].y = 15;

start_coords[tetro_T][0].x = 3;
start_coords[tetro_T][0].y = 16;
start_coords[tetro_T][1].x = 4;
start_coords[tetro_T][1].y = 16;
start_coords[tetro_T][2].x = 5;
start_coords[tetro_T][2].y = 16;
start_coords[tetro_T][3].x = 4;
start_coords[tetro_T][3].y = 15;

start_coords[tetro_Z][0].x = 3;
start_coords[tetro_Z][0].y = 16;
start_coords[tetro_Z][1].x = 4;
start_coords[tetro_Z][1].y = 16;
start_coords[tetro_Z][2].x = 4;
start_coords[tetro_Z][2].y = 15;
start_coords[tetro_Z][3].x = 5;
start_coords[tetro_Z][3].y = 15;
*/

#endif

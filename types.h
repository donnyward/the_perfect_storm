#ifndef __TYPES_H__
#define __TYPES_H__

#define DEBUG_MODE 0 //enables debug messages printed to terminal

//===============================================
//Game dimensions/constants
//===============================================
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
#define TETRO_SHAPES 7 //number of different shapes

#define BLOCK_WIDTH 25
#define BLOCK_HEIGHT 25

#define PLAYING_FIELD_X_MIN 195
#define PLAYING_FIELD_Y_MIN 30
#define PLAYING_FIELD_X_MAX 445
#define PLAYING_FIELD_Y_MAX 480
//===============================================
//Video settings
//===============================================
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIT_DEPTH 32

//boolean type, accepts true or false
typedef enum
{
	true = 1,
	false = 0,
} boolean;

//the different shapes that tetrominos can be
typedef enum
{
	TETRO_I,
	TETRO_J,
	TETRO_L,
	TETRO_O,
	TETRO_S,
	TETRO_T,
	TETRO_Z,
} tetroShape_t;

//directions
typedef enum
{
	DIR_NORTH,
	DIR_NORTHEAST,
	DIR_EAST,
	DIR_SOUTHEAST,
	DIR_SOUTH,
	DIR_SOUTHWEST,
	DIR_WEST,
	DIR_NORTHWEST,
	DIR_NONE,
} dir_t;


//state of the game, playing, paused, menus...
typedef enum
{
	STATE_PLAYING,
	STATE_PAUSED, //same as menu
	STATE_MENU,
} gamestate_t;

typedef enum
{
	REASON_QUIT, //player quit
	REASON_LOSS, //player lost
} gameOverReason_t;

//the start position for each child block of the different shaped tetrominos
//blocks 1-4 go from left to right, top to bottom (like reading)
typedef struct
{
	short x;
	short y;
} pt;
/*
pt start_coords[TETRO_SHAPES][TETRO_SIZE];

start_coords[TETRO_I][0].x = 3;
start_coords[TETRO_I][0].y = 16;
start_coords[TETRO_I][1].x = 4;
start_coords[TETRO_I][1].y = 16;
start_coords[TETRO_I][2].x = 5;
start_coords[TETRO_I][2].y = 16;
start_coords[TETRO_I][3].x = 6;
start_coords[TETRO_I][3].y = 16;

start_coords[TETRO_J][0].x = 3;
start_coords[TETRO_J][0].y = 16;
start_coords[TETRO_J][1].x = 4;
start_coords[TETRO_J][1].y = 16;
start_coords[TETRO_J][2].x = 5;
start_coords[TETRO_J][2].y = 16;
start_coords[TETRO_J][3].x = 5;
start_coords[TETRO_J][3].y = 15;

start_coords[TETRO_L][0].x = 3;
start_coords[TETRO_L][0].y = 16;
start_coords[TETRO_L][1].x = 4;
start_coords[TETRO_L][1].y = 16;
start_coords[TETRO_L][2].x = 5;
start_coords[TETRO_L][2].y = 16;
start_coords[TETRO_L][3].x = 3;
start_coords[TETRO_L][3].y = 15;

start_coords[TETRO_O][0].x = 4;
start_coords[TETRO_O][0].y = 16;
start_coords[TETRO_O][1].x = 5;
start_coords[TETRO_O][1].y = 16;
start_coords[TETRO_O][2].x = 4;
start_coords[TETRO_O][2].y = 15;
start_coords[TETRO_O][3].x = 3;
start_coords[TETRO_O][3].y = 15;

start_coords[TETRO_S][0].x = 4;
start_coords[TETRO_S][0].y = 16;
start_coords[TETRO_S][1].x = 5;
start_coords[TETRO_S][1].y = 16;
start_coords[TETRO_S][2].x = 3;
start_coords[TETRO_S][2].y = 15;
start_coords[TETRO_S][3].x = 4;
start_coords[TETRO_S][3].y = 15;

start_coords[TETRO_T][0].x = 3;
start_coords[TETRO_T][0].y = 16;
start_coords[TETRO_T][1].x = 4;
start_coords[TETRO_T][1].y = 16;
start_coords[TETRO_T][2].x = 5;
start_coords[TETRO_T][2].y = 16;
start_coords[TETRO_T][3].x = 4;
start_coords[TETRO_T][3].y = 15;

start_coords[TETRO_Z][0].x = 3;
start_coords[TETRO_Z][0].y = 16;
start_coords[TETRO_Z][1].x = 4;
start_coords[TETRO_Z][1].y = 16;
start_coords[TETRO_Z][2].x = 4;
start_coords[TETRO_Z][2].y = 15;
start_coords[TETRO_Z][3].x = 5;
start_coords[TETRO_Z][3].y = 15;
*/

#endif

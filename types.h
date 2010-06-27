#ifndef __TYPES_H__
#define __TYPES_H__

#define DEBUG_MODE 0 //enables extra debugging functionality (like moving blocks up lol lol)

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

//which block (0-3) is the pivot block
#define PIVOT_I 1
#define PIVOT_J 1
#define PIVOT_L 1
#define PIVOT_O 0
#define PIVOT_S 0
#define PIVOT_T 1
#define PIVOT_Z 1

#define NIL 0

#define MAX_LEVELS 20
#define MAX_SCORE 999999

#define SOFT_DROP_FRAMES 3 //how many frames for a block to soft drop by 1 cell
#define DAS_DELAY 11 //number of frames to wait for delayed auto shifting
#define DAS_PERIOD 5 //how often to move one notch while DAS'ing

//===============================================
//Video settings
//===============================================
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIT_DEPTH 32

#define TARGET_FRAME_RATE 60
#define ONE_SECOND 1000 //milliseconds

#define COLORKEY_R 78
#define COLORKEY_G 0
#define COLORKEY_B 83

#define FLASH_FRAMES 8 //even number = last flash frame before disappear is "normal" unflashed block
#define FLASH_INTERVAL 150

#define NUMBER_LENGTH 20 //pixels

#define SCORE_IMAGE_Y 108
#define LINES_IMAGE_Y 392
#define LEVEL_IMAGE_Y 250

//the rightmost digit. the next digit is this number - NUMBER_LENGTH
#define LEVEL_FIRST_DIGIT_X 100
#define LINES_FIRST_DIGIT_X 100
#define SCORE_FIRST_DIGIT_X 120

//===============================================
//Scoring
//===============================================
#define SCORE_MODIFIER_ONE_LINES 40
#define SCORE_MODIFIER_TWO_LINES 100
#define SCORE_MODIFIER_THREE_LINES 300
#define SCORE_MODIFIER_FOUR_LINES 1200

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
	TETRO_RANDOM, //altered in tetro_create
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
	//STATE_PAUSED, //same as menu
	STATE_MENU,
	STATE_IDLE, //do nothing
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
	short xCoord[TETRO_SIZE];
	short yCoord[TETRO_SIZE];
} coord_t;

#endif

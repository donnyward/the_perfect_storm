#ifndef __TYPES_H__
#define __TYPES_H__

//see the global int debug_mode
//#define DEBUG_MODE 0 //enables extra debugging functionality (like moving blocks up lol lol)

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
#define SIZE_ONE_DIMENSIONAL 180

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

#define MAX_LEVEL 20
#define MAX_SCORE 999999

#define SOFT_DROP_FRAMES 3 //how many frames for a block to soft drop by 1 cell
#define DAS_DELAY 11 //number of frames to wait for delayed auto shifting
#define DAS_PERIOD 5 //how often to move one notch while DAS'ing

#define ASCII_A 1 //the numerical value of ascii char A. subtracted from char's to get proper array loc for chars[]

#define LOSS_PAUSE 1500 //how long to pause when theres no room for the next block to enter the field
#define GAME_OVER_PAUSE 3000 //how long to say game over

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

#define FLASH_FRAMES 8 //even number = last flash frame before disappear is BLOCK_FLASH1
#define FLASH_INTERVAL 150

#define NUMBER_LENGTH 20 //pixels

#define SCORE_IMAGE_Y 108
#define LINES_IMAGE_Y 392
#define LEVEL_IMAGE_Y 250

//the rightmost digit. the next digit is this number - NUMBER_LENGTH
#define LEVEL_FIRST_DIGIT_X 100
#define LINES_FIRST_DIGIT_X 100
#define SCORE_FIRST_DIGIT_X 120

//for character bitmap font
#define CHAR_SIZE 28 //A-Z + underscore + space
#define CHAR_WIDTH 20
#define CHAR_HEIGHT 20

//loc for 1st high scores entry
#define HIGH_SCORES_FIRST_LETTER_LOC_X 40
#define HIGH_SCORES_FIRST_LETTER_LOC_Y 80
#define HIGH_SCORES_FIRST_NUMBERS_LOC_X 580
#define HIGH_SCORES_FIRST_NUMBERS_LOC_Y 80
#define HIGH_SCORES_GAP 20 //how much space in pixels between each high scores entry (vertical)

//where on the screen to show your initials whilst entering your initials
#define NEW_HIGH_SCORE_X 290
#define NEW_HIGH_SCORE_Y 220

//locations for some pre baked text
#define TEXT_GETREADY_X 195
#define TEXT_GETREADY_Y 30
#define TEXT_GOGOGO_X 195
#define TEXT_GOGOGO_Y 130
#define TEXT_GAMEOVER_X 195
#define TEXT_GAMEOVER_Y 100

//===============================================
//Audio
//===============================================

#define FREQUENCY 44100
#define SAMPLE_SIZE 4096

//===============================================
//Scoring
//===============================================
#define SCORE_MODIFIER_ONE_LINES 40
#define SCORE_MODIFIER_TWO_LINES 100
#define SCORE_MODIFIER_THREE_LINES 300
#define SCORE_MODIFIER_FOUR_LINES 1200

//high scores list
#define HIGH_SCORES_LIST_SIZE 10

#define HIGH_SCORES_0 500000
#define HIGH_SCORES_1 400000
#define HIGH_SCORES_2 350000
#define HIGH_SCORES_3 325000
#define HIGH_SCORES_4 300000
#define HIGH_SCORES_5 200000
#define HIGH_SCORES_6 150000
#define HIGH_SCORES_7 100000
#define HIGH_SCORES_8 50000
#define HIGH_SCORES_9 10000
/*
//lower initial scores are for testing purposes
#define HIGH_SCORES_0 1000
#define HIGH_SCORES_1 900
#define HIGH_SCORES_2 800
#define HIGH_SCORES_3 700
#define HIGH_SCORES_4 600
#define HIGH_SCORES_5 500
#define HIGH_SCORES_6 400
#define HIGH_SCORES_7 300
#define HIGH_SCORES_8 90
#define HIGH_SCORES_9 5
*/
#define INITIALS_SIZE 4 //last is null termin8er

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
	TETRO_DEAD, //for blocks that fill up on loss state
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
	STATE_LOSS, //when the game is lost. start filling up the field with blocks
	STATE_IDLE, //do nothing
} gamestate_t;

//loss = check for new high score, say game over, fill up screen with blocks
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

//single struct that stores the contents of the high scores
typedef struct
{
	int scores[HIGH_SCORES_LIST_SIZE];
	//char * names[HIGH_SCORES_LIST_SIZE];
	char name0[INITIALS_SIZE];
	char name1[INITIALS_SIZE];
	char name2[INITIALS_SIZE];
	char name3[INITIALS_SIZE];
	char name4[INITIALS_SIZE];
	char name5[INITIALS_SIZE];
	char name6[INITIALS_SIZE];
	char name7[INITIALS_SIZE];
	char name8[INITIALS_SIZE];
	char name9[INITIALS_SIZE];
} highScoresStruct_t;

//enum for use with spriteLoc_t's
typedef enum
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	UNDERSCORE,
	SPACE,
} characters_t;

//for char sprite sheet. array with each cell representing a letter
typedef struct
{
	int x;
	int y;
	int width;
	int height;
} spriteLoc_t;

typedef enum
{
	SND_MENUMOVE,
	SND_MENUSELECT,
	SND_ROTATE,
	SND_MOVESIDEWAYS,
	SND_DOWNBLOCKED,
	SND_LINECLEAR,
	SND_GAMEOVER,
	SND_NONE,
} sounds_e;

#endif

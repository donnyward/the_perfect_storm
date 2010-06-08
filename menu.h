#ifndef __MENU_H__
#define __MENU_H__

#include "types.h"

typedef enum
{
	REASON_IDLE, //player was idle (no input) for a period of time
	REASON_CHOSEN, //player explicitly chose to view high scores
} highScoresReason_t;

typedef enum
{
	M_MAIN, //has newgame, highscores, and exit
	M_HIGHSCORES, //has just high scores, escape will go back to main menu
	M_PAUSE, //has continue, and quit
	M_PAUSECONFIRMQUIT, //if quit is selected in pause menu, this will pop up asking yes or no (to confirm)
	M_EXIT, //if exit is selected in main menu, this pops up asking yes or no (to confirm)
} menuLoc_t;

typedef enum
{
	//M_MAIN menu choices
	S_NEWGAME,
	S_HIGHSCORES,
	S_EXIT, //exit entire program
	
	//M_HIGHSCORES menu choices
	//none applicable
	
	//M_PAUSE menu choices
	S_CONTINUE,
	S_QUIT,
	
	//M_PAUSECONFIRMQUIT and M_EXIT menu choices
	S_YES,
	S_NO,
} menuSelection_t;
	

typedef struct
{
	dir_t nextMoveX; //next move to make in the x direction
	dir_t nextMoveY; //next move to make in the y direction
	dir_t nextMoveDir; //next move to make in x and y (includes diagonals)
	dir_t forwardOrBack; //go forward thru the menu (NORTH/enter) backward (SOUTH/escape) or neither (DIR_NONE)
	menuLoc_t menuLoc; //which menu panel we are currently on
	menuSelection_t currentSelection;
} menu_t;

//handles the updating of the menu
//moves the menu selection to the direction specified by menu.nextMove
//can also specify a specific menu option to highlight/select, (for use with mouse pointer highlighting)
boolean m_move();

//called when enter is pushed and new game is selected. Sets up a new game.
int m_newGame();

//stops updating the game and brings up the pause menu.
boolean m_pause();

//called when no input is entered for a long time, or entered in the menu.
//displays high scores until input is received (if called with enter),
//or only for a period of time if it was called due to inactivity.
boolean m_highScores(highScoresReason_t r);

//erases the menu, or stop all menu related functions from working now since game is playing now
boolean m_clear();

//creates the menu as it should look for the frame, based on input and variables and whatnot
boolean m_create();

//set up menu initial menu values for the very 1st run thru
void m_init();

#endif

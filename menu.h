#ifndef __MENU_H__
#define __MENU_H__

#include "types.h"
#include "game.h"

typedef enum
{
	REASON_IDLE, //player was idle (no input) for a period of time
	REASON_CHOSEN, //player explicitly chose to view high scores
} highScoresReason_t;

typedef enum
{
	M_MAIN, //has newgame, highscores, and exit
	M_HIGHSCORES, //has just high scores, escape will go back to main menu
	M_NEWHIGH, //use arrow keys to enter initials. enter and escape do nothing
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
	
	S_IDLE, //do nothing
} menuSelection_t;
	

typedef struct
{
	//next move to make through menu.
	//DIR_NORTH = up 1
	//DIR_SOUTH = down 1
	//DIR_EAST = make selection (enter)
	//DIR_WEST = go back (escape)
	dir_t nextMoveDir;

	menuLoc_t menuLoc; //which menu panel we are currently on
	menuSelection_t currentSelection;
} menu_t;

//handles the updating of the menu
//moves the menu selection to the direction specified by menu.nextMove
//can also specify a specific menu option to highlight/select, (for use with mouse pointer highlighting)
//returns the enum of the sound that should be played for the movement (including no sound)
sounds_e m_move();

//called when no input is entered for a long time, or entered in the menu.
//displays high scores until input is received (if called with enter),
//or only for a period of time if it was called due to inactivity.
boolean m_highScores(highScoresReason_t r);

//set up menu initial menu values for the very 1st run thru
void m_init();

#endif

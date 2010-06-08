#include "menu.h"

menu_t menu;


boolean m_move()
{
	return true;
}

int m_newGame()
{
}

boolean m_pause()
{

}

boolean m_highScores(highScoresReason_t r)
{
}

boolean m_clear()
{
}

boolean m_create()
{

	
}

void m_init()
{
/*
typedef struct
{
	dir_t nextMove;
	menuLoc_t menuLoc; //which menu panel we are currently on
	menuSelection_t currentSelection;
} menu_t;
*/
	menu.nextMoveX = DIR_NONE;
	menu.nextMoveY = DIR_NONE;
	menu.nextMoveDir = DIR_NONE;
	menu.forwardOrBack = DIR_NONE;
	menu.menuLoc = M_MAIN;
	menu.currentSelection = S_NEWGAME;
}

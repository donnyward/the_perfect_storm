#include "menu.h"
#include <stdio.h>

menu_t menu;
extern gameModule game;

boolean m_move()
{
	if (menu.nextMoveDir == DIR_NONE)
		return false;
		
	if (menu.nextMoveDir == DIR_NORTH) //go up
	{
		if (menu.menuLoc == M_MAIN)
		{
			if (menu.currentSelection == S_NEWGAME)
				menu.currentSelection = S_EXIT;
			else
				menu.currentSelection--;
		}
		else if (menu.menuLoc == M_HIGHSCORES)
		{
			//do nothing
		}
		else if (menu.menuLoc == M_PAUSE)
		{
			if (menu.currentSelection == S_CONTINUE)
				menu.currentSelection = S_QUIT;
			else
				menu.currentSelection = S_CONTINUE;
		}
		else if (menu.menuLoc == M_PAUSECONFIRMQUIT || menu.menuLoc == M_EXIT )
		{
			if (menu.currentSelection == S_YES)
				menu.currentSelection = S_NO;
			else
				menu.currentSelection = S_YES;
		}
		else
		{
			printf("[m_move]: Up pushed, unhandled menu.menuLoc '%d'!\n", menu.menuLoc);
			return false;
		}
	}
	else if (menu.nextMoveDir == DIR_SOUTH) //go down
	{
		if (menu.menuLoc == M_MAIN)
		{
			if (menu.currentSelection == S_EXIT)
				menu.currentSelection = S_NEWGAME;
			else
				menu.currentSelection++;
		}
		else if (menu.menuLoc == M_HIGHSCORES)
		{
			//do nothing
		}
		else if (menu.menuLoc == M_PAUSE)
		{
			if (menu.currentSelection == S_CONTINUE)
				menu.currentSelection = S_QUIT;
			else
				menu.currentSelection = S_CONTINUE;
		}
		else if (menu.menuLoc == M_PAUSECONFIRMQUIT || menu.menuLoc == M_EXIT )
		{
			if (menu.currentSelection == S_YES)
				menu.currentSelection = S_NO;
			else
				menu.currentSelection = S_YES;
		}
		else
		{
			printf("[m_move]: Down pushed, unhandled menu.menuLoc '%d'!\n", menu.menuLoc);
			return false;
		}
	}
	else if (menu.nextMoveDir == DIR_EAST) //go in
	{
		if (menu.currentSelection == S_NEWGAME)
		{
			//start new game
			g_create();
		}
		else if (menu.currentSelection == S_HIGHSCORES)
		{
			//open up high scores screen
			m_highScores(REASON_CHOSEN);
		}
		else if (menu.currentSelection == S_EXIT)
		{
			menu.menuLoc = M_EXIT;
			menu.currentSelection = S_NO;
		}
		else if (menu.currentSelection == S_CONTINUE)
		{
			//resume game
			game.state = STATE_PLAYING;
		}
		else if (menu.currentSelection == S_QUIT)
		{
			menu.menuLoc = M_PAUSECONFIRMQUIT;
			menu.currentSelection = S_NO;
		}
		else if (menu.currentSelection == S_YES)
		{
			if (menu.menuLoc == M_PAUSECONFIRMQUIT)
			{
				//quit game go back to main menu
				g_clear(REASON_QUIT);
			}
			else if (menu.menuLoc == M_EXIT)
			{
				//exit game
				game.exitGameYet = true;
			}
		}
		else if (menu.currentSelection == S_NO)
		{
			if (menu.menuLoc == M_PAUSECONFIRMQUIT)
			{
				//back to pause menu
				menu.menuLoc = M_PAUSE;
				menu.currentSelection = S_QUIT;
			}
			else if (menu.menuLoc == M_EXIT)
			{
				//go back to main menu
				menu.menuLoc = M_MAIN;
				menu.currentSelection = S_EXIT;
			}
		}
		else
		{
			printf("[m_move]: Enter pushed, unhandled menu.currentSelection '%d'!\n", menu.currentSelection);
			return false;
		}
	}
	else if (menu.nextMoveDir == DIR_WEST) //go out
	{
		if (menu.menuLoc == M_MAIN)
		{
			//ask if want to exit
			menu.menuLoc = M_EXIT;
			menu.currentSelection = S_NO;
		}
		else if (menu.menuLoc == M_HIGHSCORES)
		{
			//back to main menu
			menu.menuLoc = M_MAIN;
			menu.currentSelection = S_HIGHSCORES;
		}
		else if (menu.menuLoc == M_PAUSE)
		{
			//resume game
		}
		else if (menu.menuLoc == M_PAUSECONFIRMQUIT)
		{
			//go back to pause menu
			menu.menuLoc = M_PAUSE;
			menu.currentSelection = S_QUIT;
		}
		else if (menu.menuLoc == M_EXIT)
		{
			//go back to main menu
			menu.menuLoc = M_MAIN;
			menu.currentSelection = S_EXIT;
		}
		else
		{
			printf("[m_move]: ESC pushed, unhandled menu.menuLoc '%d'!\n", menu.menuLoc);
			return false;
		}
	}
	return true;
}

boolean m_highScores(highScoresReason_t r)
{
	menu.menuLoc = M_HIGHSCORES;
	return true;
}

void m_init()
{
	menu.nextMoveDir = DIR_NONE;
	menu.menuLoc = M_MAIN;
	menu.currentSelection = S_NEWGAME;
}

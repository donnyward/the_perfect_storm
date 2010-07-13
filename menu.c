#include "menu.h"
#include <stdio.h>

menu_t menu;
extern gameModule game;
extern highScoresStruct_t highScores;
extern char * highScoresNameArray[];

boolean m_move()
{
	int i;
	FILE * f; //for amending high scores
	
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
		else if (menu.menuLoc == M_NEWHIGH)
		{
			//go backwards a letter
			if (menu.menuLoc == M_NEWHIGH)
			{
				i = 0;
				while (game.newHighScore[i] != 0)
					i++;
				i--; //i is sitting on the last char that was entered
			
				if (game.newHighScore[i] == 'A')
					game.newHighScore[i] = ' ';
				else if (game.newHighScore[i] == ' ')
					game.newHighScore[i] = '_';
				else if (game.newHighScore[i] == '_')
					game.newHighScore[i] = 'Z';
				else
					game.newHighScore[i]--;
			}
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
		else if (menu.menuLoc == M_NEWHIGH)
		{
			//go forward a letter
			if (menu.menuLoc == M_NEWHIGH)
			{
				i = 0;
				while (game.newHighScore[i] != 0)
					i++;
				i--; //i is sitting on the last char that was entered
			
				if (game.newHighScore[i] == 'Z')
					game.newHighScore[i] = ' ';
				else if (game.newHighScore[i] == ' ')
					game.newHighScore[i] = '_';
				else if (game.newHighScore[i] == '_')
					game.newHighScore[i] = 'A';
				else
					game.newHighScore[i]++;
			}
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
		else if (menu.menuLoc == M_NEWHIGH)
		{
			//do nothing
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
	else if (menu.nextMoveDir == DIR_NORTHWEST) //go left (hack for entering initials in event of new high score)
	{
		if (menu.menuLoc == M_NEWHIGH)
		{
			i = 0;
			while (game.newHighScore[i] != 0)
				i++;
			i--; //i is sitting on the last char that was entered
			
			game.newHighScore[i] = 0;
		}
	}
	else if (menu.nextMoveDir == DIR_NORTHEAST) //go right
	{
		if (menu.menuLoc == M_NEWHIGH)
		{
			i = 0;
			while (game.newHighScore[i] != 0)
				i++;
			//i--;
			
			if (i != 3)
				game.newHighScore[i] = '_';
			else //done entering initials
			{
				strcpy(highScoresNameArray[game.highScoreIndexToReplace], game.newHighScore);
				highScores.scores[game.highScoreIndexToReplace] = game.score;
				
				f = fopen("hs.dat", "w+b");
				fwrite(&highScores, sizeof(highScoresStruct_t), 1, f);
				fclose(f);
				
				m_highScores(REASON_CHOSEN);
			}
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

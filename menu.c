#include "menu.h"
#include <stdio.h>
#include "SDL/SDL_mixer.h"

menu_t menu;
extern gameModule game;
extern highScoresStruct_t highScores;
extern char * highScoresNameArray[];

extern Mix_Chunk * downBlock;
extern Mix_Chunk * gameOver;
extern Mix_Chunk * lineClear;
extern Mix_Chunk * menuMove;
extern Mix_Chunk * menuSelect;
extern Mix_Chunk * moveSideways;
extern Mix_Chunk * rotate;

extern Mix_Chunk * sound_effects[];
sounds_e m_move()
{
	int i;
	FILE * f; //for amending high scores
	sounds_e soundEffect = SND_NONE;
	
	if (menu.nextMoveDir == DIR_NONE)
		return SND_NONE;
		
	if (menu.nextMoveDir == DIR_NORTH) //go up
	{
		soundEffect = SND_MENUMOVE;
		
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
			soundEffect = SND_NONE;
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
					game.newHighScore[i] = 'Z';
				//else if (game.newHighScore[i] == '_')
				//	game.newHighScore[i] = 'Z';
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
			return SND_NONE;
		}
	}
	else if (menu.nextMoveDir == DIR_SOUTH) //go down
	{
		soundEffect = SND_MENUMOVE;
		
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
			soundEffect = SND_NONE;
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
					game.newHighScore[i] = 'A';
//				else if (game.newHighScore[i] == '_')
//					game.newHighScore[i] = 'A';
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
			return SND_NONE;
		}
	}
	else if (menu.nextMoveDir == DIR_EAST) //go in
	{
		soundEffect = SND_MENUSELECT;
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
			return SND_NONE;
		}
	}
	else if (menu.nextMoveDir == DIR_WEST) //go out
	{
		soundEffect = SND_MENUSELECT;
		
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
			soundEffect = SND_NONE;
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
			return SND_NONE;
		}
	}
	else if (menu.nextMoveDir == DIR_NORTHWEST) //go left (hack for entering initials in event of new high score)
	{
		if (menu.menuLoc == M_NEWHIGH)
		{
			soundEffect = SND_MENUMOVE;
			i = 0;
			while (game.newHighScore[i] != 0)
				i++;
			i--; //i is sitting on the last char that was entered
			
			if ( i <= 0 )
			{
				//do nothing, always have 1st letter displayed
			}
			else
				game.newHighScore[i] = 0;
		}
	}
	else if (menu.nextMoveDir == DIR_NORTHEAST) //go right
	{
		if (menu.menuLoc == M_NEWHIGH)
		{
			soundEffect = SND_MENUMOVE;
			i = 0;
			while (game.newHighScore[i] != 0)
				i++;
			
			if (i != 3)
				game.newHighScore[i] = ' ';
			else //done entering initials
			{
				for ( i = HIGH_SCORES_LIST_SIZE-1; i > game.highScoreIndexToReplace; i-- )
				{
					strcpy(highScoresNameArray[i], highScoresNameArray[i-1]);
					highScores.scores[i] = highScores.scores[i-1];
				}
				
				strcpy(highScoresNameArray[game.highScoreIndexToReplace], game.newHighScore);
				highScores.scores[game.highScoreIndexToReplace] = game.score;
				
				f = fopen("hs.dat", "w+b");
				fwrite(&highScores, sizeof(highScoresStruct_t), 1, f);
				fclose(f);
				
				m_highScores(REASON_CHOSEN);
			}
		}
	}
	/*
	if (soundEffect == SND_MENUMOVE)
		s_playSound(menuMove);
	else if (soundEffect == SND_MENUSELECT)
		s_playSound(menuSelect);
	*/
	return soundEffect;
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

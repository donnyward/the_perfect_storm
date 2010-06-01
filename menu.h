#ifndef __MENU_H__
#def __MENU_H__

enum highScoresReason_e
{
	reason_idle, //player was idle (no input) for a period of time
	reason_chosen, //player explicitly chose to view high scores
}


int menu_getInput();

int menu_up();
int menu_down();
int menu_newGame();
int menu_highScores(int highScoresReason);
int menu_clear();
int menu_create();

#endif

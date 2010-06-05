#ifndef __MENU_H__
#define __MENU_H__

#include "types.h"

typedef enum
{
	reason_idle, //player was idle (no input) for a period of time
	reason_chosen, //player explicitly chose to view high scores
} highScoresReason_t;


int m_getInput();

int m_up();
int m_down();
int m_newGame();
int m_highScores(highScoresReason_t r);
int m_clear();
int m_create();

#endif

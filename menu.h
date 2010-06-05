#ifndef __MENU_H__
#define __MENU_H__

#include "types.h"

typedef enum
{
	reason_idle, //player was idle (no input) for a period of time
	reason_chosen, //player explicitly chose to view high scores
} highScoresReason_t;


boolean m_up();
boolean m_down();
int m_newGame();
boolean m_highScores(highScoresReason_t r);
boolean m_clear();
boolean m_create();

#endif

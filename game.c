#include "game.h"
#include "types.h"

#include "SDL/SDL.h" //needed for all SDL apps
#include <stdio.h> //for prinf
#include <stdlib.h> //for exit()

#include "menu.h"

extern SDL_Surface * screen; //the screen surface. originally declared in main.c
SDL_Surface * image; //used by g_drawGame()
extern menu_t menu; //needed in this file to set its values in g_handleInput()
gameModule game; //game module, stores levle, 2d grid and what it contains, etc

dir_t clockwiseDir[] = 
{
	DIR_NORTHEAST,
	DIR_EAST,
	DIR_SOUTHEAST,
	DIR_SOUTH,
	DIR_SOUTHWEST,
	DIR_WEST,
	DIR_NORTHWEST,
	DIR_NORTH,
	DIR_NONE,
};

dir_t counterClockwiseDir[] = 
{
	DIR_NORTHWEST,
	DIR_NORTH,
	DIR_NORTHEAST,
	DIR_EAST,
	DIR_SOUTHEAST,
	DIR_SOUTH,
	DIR_SOUTHWEST,
	DIR_WEST,
	DIR_NONE,
};

//pixel locations of the top left corner of each button image
int buttonLocX[] = 
{
	160, 160, 160, 160, 160, 160, 160
};

int buttonLocY[] = 
{
	180, 280, 380, 210, 310, 210, 310
};

//coords for tetrominos in stasis (in pixel coords)
short stasisPixelCoordX[] = 
{
//	467, 492, 517, 542
	505, 530, 555, 580
};

short stasisPixelCoordY[] = 
{
//	234, 259
	213, 238
};

//these coords are the 4x2 grid that the the "next" box contains
startCoord_t stasisCoord[] = 
{
	//I SHAPE
	{ {0, 1, 2, 3}, {0, 0, 0, 0} },
	
	//J SHAPE
	{ {0, 1, 2, 2}, {0, 0, 0, 1} },
	
	//L SHAPE
	{ {0, 1, 2, 0}, {0, 0, 0, 1} },
	
	//O SHAPE
	{ {1, 2, 1, 2}, {0, 0, 1, 1} },
	
	//S SHAPE
	{ {1, 2, 0, 1}, {0, 0, 1, 1} },
		
	//T SHAPE
	{ {0, 1, 2, 1}, {0, 0, 0, 1} },

	//Z SHAPE
	{ {0, 1, 1, 2}, {0, 0, 1, 1} }
};

int frame = 0;

int g_getScore()
{
	return game.score;
}

int g_getLevel()
{
	return game.level;
}

block * g_getBlockAtPos(int x, int y)
{
	//debug_msg("[g_getBlockAtPos]: starting...\n");
	return game.pos[x][y];
}

boolean g_setBlockToPos(block * b, int x, int y)
{
	int x_old = block_getLocX(b);
	int y_old = block_getLocY(b);
	block * b2 = g_getBlockAtPos(x, y);

	if ( b2 != NULL ) //another block is already here at destination
	{
		return false;
	}
	else if ( x < X_MIN || x > X_MAX || y < Y_MIN || y > Y_MAX ) //destination is out of bounds
	{
		return false;
	}
	else //good 2go
	{
		if ( !g_removeBlockFromPos(b) ) //its original position was invalid
			debug_msg("Block tried to remove from pos: Failed!\n");

		game.pos[x][y] = b;
		block_setLocX(b, x);
		block_setLocY(b, y);
		return true;
	}
}
/*
boolean g_setBlockToPos(block * b, int x, int y)
{
	int x_old = block_getLocX(b);
	int y_old = block_getLocY(b);
	block * b2 = g_getBlockAtPos(x, y);
	
	if ( x < X_MIN || x > X_MAX || y < Y_MIN || y > Y_MAX ) //destination is out of bounds
	{
		printf("[g_setBlockToPos]: (%d, %d) is out of bounds!\n");
		return false;
	}
	else if ( b2 != NULL ) //another block is already here at destination
	{
		//return false;
		printf("[g_setBlockToPos]: warning, a block already at (%d, %d)\n", x, y);
		
		if ( block_getParent(b2) == block_getParent(b) )
		{
			printf("[g_setBlockToPos]: the two blocks are of the same parent so it is fine\n");
			
			//return true;
		}
		else
		{
			printf("[g_setBlockToPos]: the two blocks are of different parent, can not move\n");
			return false;
		}
		
	}
	

	//if ( !g_removeBlockFromPos(b) ) //its original position was invalid
	//	debug_msg("[g_setBlockToPos]: Block tried to remove from pos: Failed!\n");
			
	game.pos[x][y] = b;
	block_setLocX(b, x);
	block_setLocY(b, y);
	return true;
}
*/
boolean g_removeBlockFromPos(block * b)
{
	int x = block_getLocX(b);
	int y = block_getLocY(b);
	
	if ( x < X_MIN || x > X_MAX || y < Y_MIN || y > Y_MAX ) //destination is out of bounds
	{
		debug_msg("[g_removeBlockFromPos]: target is out of bounds!\n");
		return false;
	}
	
	game.pos[x][y] = NULL;
	return true;
}

void g_clear(gameOverReason_t r)
{
	//clear tetrominoz
	tetro_clear(game.next);
	tetro_clear(game.current);
	
	g_clearGrid();
	
	//setup variables for menu
	game.state = STATE_MENU;
	m_init();
}

void g_end()
{
	printf("Unloading SDL...\n");
	SDL_Quit();
}

void g_create()
{
	//set initial values for a gameModule new game
	game.level = 1;
	game.score = 0;
	game.lines = 0;
	game.state = STATE_IDLE;
	game.next = NULL;
	game.current = NULL;

	printf("Get Ready...\n");
	
	game.current = tetro_create(TETRO_RANDOM);
	game.next = tetro_create(TETRO_RANDOM);
//	SDL_Delay(2000); //2 seconds
	
	printf("GO GO GO\n");
	tetro_moveStart(game.current);
	game.state = STATE_PLAYING;
}

SDL_Surface * g_loadImage(char * filename)
{
	SDL_Surface * loaded = NULL;
	SDL_Surface * image = NULL;
	Uint32 colorkey;
	
	loaded = SDL_LoadBMP(filename);
	if ( loaded != NULL )
	{
		image = SDL_DisplayFormat(loaded); //converts loaded bmp into screen settings (if not already set to them)
		SDL_FreeSurface(loaded);
		
		colorkey = SDL_MapRGB(image->format, COLORKEY_R, COLORKEY_G, COLORKEY_B);
		SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey);
	}
	else
		return NULL;
		
	return image;
}

SDL_Surface * g_loadBlockImage(tetroShape_t type)
{
	SDL_Surface * image;
	switch (type)
	{
		case TETRO_I:
			image = g_loadImage("./pictures/BLOCK_I.bmp");
			break;
		case TETRO_J:
			image = g_loadImage("./pictures/BLOCK_J.bmp");
			break;
		case TETRO_L:
			image = g_loadImage("./pictures/BLOCK_L.bmp");
			break;
		case TETRO_O:
			image = g_loadImage("./pictures/BLOCK_O.bmp");
			break;
		case TETRO_S:
			image = g_loadImage("./pictures/BLOCK_S.bmp");
			break;
		case TETRO_T:
			image = g_loadImage("./pictures/BLOCK_T.bmp");
			break;
		case TETRO_Z:
			image = g_loadImage("./pictures/BLOCK_Z.bmp");
			break;
	}
	return image;
}

boolean g_addSurface(int x, int y, SDL_Surface * source, SDL_Surface * dest)
{
	SDL_Rect offset;
	
	offset.x = x;
	offset.y = y;
	
	if ( SDL_BlitSurface(source, NULL, dest, &offset) < 0 ) //<0 = fail
	{
		SDL_FreeSurface(source);
		return false;
	}
	else
	{
		SDL_FreeSurface(source);
		return true;
	}
}

void debug_msg(char * msg)
{
	if (DEBUG_MODE)
		printf("%s", msg);
}

void g_loop()
{
	//return; //until the loop is no longer infinite
	while ( !game.exitGameYet )
	{
		//get time passed since last loop and see if enough time has passed for another update (target = 30 fps)
		//take note of the current time (for next loop around)
		
		
		//handle input, this should do nothing more then set values on objects (like tetrominos) so they know what to do next
		g_handleInput();
		
		//update the game (moving, scoring, menu seletion etc). this should look at the values set by g_handleInput() and update accordingly
		g_updateGame();
		
		//update the display with latest updated data clear the screen and redraw everything
		g_drawGame();
//		SDL_Delay(1000);
//		printf("gamestate = %d\n", game.state);
	}
	g_end(); //exit to dos
}

void g_handleInput()
{
	SDL_Event event;
	
	while ( SDL_PollEvent(&event) ) //returns 1 if there are pending events
	{
		//an event was polled and the details are stored in the SDL_Event for this loop
		switch (event.type)
		{
			case SDL_KEYDOWN:
				//printf("[g_handleInput()]: A key was pushed down!\n");
				if (game.state == STATE_MENU)
				{
					switch (event.key.keysym.sym) //figure out what key
					{
						case SDLK_DOWN:
							if (menu.nextMoveDir == DIR_NONE)
								menu.nextMoveDir = DIR_SOUTH;
							else if (menu.nextMoveDir == DIR_NORTH)
							{
								//cancel out opposite inputs
								menu.nextMoveDir = DIR_NONE;
							}
							break;
						case SDLK_UP:
							if (menu.nextMoveDir == DIR_NONE)
								menu.nextMoveDir = DIR_NORTH;
							else if (menu.nextMoveDir == DIR_SOUTH)
							{
								//cancel out opposite inputs
								menu.nextMoveDir = DIR_NONE;
							}
							break;
						case SDLK_RETURN:
							if (menu.nextMoveDir == DIR_NONE) //DIR_EAST equivalent to forward
								menu.nextMoveDir = DIR_EAST;
							else if (menu.nextMoveDir == DIR_WEST)
							{
								//cancel out opposite inputs
								menu.nextMoveDir = DIR_NONE;
							}
							break;
						case SDLK_ESCAPE://(brings up pause menu, or go back thru any menu that your in.)
							if (menu.nextMoveDir == DIR_NONE) //WEST == LEFT == go back
								menu.nextMoveDir = DIR_WEST;	
							else if (menu.nextMoveDir == DIR_EAST)
							{
								//cancel out oppostie inputs
								menu.nextMoveDir = DIR_NONE;
							}	
							break;	
						default:
							break;
					}
				}
				else if (game.state == STATE_PLAYING)
				{
					switch (event.key.keysym.sym) //figure out what key
					{
						case SDLK_DOWN:
							if (game.current->nextMoveY == DIR_NONE)
								game.current->nextMoveY = DIR_SOUTH;
							else if (game.current->nextMoveY == DIR_NORTH)
							{
								//cancel out opposite inputs
								game.current->nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_UP:
							if (!DEBUG_MODE)
								break;
							if (game.current->nextMoveY == DIR_NONE)
								game.current->nextMoveY = DIR_NORTH;
							else if (game.current->nextMoveY == DIR_SOUTH)
							{
								//cancel out opposite inputs
								game.current->nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_LEFT:
							if (game.current->nextMoveX == DIR_NONE)
								game.current->nextMoveX = DIR_WEST;
							else if (game.current->nextMoveX == DIR_EAST)
							{
								//cancel out opposite inputs
								game.current->nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_RIGHT:
							if (game.current->nextMoveX == DIR_NONE)
								game.current->nextMoveX = DIR_EAST;
							else if (game.current->nextMoveX == DIR_WEST)
							{
								//cancel out opposite inputs
								game.current->nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_ESCAPE: //(bring up pause menu, or go back thru any menu that your in.)
							game.state = STATE_MENU;
							menu.currentSelection = S_CONTINUE;
							menu.menuLoc = M_PAUSE;
						case SDLK_SPACE: //(rotate tertromino in this case)
						
						default:
							break;
					}
				}
				break;
			case SDL_KEYUP:
				//printf("[g_handleInput()]: A key was pushed up!\n");
				switch (event.key.keysym.sym) //figure out what key
				{
					case SDLK_DOWN:
					case SDLK_LEFT:
					case SDLK_RIGHT:

					default:
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN: //dont need a case for mouseUP. mouse click is only used in menus, not gameplay
				//printf("[g_handleInput()]: A mouse button was pushed down!\n");
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						//find out what menu we are on, and if we clicked on a button,
						//if we clicked a button then set a variable in the menu to be handled
						break;
				}
				break;
			case SDL_QUIT: //user clicked the x on the window
				//printf("[g_handleInput()]: X clicked!\n");
				game.exitGameYet = true;
				break;
		}
	}			
}

void g_updateGame()
{
	if (game.state == STATE_IDLE)
	{
		//do nothing
	}
	else if (game.state == STATE_MENU)
	{
		if (DEBUG_MODE)
		{
			frame++;
			if (frame == 61)
				frame = 1;
			printf("[g_updateGame]: FRAME = %d\n", frame);
		}
		
		if ( !m_move() )
		{
			//too much output
			//printf("[g_updateGame]: m_move returned false!\n");
		}
	}
	else if (game.state == STATE_PLAYING)
	{	
		//take note of the time here, see if it is time to move the current tetro
		//1 notch down
		/*
		if ( !tetro_move(game.current, DIR_SOUTH) ) //the periodic drop down was blocked
		{
			printf("[g_updateGame]: periodic drop down blocked!\n");
			g_onDownBlocked();
		}
			
		*/
			
		//create resultant move direction
		if ( game.current->nextMoveX == DIR_NONE )
			game.current->nextMoveDir = game.current->nextMoveY;
		else if ( game.current->nextMoveY == DIR_NONE )
			game.current->nextMoveDir = game.current->nextMoveX;
		else if ( game.current->nextMoveY == DIR_NORTH )
		{
			if ( game.current->nextMoveX == DIR_WEST )
				game.current->nextMoveDir = DIR_NORTHWEST;
			else
				game.current->nextMoveDir = DIR_NORTHEAST;
		}
		else //nextMoveY == south
		{
			if ( game.current->nextMoveX == DIR_WEST )
				game.current->nextMoveDir = DIR_SOUTHWEST;
			else
				game.current->nextMoveDir = DIR_SOUTHEAST;
		}
			
		//attempt to move tetro in this direction
		if ( !tetro_move(game.current, game.current->nextMoveDir) )
		{
			printf("[g_updateGame]: tetro_move failed!\n");
			if ( game.current->nextMoveDir == DIR_SOUTH  || game.current->nextMoveDir == DIR_SOUTHWEST || game.current->nextMoveDir == DIR_SOUTHEAST )
			{
				g_onDownBlocked();
			}
		}
		
		game.current->nextMoveX = DIR_NONE;
		game.current->nextMoveY = DIR_NONE;
		game.current->nextMoveDir = DIR_NONE;
			
	}
	else
	{
		printf("[g_updateGame]: unknown game.state!\n");
	}
	
	menu.nextMoveDir = DIR_NONE;
	//clear tetromino move values for next frame
//	game.current->nextMoveX = DIR_NONE;
//	game.current->nextMoveY = DIR_NONE;
//	game.current->nextMoveDir = DIR_NONE;
}

void g_getImageCoords(short i, short j, short * x, short * y)
{
	*x = PLAYING_FIELD_X_MIN + (BLOCK_WIDTH * i);
	*y = PLAYING_FIELD_Y_MAX - BLOCK_HEIGHT - (BLOCK_HEIGHT * j);
}

void g_drawGame()
{
	SDL_Surface * image = NULL;
	short i, j, x, y;
	block * b = NULL;
	
	if (game.state == STATE_MENU)
	{
		//draw the background followed by the buttons.
		//the 'selected' button will have a highlight drawn over it
		
		/*
		image = g_loadImage("./pictures/menubackground.bmp");
		
		if (image == NULL)
			printf("[g_drawGame]: error loading an image!\n");
			
		g_addSurface(0, 0, image, screen);
		*/
		switch (menu.menuLoc)
		{
			case M_MAIN:
				image = g_loadImage("./pictures/menubackground.bmp");
				g_addSurface(0, 0, image, screen);
				
				image = g_loadImage("./pictures/NEWGAME_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NEWGAME], buttonLocY[S_NEWGAME], image, screen);
				image = g_loadImage("./pictures/HIGHSCORE_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_HIGHSCORES], buttonLocY[S_HIGHSCORES], image, screen);
				image = g_loadImage("./pictures/EXIT_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_EXIT], buttonLocY[S_EXIT], image, screen);
				break;
			case M_HIGHSCORES:
				break;
			case M_PAUSE:
				image = g_loadImage("./pictures/menupaused.bmp");
				g_addSurface(0, 0, image, screen);
				
				image = g_loadImage("./pictures/CONTINUE_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_CONTINUE], buttonLocY[S_CONTINUE], image, screen);
				image = g_loadImage("./pictures/QUITGAME_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_QUIT], buttonLocY[S_QUIT], image, screen);
				break;
			case M_PAUSECONFIRMQUIT:
				image = g_loadImage("./pictures/menuconfirmquit.bmp");
				g_addSurface(0, 0, image, screen);
				
				image = g_loadImage("./pictures/YES_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_YES], buttonLocY[S_YES], image, screen);
				image = g_loadImage("./pictures/NO_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NO], buttonLocY[S_NO], image, screen);
				break;
			case M_EXIT:
				image = g_loadImage("./pictures/menu_exitconfirm.bmp");
				g_addSurface(0, 0, image, screen);
				
				image = g_loadImage("./pictures/YES_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_YES], buttonLocY[S_YES], image, screen);
				image = g_loadImage("./pictures/NO_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NO], buttonLocY[S_NO], image, screen);
				break;
			default:
				printf("[g_drawGame]: weird menu.menuLoc!\n");
				break;
		}
		image = g_loadImage("./pictures/BORDER_GREEN.bmp");
		g_addSurface(buttonLocX[menu.currentSelection], buttonLocY[menu.currentSelection], image, screen);
	}
	else if (game.state == STATE_PLAYING)
	{
		//draw the background,
		//then score, level, next block in stasis, and loop thru the 2d pos array
		//to draw all of the blocks
		image = g_loadImage("./pictures/gamescreen.bmp");
		
		if (image == NULL)
			printf("[g_drawGame]: error loading an image!\n");
			
		g_addSurface(0, 0, image, screen);
		
		
		//score, level, lines here
		
		//draw next block in the box
		for ( i = 0; i < TETRO_SIZE; i++ )
		{
			//image = g_loadImage("./pictures/blocktemp.bmp");
			image = g_loadBlockImage(tetro_getType(game.next));
			x = stasisPixelCoordX[ stasisCoord[ tetro_getType(game.next) ].xCoord[i] ];
			y = stasisPixelCoordY[ stasisCoord[ tetro_getType(game.next) ].yCoord[i] ];
			//printf("[g_drawGame]: next block pixel coords = (%d, %d)\n", x, y);
			
			g_addSurface(x, y, image, screen);
		}
		
		//draw every block in play
		for ( i = 0; i < SIZE_X; i++ )
		{
			for (j = 0; j < SIZE_Y; j++ )
			{
				b = g_getBlockAtPos(i, j);
				
				if (b != NULL)
				{
					image = g_loadBlockImage(block_getType(b));

					if (image == NULL)
					{
						printf("[g_drawGame]: failed to load block image!\n");
					}
					g_getImageCoords(i, j, &x, &y);
					g_addSurface(x, y, image, screen);
				}					
			}
			
		}
	}
	
	SDL_Flip(screen);
}

void g_init()
{
	int i, j;
	
	game.exitGameYet = false;
	game.state = STATE_MENU;
	
	//set grid array to NULL
	for ( i = 0; i < SIZE_X; i++ )
		for ( j = 0; j < SIZE_Y; j++ )
			game.pos[i][j] = NULL;
}

void g_clearGrid()
{
	int x, y;
	block * b;
	debug_msg("[g_clearGrid]: starting...\n");
	//set grid array to NULL
	for ( x = 0; x < SIZE_X; x++ )
	{
		for ( y = 0; y < SIZE_Y; y++ )
		{
			b = g_getBlockAtPos(x, y);
			//debug_msg("before if/else branches\n");
			
			if ( b == NULL )
			{
				printf("[g_clearGrid]: no block located at (%d, %d)\n", x, y);
			}
			else
			{
				if ( !block_clear(b) )
				{
					printf("[g_clearGrid]: block failed to clear! Clearing parent tetromino first...\n");
					tetro_clear(block_getParent(b));
					if ( !block_clear(b) )
						printf("[g_clearGrid]: block STILL failed to clear! WTF?\n");
					else
					{
						game.pos[x][y] = NULL;
						printf("[g_clearGrid]: game.pos[%d][%d] set to NULL!\n", x, y);
					}
				}
				else
				{
					game.pos[x][y] = NULL;
					printf("[g_clearGrid]: game.pos[%d][%d] set to NULL!\n", x, y);
				}
				//debug_msg("after if/else branches\n");
			}
		}
	}
}

void g_onDownBlocked()
{
	int rowTop = 0;
	int rowBottom = 0;
	short i, j, k;
	short x = -1, y = -1;
	int count = 0;
	SDL_Surface * blockFlash;
	block * b;
	int flashCount;
	
	short fullRows[4] = 
	{
		-1, -1, -1, -1
	};
	
	printf("[g_onDownBlocked]!\n");

	
	//calculate / move down lines here
	//only check the rows that the tetromino is/was at when it stopped
	//the above blocks only move down the number of rows equal to those cleared
	tetro_getRows(game.current, &rowTop, &rowBottom);
	printf("[g_onDownBlocked]: rowTop: %d, rowBottom: %d\n", rowTop, rowBottom);
	
	if ( !tetro_clear(game.current) )
	{
		printf("[g_onDownBlocked]: game.current failed to clear!\n");
	}
	
	for ( i = rowBottom; i <= rowTop; i++ )
	{
		if ( g_checkFullRow(i) )
		{
			printf("[g_onDownBlocked]: full row at %d!\n", i);
			fullRows[count] = i;
			count++;
		}
		else
			printf("[g_onDownBlocked]: no full row at %d!\n", i);
	}
	
	printf("[g_onDownBlocked]: number of rows filled = %d\n", count);
	
	//full line(s) exist, flash them then remove, then drop above blocks down
	if ( count > 0 )
	{
		printf("[g_onDownBlocked]: start flashing\n");
		//k = index for a row to flash
		//i = current x position
		//j = y position (row). fullRows[k]
		for ( flashCount = 0; flashCount < FLASH_FRAMES; flashCount++ )
		{
			for ( k = 0; k < count; k++ )
			{
				for ( i = 0; i < SIZE_X; i++ )
				{
					b = g_getBlockAtPos(i, fullRows[k]);
					
					if ( flashCount % 2 == 1 )
						blockFlash = g_loadBlockImage(block_getType(b));
					else
						blockFlash = g_loadImage("./pictures/BLOCK_FLASH1.bmp");
					g_getImageCoords(i, fullRows[k], &x, &y);
					g_addSurface(x, y, blockFlash, screen);
				}
			}
		
			SDL_Flip(screen);
			SDL_Delay(150);
		}
		printf("[g_onDownBlocked]: finished flashing\n");
		
		//remove all those blocks
		for ( k = 0; k < count; k++ )
		{
			for ( i = 0; i < SIZE_X; i++ )
			{
				b = g_getBlockAtPos(i, fullRows[k]);
				g_removeBlockFromPos(b);
				if ( block_clear(b) )
					printf("[g_onDownBlocked]: block at (%d, %d) removed\n", i, fullRows[k]);
				else
					printf("[g_onDownBlocked]: no block removed at (%d, %d)\n", i, fullRows[k]);
			}
		}
		
		//move downt he above blocks
		g_dropAboveBlocksDown((fullRows[0])+1, count);
	}
	
	
	
	game.current = game.next;
	game.next = NULL;
	game.next = tetro_create(TETRO_RANDOM);
	if ( !tetro_moveStart(game.current) )
	{
		printf("[g_onDownBlocked]: GAME OVER!\n");
		game.state = STATE_IDLE;
	}
}

boolean g_checkFullRow(int row)
{
	int i;
	
	for ( i = 0; i < SIZE_X; i++ )
	{
		printf("[g_checkFullRow]: checking row %d, column %d\n", row, i);
		if ( g_getBlockAtPos(i, row) == NULL )
			return false;
	}
	return true;
}

void g_dropAboveBlocksDown(int startRow, int count)
{
/*
currently a bug exists in the logic. right now it starts at the first full line row (which is now empty)
and goes from that row to the top, moving every block it comes across down a number equal to count.
what must be done however is each row moves down a number equal to the number of rows that were cleared below it
*/
	//start from bottom most row and go up
	int i, j;
	int currentRow, column;
	block * b = NULL;
	
	for ( currentRow = startRow; currentRow < SIZE_Y; currentRow++ )
	{
		for ( j = 0; j < SIZE_X; j++ )
		{
			b = g_getBlockAtPos(j, currentRow);
			if ( b != NULL )
			{
				for ( i = 0; i < count; i++ )
				{
					if ( block_move(b, DIR_SOUTH) )
					{
						printf("[g_dropAboveBlocksDown]: block at (%d, %d) moved down!\n", j, currentRow);
					}
					else
					{
						printf("[g_dropAboveBlocksDown]: block at (%d, %d) could not move down!\n", j, currentRow);
					}
				}
			}
		}
	}
}

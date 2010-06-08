#include "game.h"
#include "types.h"

#include "SDL/SDL.h" //needed for all SDL apps
#include <stdio.h> //for prinf
#include <stdlib.h> //for exit()

#include "menu.h"

extern SDL_Surface * screen; //the screen surface. originally declared in main.c
extern menu_t menu;
gameModule game; //game module, stores levle, 2d grid and what it contains, etc

/*
dir_t oppositeDir[] = 
{
	DIR_RIGHT, DIR_LEFT, DIR_DOWN,
	DIR_UP, DIR_NONE
};
*/
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

void g_draw()
{

}

void g_doInput()
{

}

void g_pause()
{

}

void g_clear(gameOverReason_t r)
{

}

void g_end()
{
	printf("Unloading SDL...\n");
	SDL_Quit();
}

void g_create()
{
	//set the grid array to NULL
	g_clearGrid();
	
	//set initial values for a gameModule new game
	game.exitGameYet = false;
	game.level = 1;
	game.score = 0;
	game.state = STATE_PLAYING; //state of the game
	game.next = NULL;
	game.current = NULL;

	//
}

boolean draw_bmp(char * filename)
{
	SDL_Surface * image;

	image = SDL_LoadBMP(filename);
	if (image == NULL) //fail
	{
		printf("Error loading image \"%s\"\n", filename);
		return false;
	}

	//if the image has a palette then use that to make colors better
	if (image->format->palette && screen->format->palette)
	{
		SDL_SetColors(screen, image->format->palette->colors, 0, image->format->palette->ncolors);
	}

	//blit image onto screen surface
	if (SDL_BlitSurface(image, NULL, screen, NULL) < 0 ) //<0 = fail
		printf("Image failed to blit: %s\n", SDL_GetError());

	//updated modified part of screen
	SDL_UpdateRect(screen, 0, 0, image->w, image->h);

	//free space allocated to bmp (its already on screen i guess)
	SDL_FreeSurface(image);
	return true;
}

boolean g_addSurface()
{

	return true;
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
				printf("[g_handleInput()]: A key was pushed down!\n");
				if (game.state = STATE_MENU)
				{
					switch (event.key.keysym.sym) //figure out what key
					{
						case SDLK_DOWN:
							if (menu.nextMoveY == DIR_NONE)
								menu.nextMoveY = DIR_SOUTH;
							else if (menu.nextMoveY == DIR_NORTH)
							{
								//cancel out opposite inputs
								menu.nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_UP:
							if (menu.nextMoveY == DIR_NONE)
								menu.nextMoveY = DIR_NORTH;
							else if (menu.nextMoveY == DIR_SOUTH)
							{
								//cancel out opposite inputs
								menu.nextMoveY == DIR_NONE;
							}
							break;
						//case enter:
						/*
							if (menu.forwardOrBack == DIR_NONE)
								menu.forwardOrBack = DIR_NORTH;
							else if (menu.forwardOrBack == DIR_SOUTH) //scheduled to go back
								menu.forwardOrBack = DIR_NONE;
						case ESCAPE!:
							if (menu.forwardOrBack == DIR_NONE)
								menu.forwardOrBack = DIR_SOUTH;
							else if (menu.forwardOrBack == DIR_NORTH) //scheduled to go forward
								menu.forwardOrBack = DIR_NONE;
						/*
						case SDLK_LEFT:
							if (menu.nextMoveX == DIR_NONE)
								menu.nextMoveX = DIR_WEST;
							else if (menu.nextMoveX == DIR_EAST)
							{
								//cancel out opposite inputs
								menu.nextMoveY == DIR_NONE;
							}
							break;
						case SDLK_RIGHT:
							if (menu.nextMoveX == DIR_NONE)
								menu.nextMoveX = DIR_EAST;
							else if (menu.nextMoveX == DIR_WEST)
							{
								//cancel out opposite inputs
								menu.nextMoveY == DIR_NONE;
							}
							break;
						*/
						//case escape (bring up pause menu, or go back thru any menu that your in.)
						//
						default:
							break;
					}
				}
				else if (game.state = STATE_PLAYING)
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
						//case escape (bring up pause menu, or go back thru any menu that your in.)
						//case spacebar (rotate tertromino in this case)
						default:
							break;
					}
				}
				break;
			case SDL_KEYUP:
				printf("[g_handleInput()]: A key was pushed up!\n");
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
				printf("[g_handleInput()]: A mouse button was pushed down!\n");
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						//find out what menu we are on, and if we clicked on a button,
						//if we clicked a button then set a variable in the menu to be handled
						break;
				}
				break;
			case SDL_QUIT: //user clicked the x on the window
				printf("[g_handleInput()]: X clicked!\n");
				game.exitGameYet = true;
				break;
		}
	}			
}

void g_updateGame()
{
	if (game.state == STATE_MENU)
	{
		if (DEBUG_MODE)
		{
			frame++;
			if (frame == 61)
				frame = 1;
			printf("[g_updateGame]: FRAME = %d\n", frame);
		}
		
		if ( !m_move() )
			printf("[g_updateGame]: m_move failed!\n");
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
			if ( game.current->nextMoveDir == DIR_SOUTH )
			{
				//check for full lines, and call next tetromino into play
			}
		}
			
		//clear tetromino move values for next frame
		game.current->nextMoveX = DIR_NONE;
		game.current->nextMoveY = DIR_NONE;
		game.current->nextMoveDir = DIR_NONE;

	}
	else
	{
		printf("[g_updateGame]: unknown game.state!\n");
	}
}

void g_drawGame()
{

}

void g_init()
{
	game.exitGameYet = false;
	game.state = STATE_MENU;
}

void g_clearGrid()
{
	int i, j;
	
	//set grid array to NULL
	for ( i = 0; i < SIZE_X; i++ )
		for ( j = 0; j < SIZE_Y; j++ )
			game.pos[i][j] = NULL;
}

void g_onDownBlocked()
{

}

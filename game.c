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
/*
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
} menuSelection_t;
*/

//pixel locations of the top left corner of each button image
int buttonLocX[] = 
{
	
};

int buttonLocY[] = 
{
	
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

void g_clear(gameOverReason_t r)
{
	//unload unneeded resources
	g_clearGrid();
	tetro_clear(game.next);
	tetro_clear(game.current); //should already be cleared by g_clearGrid() since its on the grid somewhere
	
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

SDL_Surface * g_loadImage(char * filename)
{
	SDL_Surface * loaded = NULL;
	SDL_Surface * image = NULL;
	
	loaded = SDL_LoadBMP(filename);
	if ( loaded != NULL )
	{
		image = SDL_DisplayFormat(loaded); //converts loaded bmp into screen settings (if not already set to them)
		SDL_FreeSurface(loaded);
	}
	else
		return NULL;
		
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
				if (game.state = STATE_MENU)
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
						case SDLK_ESCAPE: //(bring up pause menu, or go back thru any menu that your in.)
						
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
		{
			//too much output
			//printf("[g_updateGame]: m_move returned false!\n");
		}
		//clean up menu options for next frame
		menu.nextMoveDir = DIR_NONE;
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
				g_onDownBlocked();
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
		//the 'selected' button will have its highlighted version drawn
		image = g_loadImage("./pictures/menubackground.bmp");
		
		if (image == NULL)
			printf("[g_drawGame]: error loading an image!\n");
			
		g_addSurface(0, 0, image, screen);

		switch (menu.menuLoc)
		{
			case M_MAIN:
				break;
			case M_HIGHSCORES:
				break;
			case M_PAUSE:
				break;
			case M_PAUSECONFIRMQUIT:
				break;
			case M_EXIT:
				break;
			default:
				printf("[g_drawGame]: weird menu.menuLoc!\n");
				break;
		}
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
		
		//next block here
		
		//draw every block in play
		for ( i = 0; i < SIZE_X; i++ )
		{
			for (j = 0; j < SIZE_Y; j++ )
			{
				b = g_getBlockAtPos(i, j);
				
				if (b != NULL)
				{
					switch (block_getType(b))
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

	//set grid array to NULL
	for ( x = 0; x < SIZE_X; x++ )
	{
		for ( y = 0; y < SIZE_Y; y++ )
		{
			b = g_getBlockAtPos(x, y);
			
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
		}
	}
}

void g_onDownBlocked()
{

}

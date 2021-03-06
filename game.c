#include "game.h"
#include "types.h"

#include "SDL/SDL.h" //needed for all SDL apps
#include "SDL/SDL_mixer.h"
#include <stdio.h> //for prinf
#include <stdlib.h> //for exit()

#include "menu.h"

#include <time.h>
extern SDL_Surface * screen; //the screen surface. originally declared in main.c
//SDL_Surface * image; //used by g_drawGame()
extern menu_t menu; //needed in this file to set its values in g_handleInput()
gameModule game; //game module, stores levle, 2d grid and what it contains, etc
extern highScoresStruct_t highScores;
extern int debug_mode;

//keeps track of how many rows are filled below row [ ].
//used to figure out how far to move down blocks when erasing lines
int numRowsFilledBelow[SIZE_Y];
int dropFrameInterval = 0;

extern char * highScoresNameArray[];

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
coord_t stasisCoord[] = 
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

//how many frames between each periodic drop down (indexed based on level)
int dropIntervalPerRow[] = 
{
	53, 49, 45, 41, 37,
	33, 28, 22, 17, 11,
	10, 9, 8, 7, 6,
	6, 5, 5, 4, 4,
	3
};

char numIcon0[] = "./pictures/NUM0.bmp";
char numIcon1[] = "./pictures/NUM1.bmp";
char numIcon2[] = "./pictures/NUM2.bmp";
char numIcon3[] = "./pictures/NUM3.bmp";
char numIcon4[] = "./pictures/NUM4.bmp";
char numIcon5[] = "./pictures/NUM5.bmp";
char numIcon6[] = "./pictures/NUM6.bmp";
char numIcon7[] = "./pictures/NUM7.bmp";
char numIcon8[] = "./pictures/NUM8.bmp";
char numIcon9[] = "./pictures/NUM9.bmp";

char blockIconI[] = "./pictures/BLOCK_I.bmp";
char blockIconJ[] = "./pictures/BLOCK_J.bmp";
char blockIconL[] = "./pictures/BLOCK_L.bmp";
char blockIconO[] = "./pictures/BLOCK_O.bmp";
char blockIconS[] = "./pictures/BLOCK_S.bmp";
char blockIconT[] = "./pictures/BLOCK_T.bmp";
char blockIconZ[] = "./pictures/BLOCK_Z.bmp";
char blockIconDEAD[] = "./pictures/BLOCK_DEAD.bmp";

char * numIcon[] = 
{
	numIcon0,
	numIcon1,
	numIcon2,
	numIcon3,
	numIcon4,
	numIcon5,
	numIcon6,
	numIcon7,
	numIcon8,
	numIcon9
};

char * blockIcon[] = 
{
	blockIconI,
	blockIconJ,
	blockIconL,
	blockIconO,
	blockIconS,
	blockIconT,
	blockIconZ,
	blockIconDEAD,
	NULL //TETRO_RANDOM
};

//location on the sprite sheet for characters
spriteLoc_t chars[CHAR_SIZE] = 
{
	{0,0,CHAR_WIDTH,CHAR_HEIGHT}, //A
	{20,0,CHAR_WIDTH,CHAR_HEIGHT}, //B
	{40,0,CHAR_WIDTH,CHAR_HEIGHT}, //C
	{60,0,CHAR_WIDTH,CHAR_HEIGHT}, //D
	{80,0,CHAR_WIDTH,CHAR_HEIGHT}, //E
	{100,0,CHAR_WIDTH,CHAR_HEIGHT}, //F
	{120,0,CHAR_WIDTH,CHAR_HEIGHT}, //G
	{140,0,CHAR_WIDTH,CHAR_HEIGHT}, //H
	{160,0,CHAR_WIDTH,CHAR_HEIGHT}, //I
	{180,0,CHAR_WIDTH,CHAR_HEIGHT}, //J
	{200,0,CHAR_WIDTH,CHAR_HEIGHT}, //K
	{220,0,CHAR_WIDTH,CHAR_HEIGHT}, //L
	{240,0,CHAR_WIDTH,CHAR_HEIGHT}, //M
	{260,0,CHAR_WIDTH,CHAR_HEIGHT}, //N
	{280,0,CHAR_WIDTH,CHAR_HEIGHT}, //O
	{300,0,CHAR_WIDTH,CHAR_HEIGHT}, //P
	{320,0,CHAR_WIDTH,CHAR_HEIGHT}, //Q
	{340,0,CHAR_WIDTH,CHAR_HEIGHT}, //R
	{360,0,CHAR_WIDTH,CHAR_HEIGHT}, //S
	{380,0,CHAR_WIDTH,CHAR_HEIGHT}, //T
	{400,0,CHAR_WIDTH,CHAR_HEIGHT}, //U
	{420,0,CHAR_WIDTH,CHAR_HEIGHT}, //V
	{440,0,CHAR_WIDTH,CHAR_HEIGHT}, //W
	{460,0,CHAR_WIDTH,CHAR_HEIGHT}, //X
	{480,0,CHAR_WIDTH,CHAR_HEIGHT}, //Y
	{500,0,CHAR_WIDTH,CHAR_HEIGHT}, //Z
	{520,0,CHAR_WIDTH,CHAR_HEIGHT}, //UNDERSCORE
	{540,0,CHAR_WIDTH,CHAR_HEIGHT}, //SPACE
};
	
extern Mix_Chunk * downBlock;
extern Mix_Chunk * gameOver;
extern Mix_Chunk * lineClear;
extern Mix_Chunk * menuMove;
extern Mix_Chunk * menuSelect;
extern Mix_Chunk * moveSideways;
extern Mix_Chunk * rotate;

//sounds_e menuSound = SND_NONE;

int g_getScore()
{
	return game.score;
}

int g_getLevel()
{
	return game.level;
}

boolean g_isLocOutOfBounds(int x, int y)
{
	if ( x < X_MIN || x > X_MAX || y < Y_MIN || y > Y_MAX ) //destination is out of bounds
		return true;
	else
		return false;
}

block * g_getBlockAtPos(int x, int y)
{
	//debug_msg("[g_getBlockAtPos]: starting...\n");
	
	if ( g_isLocOutOfBounds(x, y) )
		return NULL;
	
	return game.pos[x][y];
}

boolean g_setBlockToPos(block * b, int x, int y)
{
	int x_old = block_getLocX(b);
	int y_old = block_getLocY(b);
	block * b2 = g_getBlockAtPos(x, y);

	if ( b2 != NULL || g_isLocOutOfBounds(x, y) ) //another block is already here at destination
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
	
	if ( g_isLocOutOfBounds(x, y) )
	{
		debug_msg("[g_removeBlockFromPos]: target is out of bounds!\n");
		return false;
	}
	
	game.pos[x][y] = NULL;
	return true;
}

void g_clear(gameOverReason_t r)
{
	int i;
	boolean newHighScore = false;
	SDL_Surface * image;
	
	if (r == REASON_LOSS)
	{
		SDL_Delay(500);
		
		printf("Game over...\n");
		image = g_loadImage("./pictures/GAME_OVER.bmp");
		g_addSurface(TEXT_GAMEOVER_X, TEXT_GAMEOVER_Y, image, screen, NULL);
		SDL_FreeSurface(image);
		SDL_Flip(screen);
		
		SDL_Delay(GAME_OVER_PAUSE);
		
		for ( i = 0; i < HIGH_SCORES_LIST_SIZE; i++ )
		{
			if (game.score > highScores.scores[i])
			{
				newHighScore = true;
				game.highScoreIndexToReplace = i;
				break;
			}
		}
	}
	
	//clear tetrominoz
	tetro_clear(game.next);
	tetro_clear(game.current);
	g_clearGrid();
			
	game.state = STATE_MENU;
	
	if (newHighScore)
	{
		menu.nextMoveDir = DIR_NONE;
		menu.menuLoc = M_NEWHIGH;
		menu.currentSelection = S_IDLE,
		game.newHighScore[0] = ' ';
		game.newHighScore[1] = 0;
		game.newHighScore[2] = 0;
		game.newHighScore[3] = 0;
	}
	else
		m_init();
}

void g_end()
{
	printf("Unloading SDL...\n");
	SDL_Quit();
	s_clean();
}

void g_create()
{
	int i;
	SDL_Surface * image;
	
	//set initial values for a gameModule new game
	game.level = 0;
	game.score = 0;
	game.lines = 0;
	game.state = STATE_IDLE;
	game.next = NULL;
	game.current = NULL;
	//game.currentTime = 0;
	
	for ( i = 0; i < SIZE_Y; i++ )
		numRowsFilledBelow[i] = 0;
	
	//background
	image = g_loadImage("./pictures/gamescreen.bmp");
	if (image == NULL)
		debug_msg("[g_drawGame]: error loading an image!\n");
	g_addSurface(0, 0, image, screen, NULL);
	SDL_FreeSurface(image);
	
	//score, level, lines 0
	image = g_loadImage(numIcon[0]);
	g_addSurface(SCORE_FIRST_DIGIT_X, SCORE_IMAGE_Y, image, screen, NULL);
	g_addSurface(LEVEL_FIRST_DIGIT_X, LEVEL_IMAGE_Y, image, screen, NULL);
	g_addSurface(LINES_FIRST_DIGIT_X, LINES_IMAGE_Y, image, screen, NULL);
	SDL_FreeSurface(image);
	
	printf("Get Ready...\n");
	image = g_loadImage("./pictures/GET_READY.bmp");
	g_addSurface(TEXT_GETREADY_X, TEXT_GETREADY_Y, image, screen, NULL);
	SDL_FreeSurface(image);
	SDL_Flip(screen);
	
	//make random numbers more random
	srand( time(NULL) );
	
	SDL_Delay(2000); //2 seconds
	
	printf("GO GO GO\n");
	image = g_loadImage("./pictures/GOGOGO.bmp");
	g_addSurface(TEXT_GOGOGO_X, TEXT_GOGOGO_Y, image, screen, NULL);
	SDL_FreeSurface(image);
	SDL_Flip(screen);
	
	SDL_Delay(500);
	
	game.current = tetro_create(TETRO_RANDOM);
	game.next = tetro_create(TETRO_RANDOM);
	tetro_moveToStart(game.current);
	game.state = STATE_PLAYING;
	//game.lastDropTime = SDL_GetTicks();
	
	game.isSoftDropping = false;
	game.dasDir = DIR_NONE;
	game.dasDelaying = true;
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
	
	image = g_loadImage(blockIcon[type]);
	
	return image;
}

boolean g_addSurface(int x, int y, SDL_Surface * source, SDL_Surface * dest, SDL_Rect * clip)
{
	SDL_Rect offset;
	
	offset.x = x;
	offset.y = y;
	
	if ( SDL_BlitSurface(source, clip, dest, &offset) < 0 ) //<0 = fail
	{
		//SDL_FreeSurface(source);
		return false;
	}
	else
	{
		//SDL_FreeSurface(source);
		return true;
	}
}

void g_blitMessageToLoc(int x, int y, char * msg)
{
	SDL_Surface * charSheet = NULL;
	SDL_Rect clip;
	characters_t currentChar;
	
	int i = 0;
	
	charSheet = g_loadImage("./pictures/char_sheet.bmp");
	
	if ( charSheet == NULL )
	{
		debug_msg("[g_blitMessageToLoc]: charSheet failed to load!\n");
		return;
	}
	
	while ( msg[i] != 0 )
	{
		//printf("[g_blitMessageToLoc]: msg[%d] = %c\n", i, msg[i]);
		if ( msg[i] == '_' )
			currentChar = UNDERSCORE;
		else if ( msg[i] == ' ' )
			currentChar = SPACE;
		else
			currentChar = msg[i] - (int)('A'); //toUpper'd
			
		clip.x = chars[currentChar].x;
		clip.y = chars[currentChar].y;
		clip.w = chars[currentChar].width;
		clip.h = chars[currentChar].height;
		
		g_addSurface(x, y, charSheet, screen, &clip);
		
		x += clip.w;
		i++;
	}
	SDL_FreeSurface(charSheet);
}

void g_blitNumbersToLoc(int x, int y, int number)
{
	int modifier = 1; //short int is too small
	short count = 0;
	int quotient = number / modifier;
	short i;
	SDL_Surface * image;
	
	while (quotient > 0) //stops after score runs out of digits
	{	
		i = quotient % 10;
		image = g_loadImage(numIcon[i]);
		g_addSurface(x-(NUMBER_LENGTH*count), y, image, screen, NULL);
		SDL_FreeSurface(image);
			
		count++;
		modifier *= 10;
		quotient = number / modifier;
	}
}

void debug_msg(char * msg)
{
	if (debug_mode)
		printf("%s", msg);
}

void g_loop()
{
	int frame = 0;
	int startTime;
	int endTime;

	while ( !game.exitGameYet )
	{
		startTime = SDL_GetTicks();
		//get time passed since last loop and see if enough time has passed for another update (target = 60 fps)
		//take note of the current time (for next loop around)
		
		
		//handle input, this should do nothing more then set values on objects (like tetrominos) so they know what to do next
		g_handleInput();
		
		//update the game (moving, scoring, menu seletion etc). this should look at the values set by g_handleInput() and update accordingly
		g_updateGame();
		
		//update the display with latest updated data clear the screen and redraw everything
		g_drawGame();
		frame++;
		//printf("frame %d\n", frame);
		
		endTime = SDL_GetTicks() - startTime;
		
		if ( endTime < (ONE_SECOND / TARGET_FRAME_RATE) )
			SDL_Delay( (ONE_SECOND / TARGET_FRAME_RATE) - endTime );
			
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
						case SDLK_LEFT:
							if (menu.nextMoveDir == DIR_NONE)
								menu.nextMoveDir = DIR_NORTHWEST;
							else if (menu.nextMoveDir == DIR_NORTHEAST)
							{
								//cancel out opposite inputs
								menu.nextMoveDir = DIR_NONE;
							}
							break;
						case SDLK_RIGHT:
							if (menu.nextMoveDir == DIR_NONE)
								menu.nextMoveDir = DIR_NORTHEAST;
							else if (menu.nextMoveDir == DIR_NORTHWEST)
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
							/*
							if (game.current->nextMoveY == DIR_NONE)
								game.current->nextMoveY = DIR_SOUTH;
							else if (game.current->nextMoveY == DIR_NORTH)
							{
								//cancel out opposite inputs
								game.current->nextMoveY == DIR_NONE;
							}
							*/
							game.softDropFrame = SOFT_DROP_FRAMES;
							game.isSoftDropping = true;
							game.softDropDistanceCount = 0;
							break;
						case SDLK_UP:
							if (!debug_mode)
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
							if ( game.dasDir == DIR_NONE )
							{
								if (game.current->nextMoveX == DIR_NONE)
									game.current->nextMoveX = DIR_WEST;
								else if (game.current->nextMoveX == DIR_EAST)
								{
									//cancel out opposite inputs
									game.current->nextMoveY == DIR_NONE;
								}
								game.dasDir = DIR_WEST;
								game.dasFrame = 0;
								game.dasDelaying = true;
							}
							else if ( game.dasDir == DIR_EAST ) //cancel das
							{
								game.dasDir = DIR_NONE;
							}
							break;
						case SDLK_RIGHT:
							if ( game.dasDir == DIR_NONE )
							{
								if (game.current->nextMoveX == DIR_NONE)
									game.current->nextMoveX = DIR_EAST;
								else if (game.current->nextMoveX == DIR_WEST)
								{
									//cancel out opposite inputs
									game.current->nextMoveY == DIR_NONE;
								}
								game.dasDir = DIR_EAST;
								game.dasFrame = 0;
								game.dasDelaying = true;
							}
							else if ( game.dasDir == DIR_WEST ) //cancel das
							{
								game.dasDir = DIR_NONE;
							}
							break;
						case SDLK_ESCAPE: //(bring up pause menu, or go back thru any menu that your in.)
							game.state = STATE_MENU;
							menu.currentSelection = S_CONTINUE;
							menu.menuLoc = M_PAUSE;
							break;
						case SDLK_SPACE: //(rotate tertromino in this case)
							game.current->tryRotate = true;
							break;
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
						game.softDropFrame = 0;
						game.isSoftDropping = false;
						dropFrameInterval = dropIntervalPerRow[game.level] / 2; //dont want block to autodrop riiiiight away
						break;
					case SDLK_LEFT:
						if ( game.dasDir == DIR_WEST )
						{
							game.dasDir = DIR_NONE;
						}
						break;
					case SDLK_RIGHT:
						if ( game.dasDir == DIR_EAST )
						{
							game.dasDir = DIR_NONE;
						}
						break;
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
		//menuSound = m_move;
		
		switch ( m_move() )
		{
			case SND_MENUMOVE:
				s_playSound(menuMove);
				break;
			case SND_MENUSELECT:
				s_playSound(menuSelect);
				break;
			case SND_NONE:
			default:
				break;
		}
		/*
		if ( !m_move() )
		{
			//too much output
			//printf("[g_updateGame]: m_move returned false!\n");
		}
		*/
	}
	else if (game.state == STATE_PLAYING)
	{	
		//game.currentTime = SDL_GetTicks();
		
		//take note of the frame here, see if it is time to move the current tetro
		//1 notch down
		if ( dropFrameInterval >= dropIntervalPerRow[game.level] && !game.isSoftDropping ) //time to drop down 1, only if down arrow not already held down
		{
			dropFrameInterval = 0;

			if ( !tetro_move(game.current, DIR_SOUTH) ) //the periodic drop down was blocked
			{
				//printf("[g_updateGame]: periodic drop down blocked!\n");
				g_onDownBlocked();
			}
			//game.lastDropTime = game.currentTime;
		}
		
		
			
		//rotate has priority
		if ( game.current->tryRotate )
		{
			debug_msg("[g_updateGame]: trying rotate...\n");
			if ( tetro_rotate(game.current) )
			{
				debug_msg("[g_updateGame]: rotate success!\n");
				s_playSound(rotate);
			}
			else
				debug_msg("[g_updateGame]: rotate failed!\n");
		}
		
		//for delayed auto shifting
		if ( game.dasDir != DIR_NONE )
		{
			game.current->nextMoveY = DIR_NONE;
			if ( game.dasDelaying )
			{
				if ( game.dasFrame >= DAS_DELAY )
				{
					game.dasDelaying = false;
					game.current->nextMoveX = game.dasDir;
					game.dasFrame = 0;
				}
			}
			else
			{
				if ( game.dasFrame >= DAS_PERIOD )
				{
					game.current->nextMoveX = game.dasDir;
					game.dasFrame = 0;
				}
			}
			game.dasFrame++;
		}
		//printf("game.dasFrame = %d\n", game.dasFrame);
		
		if ( game.isSoftDropping )
		{
			if ( game.softDropFrame >= SOFT_DROP_FRAMES ) //time to move down a notch
			{
				game.softDropFrame = 0;
				if ( !tetro_move(game.current, DIR_SOUTH) )
				{
					//printf("[g_updateGame]: periodic drop down blocked!\n");
					g_onDownBlocked();
					game.softDropFrame = SOFT_DROP_FRAMES; //next block insta drop down
					game.score += game.softDropDistanceCount;
					game.softDropDistanceCount = 0;
				}
				else
					game.softDropDistanceCount++;
			}
			game.softDropFrame++;
		}
		else
		{
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
				printf("move failed no sound\n");
				//printf("[g_updateGame]: tetro_move failed!\n");
				if ( game.current->nextMoveDir == DIR_SOUTH  || game.current->nextMoveDir == DIR_SOUTHWEST || game.current->nextMoveDir == DIR_SOUTHEAST )
				{
					g_onDownBlocked();
				}
			}
			else if ( game.current->nextMoveDir == DIR_WEST || game.current->nextMoveDir == DIR_EAST ) //successful move, check to see if i should play a sound
			{
				printf("play move because move succeeded west or east\n");
				//if ( game.dasFrame == 0 && game.dasDelaying == true )
					s_playSound(moveSideways);
			}
		}
		
		game.current->nextMoveX = DIR_NONE;
		game.current->nextMoveY = DIR_NONE;
		game.current->nextMoveDir = DIR_NONE;
		game.current->tryRotate = false;

		dropFrameInterval++;
	}
	else if (game.state == STATE_LOSS)
	{
		//the idea: every frame fill up one of the spaces with a "dead" block. so in 180 frames the whole screen will be filled
		short i;
		block * b;
		
		if (game.lossFrame == -1)
		{
			//do nothing
		}
		else
		{
			if (game.lossCurrentRow >= SIZE_Y)
			{
				g_clear(REASON_LOSS);
			}
			else
			{
				if (game.lossFrame == 0)
					SDL_Delay(LOSS_PAUSE);
				
				if (game.lossFrame % 3 == 0)
				{
					for ( i = 0; i < SIZE_X; i++ )
					{
						b = g_getBlockAtPos(i, game.lossCurrentRow);
						if (b != NULL)
							g_removeBlockFromPos(b);
						b = NULL;
					
						b = block_create(TETRO_DEAD, NULL);
						if ( !block_teleport(b, i, game.lossCurrentRow) )
							debug_msg("[g_updateGame]: STATE_LOSS- error teleporting dead block into position!\n");
						b = NULL;
					}
					game.lossCurrentRow++;
				}
			}
		}
		
		game.lossFrame++;
	}		
	else
		debug_msg("[g_updateGame]: unknown game.state!\n");
	
	menu.nextMoveDir = DIR_NONE;
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
	int quotient;
	short modifier, count;
	
	if (game.state == STATE_MENU)
	{
		//draw the background followed by the buttons.
		//the 'selected' button will have a highlight drawn over it
		
		switch (menu.menuLoc)
		{
			case M_MAIN:
				image = g_loadImage("./pictures/menubackground.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/NEWGAME_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NEWGAME], buttonLocY[S_NEWGAME], image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/HIGHSCORE_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_HIGHSCORES], buttonLocY[S_HIGHSCORES], image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/EXIT_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_EXIT], buttonLocY[S_EXIT], image, screen, NULL);
				SDL_FreeSurface(image);
				break;
			case M_HIGHSCORES:
				image = g_loadImage("./pictures/highscores_background.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				x = HIGH_SCORES_FIRST_LETTER_LOC_X;
				y = HIGH_SCORES_FIRST_LETTER_LOC_Y;
				
				for ( i = 0; i < HIGH_SCORES_LIST_SIZE; i++ )
				{
					g_blitMessageToLoc(x, y, highScoresNameArray[i]);
					g_blitNumbersToLoc(HIGH_SCORES_FIRST_NUMBERS_LOC_X, y, highScores.scores[i]);
					
					y = y + (HIGH_SCORES_GAP + HIGH_SCORES_GAP);
				}
				
				break;
			case M_NEWHIGH:
				image = g_loadImage("./pictures/newhighscoresentry.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				g_blitMessageToLoc(NEW_HIGH_SCORE_X, NEW_HIGH_SCORE_Y, game.newHighScore);
				
				i = 0;
				
				while ( game.newHighScore[i] != 0 )
					i++;	
				i--;
				
				g_blitMessageToLoc(NEW_HIGH_SCORE_X+(CHAR_WIDTH*i), NEW_HIGH_SCORE_Y, "_");
				break;
			case M_PAUSE:
				image = g_loadImage("./pictures/menupaused.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/CONTINUE_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_CONTINUE], buttonLocY[S_CONTINUE], image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/QUITGAME_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_QUIT], buttonLocY[S_QUIT], image, screen, NULL);
				SDL_FreeSurface(image);
				break;
			case M_PAUSECONFIRMQUIT:
				image = g_loadImage("./pictures/menuconfirmquit.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/YES_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_YES], buttonLocY[S_YES], image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/NO_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NO], buttonLocY[S_NO], image, screen, NULL);
				SDL_FreeSurface(image);
				break;
			case M_EXIT:
				image = g_loadImage("./pictures/menu_exitconfirm.bmp");
				g_addSurface(0, 0, image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/YES_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_YES], buttonLocY[S_YES], image, screen, NULL);
				SDL_FreeSurface(image);
				
				image = g_loadImage("./pictures/NO_NOBORDER.bmp");
				g_addSurface(buttonLocX[S_NO], buttonLocY[S_NO], image, screen, NULL);
				SDL_FreeSurface(image);
				break;
			
			default:
				printf("[g_drawGame]: weird menu.menuLoc!\n");
				break;
		}
		//no selection border for high scores screen
		if ( menu.menuLoc != M_HIGHSCORES  && menu.menuLoc != M_NEWHIGH )
		{
			image = g_loadImage("./pictures/BORDER_GREEN.bmp");
			g_addSurface(buttonLocX[menu.currentSelection], buttonLocY[menu.currentSelection], image, screen, NULL);
			SDL_FreeSurface(image);
		}
	}
	else if (game.state == STATE_PLAYING || game.state == STATE_LOSS)
	{
		//draw the background,
		//then score, level, next block in stasis, and loop thru the 2d pos array
		//to draw all of the blocks
		image = g_loadImage("./pictures/gamescreen.bmp");
		
		if (image == NULL)
			debug_msg("[g_drawGame]: error loading an image!\n");
			
		g_addSurface(0, 0, image, screen, NULL);
		SDL_FreeSurface(image);
		
		//SCORE
		modifier = 1;
		count = 0;
		quotient = game.score / modifier;
		
		if ( game.score == 0 )
		{
			image = g_loadImage(numIcon[0]);
			g_addSurface(SCORE_FIRST_DIGIT_X, SCORE_IMAGE_Y, image, screen, NULL);
			SDL_FreeSurface(image);
		}
		else
		{
			if ( game.score >= MAX_SCORE )
				g_blitNumbersToLoc(SCORE_FIRST_DIGIT_X, SCORE_IMAGE_Y, MAX_SCORE);
			else
				g_blitNumbersToLoc(SCORE_FIRST_DIGIT_X, SCORE_IMAGE_Y, game.score);
		}

		//LEVEL
		modifier = 1;
		count = 0;
		quotient = game.level / modifier;
		
		if ( game.level == 0 )
		{
			image = g_loadImage(numIcon[0]);
			g_addSurface(LEVEL_FIRST_DIGIT_X, LEVEL_IMAGE_Y, image, screen, NULL);
			SDL_FreeSurface(image);
		}
		else
		{
			if ( game.level >= MAX_LEVEL )
				g_blitNumbersToLoc(LEVEL_FIRST_DIGIT_X, LEVEL_IMAGE_Y, MAX_LEVEL);
			else
				g_blitNumbersToLoc(LEVEL_FIRST_DIGIT_X, LEVEL_IMAGE_Y, game.level);
		}
		
		//LINES
		modifier = 1;
		count = 0;
		quotient = game.lines / modifier;
		
		if ( game.lines == 0 )
		{
			image = g_loadImage(numIcon[0]);
			g_addSurface(LINES_FIRST_DIGIT_X, LINES_IMAGE_Y, image, screen, NULL);
			SDL_FreeSurface(image);
		}
		else
		{
			g_blitNumbersToLoc(LINES_FIRST_DIGIT_X, LINES_IMAGE_Y, game.lines);
		}		

		
		//draw next block in the box
		if (game.next != NULL)
		{
			image = g_loadBlockImage(tetro_getType(game.next));
			for ( i = 0; i < TETRO_SIZE; i++ )
			{
				//image = g_loadImage("./pictures/blocktemp.bmp");
				image = g_loadBlockImage(tetro_getType(game.next));
				x = stasisPixelCoordX[ stasisCoord[ tetro_getType(game.next) ].xCoord[i] ];
				y = stasisPixelCoordY[ stasisCoord[ tetro_getType(game.next) ].yCoord[i] ];
				//printf("[g_drawGame]: next block pixel coords = (%d, %d)\n", x, y);

				g_addSurface(x, y, image, screen, NULL);
			}
			SDL_FreeSurface(image);
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
						debug_msg("[g_drawGame]: failed to load block image!\n");
					}
					g_getImageCoords(i, j, &x, &y);
					g_addSurface(x, y, image, screen, NULL);
					SDL_FreeSurface(image);
				}					
			}
			
		}
	}
	else
		debug_msg("[g_drawGame]: unrecognized game.state!\n");
		
	
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
				//printf("[g_clearGrid]: no block located at (%d, %d)\n", x, y);
			}
			else
			{
				if ( !block_clear(b) )
				{
					//printf("[g_clearGrid]: block failed to clear! Clearing parent tetromino first...\n");
					tetro_clear(block_getParent(b));
					if ( !block_clear(b) )
						debug_msg("[g_clearGrid]: block STILL failed to clear! WTF?\n");
					else
					{
						game.pos[x][y] = NULL;
						//printf("[g_clearGrid]: game.pos[%d][%d] set to NULL!\n", x, y);
					}
				}
				else
				{
					game.pos[x][y] = NULL;
					//printf("[g_clearGrid]: game.pos[%d][%d] set to NULL!\n", x, y);
				}
				//debug_msg("after if/else branches\n");
			}
		}
	}
}

void g_onDownBlocked()
{
	int rowTop = -1;
	int rowBottom = -1;
	short i, j, k;
	short x = -1, y = -1;
	int count = 0;
	//SDL_Surface * blockFlash;
	SDL_Surface * flash1;
	SDL_Surface * flash2;
	block * b;
	int flashCount;
	
	short fullRows[4] = 
	{
		-1, -1, -1, -1
	};
	
	debug_msg("[g_onDownBlocked]!\n");

	
	//calculate / move down lines here
	//only check the rows that the tetromino is/was at when it stopped
	//the above blocks only move down the number of rows equal to those cleared
	tetro_getRows(game.current, &rowTop, &rowBottom);
	//printf("[g_onDownBlocked]: rowTop: %d, rowBottom: %d\n", rowTop, rowBottom);
	
	if ( !tetro_clear(game.current) )
	{
		debug_msg("[g_onDownBlocked]: game.current failed to clear!\n");
	}
	
	for ( i = rowBottom; i <= rowTop; i++ )
	{
		if ( g_checkFullRow(i) )
		{
			//printf("[g_onDownBlocked]: full row at %d!\n", i);
			fullRows[count] = i;
			count++;
			game.lines++;
		}
		else
		{
			//printf("[g_onDownBlocked]: no full row at %d!\n", i);
		}
	}
	
	//printf("[g_onDownBlocked]: number of rows filled = %d\n", count);
	
	switch (count)
	{
		case 1:
			game.score += SCORE_MODIFIER_ONE_LINES * (game.level+1);
			break;
		case 2:
			game.score += SCORE_MODIFIER_TWO_LINES * (game.level+1);
			break;
		case 3:
			game.score += SCORE_MODIFIER_THREE_LINES * (game.level+1);
			break;
		case 4:
			game.score += SCORE_MODIFIER_FOUR_LINES * (game.level+1);
			break;
	}
	
	//calculate level here
	game.level = game.lines/10;
	if (game.level > MAX_LEVEL)
		game.level = MAX_LEVEL;
	
	//full line(s) exist, flash them then remove, then drop above blocks down
	if ( count > 0 )
	{
		s_playSound(lineClear);
		
		debug_msg("[g_onDownBlocked]: start flashing\n");
		//k = index for a row to flash
		//i = current x position
		//j = y position (row). fullRows[k]
		
		//flash1 = g_loadBlockImage(block_getType(b));
		flash1 = g_loadImage("./pictures/BLOCK_FLASH1.bmp");
		flash2 = g_loadImage("./pictures/BLOCK_FLASH2.bmp");
		
		for ( flashCount = 0; flashCount < FLASH_FRAMES; flashCount++ )
		{
			for ( k = 0; k < count; k++ )
			{
				for ( i = 0; i < SIZE_X; i++ )
				{
					b = g_getBlockAtPos(i, fullRows[k]);
					
					g_getImageCoords(i, fullRows[k], &x, &y);
					if ( flashCount % 2 == 1 )
						g_addSurface(x, y, flash1, screen, NULL);
					else
						g_addSurface(x, y, flash2, screen, NULL);
					
					//g_addSurface(x, y, blockFlash, screen, NULL);
				}
			}
		
			SDL_Flip(screen);
			SDL_Delay(FLASH_INTERVAL);
		}
		SDL_FreeSurface(flash1);
		SDL_FreeSurface(flash2);
		
		debug_msg("[g_onDownBlocked]: finished flashing\n");
		
		//remove all those blocks
		for ( k = 0; k < count; k++ )
		{
			for ( i = 0; i < SIZE_X; i++ )
			{
				b = g_getBlockAtPos(i, fullRows[k]);
				g_removeBlockFromPos(b);
				if ( block_clear(b) )
				{
					//printf("[g_onDownBlocked]: block at (%d, %d) removed\n", i, fullRows[k]);
				}
				else
				{
					//printf("[g_onDownBlocked]: no block removed at (%d, %d)\n", i, fullRows[k]);
				}
			}
		}
		
		//move downt he above blocks
		g_dropAboveBlocksDown((fullRows[0])+1, count);
	}
	else
		s_playSound(downBlock); //no line clear so just play stacking sound
	
	
	
	game.current = game.next;
	game.next = NULL;
	game.next = tetro_create(TETRO_RANDOM);
	if ( !tetro_moveToStart(game.current) )
	{
		//printf("[g_onDownBlocked]: GAME OVER!\n");
		//g_clear(REASON_LOSS);
		game.state = STATE_LOSS;
		
		//on frame -1 do nothing, frame 0 pauses briefly so you can see the last tetromino that couldnt get in,
		//then the screen fills up rapidly
		game.lossFrame = -1;
		game.lossCurrentRow = 0;
		
		s_playSound(gameOver);
	}
}

boolean g_checkFullRow(int row)
{
	int i;
	
	for ( i = 0; i < SIZE_X; i++ )
	{
		//printf("[g_checkFullRow]: checking row %d, column %d\n", row, i);
		if ( g_getBlockAtPos(i, row) == NULL )
			return false;
	}
	
	//row is full, increment counter for every row above this one
	for ( i = row+1; i < SIZE_Y; i++ )
		(numRowsFilledBelow[i])++;
		
	return true;
}

void g_dropAboveBlocksDown(int startRow, int count)
{
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
				for ( i = 0; i < numRowsFilledBelow[currentRow]; i++ )
				{
					if ( block_move(b, DIR_SOUTH) )
					{
						//printf("[g_dropAboveBlocksDown]: block at (%d, %d) moved down!\n", j, currentRow);
					}
					else
					{
						//printf("[g_dropAboveBlocksDown]: block at (%d, %d) could not move down!\n", j, currentRow);
					}
				}
			}
		}
	}
	//reset array
	for ( i = 0; i < SIZE_Y; i++ )
		numRowsFilledBelow[i] = 0;
}

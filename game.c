#include "game.h"


#include "SDL/SDL.h" //needed for all SDL apps
#include <stdio.h>
#include <stdlib.h> //for exit()

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIT_DEPTH 8


SDL_Surface * screen; //the screen surface

gameModule game; //game module, stores levle, 2d grid and what it contains, etc



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
	debug_msg("Unloading SDL...\n");
	SDL_Quit();
}

void g_create()
{

}

boolean g_boot()
{
	//init screen surface
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) //init basics (event handling, filei/o and threading) + video + timer (for constant update rate)
	{
		printf("Error initializing SDL. Reason: %s\n", SDL_GetError()); // <1 = fail
		return false;
	}

	//set video mode
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_SWSURFACE);
	if (screen == NULL) //fail
	{
		fprintf(stderr, "Error setting video mode to %dx%d, %d bit depth: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_GetError());
		return false;
	}

	//draw a bmp on the screen
	draw_bmp("splash.bmp");
	
	SDL_Delay(5000); //wait, to see image for a bit
}

boolean draw_bmp(char * filename)
{
	SDL_Surface * image;

	image = SDL_LoadBMP(filename);
	if (image == NULL) //fail
	{
		fprintf(stderr, "Error loading image \"%s\"\n", filename);
		return false;
	}

	//if the image has a palette then use that to make colors better
	if (image->format->palette && screen->format->palette)
	{
		SDL_SetColors(screen, image->format->palette->colors, 0, image->format->palette->ncolors);
	}

	//blit image onto screen surface
	if (SDL_BlitSurface(image, NULL, screen, NULL) < 0 ) //<0 = fail
		fprintf(stderr, "Image failed to blit: %s\n", SDL_GetError());

	//updated modified part of screen
	SDL_UpdateRect(screen, 0, 0, image->w, image->h);

	//free space allocated to bmp (its already on screen i guess)
	SDL_FreeSurface(image);
	return true;
}

void debug_msg(char * msg)
{
	if (DEBUG_MODE)
		printf("%s", msg);
}

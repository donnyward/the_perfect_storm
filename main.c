#include "game.h"
#include "types.h"
#include "SDL/SDL.h"


SDL_Surface * screen;

int main(int argc, char * argv[])
{
	printf("Loading SDL...\n");
	//init screen surface
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) //init basics (event handling, filei/o and threading) + video + timer (for constant update rate)
	{
		printf("Error initializing SDL. Reason: %s\n", SDL_GetError()); // <1 = fail
		return false;
	}

	printf("Setting Video mode to %dx%d, %d bit depth...\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH);
	//set video mode
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_SWSURFACE);
	if (screen == NULL) //fail
	{
		printf("Error setting video mode to %dx%d, %d bit depth: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_GetError());
		return false;
	}

	//draw a bmp on the screen
	draw_bmp("splash.bmp");
	
	//set some initial values for the gameModule
	g_init();
	
	SDL_Delay(5000); //wait, to see image for a bit



	g_loop(); //never returns
	return 0;
}



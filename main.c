#include "game.h"
#include "types.h"
#include "SDL/SDL.h"


SDL_Surface * screen;

int main(int argc, char * argv[])
{
	SDL_Surface * splash;
	
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
	
	SDL_WM_SetCaption("The Perfect Storm", "PS"); //sets the title of the window. 1st string is the window title, 2nd is taskbar title

	//display splash screen
	splash = SDL_LoadBMP("./pictures/splash.bmp");
	if (splash == NULL)
		printf("[main]: No splash image found!\n");
	else
	{
		if ( SDL_BlitSurface(splash, NULL, screen, NULL) < 0 ) //< 0 == fail
			printf("[main]: Splash failed to Blit!\n");
			
		SDL_Flip(screen); //updates entire screen
		SDL_FreeSurface(splash); //clear splash from memory, its on screen now
	}
	
	//set some initial values for the gameModule
	g_init();
	
	SDL_Delay(2000); //wait, to see image for a bit


	m_init(); //setup menu
	g_loop(); //never returns
	return 0;
}



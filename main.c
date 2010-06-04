#include "SDL.h" //needed for all SDL apps
#include <stdio.h>
#include <stdlib.h> //for exit()

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BIT_DEPTH 8

int debugMode = 0;

SDL_Surface * screen; //the screen surface

void debug_msg(char msg[]);
int draw_bmp(char * filename);

int main(int argc, char * argv[])
{


	//init screen surface
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 1 ) //init basics (event handling, filei/o and threading) + video + timer (for constant update rate)
	{
		printf("Error initializing SDL. Reason: %s\n", SDL_GetError()); // <1 = fail
		exit(-1);
	}

	//set video mode
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_SWSURFACE);
	if (screen == NULL) //fail
	{
		fprintf(stderr, "Error setting video mode to %dx%d, %d bit depth: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_GetError());
		exit(-1);
	}

	//draw a bmp on the screen
	draw_bmp("splash.bmp");




	SDL_Delay(3000); //wait, to see image for a bit

	printf("Exiting program...");
//	getch();
	SDL_Quit(); //unload sdl library
	return 0;
}

int draw_bmp(char * filename)
{
	SDL_Surface * image;

	image = SDL_LoadBMP(filename);
	if (image == NULL) //fail
	{
		fprintf(stderr, "Error loading image \"%s\"\n", filename);
		return 0;
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
	return 1;
}

void debug_msg(char msg[])
{


}

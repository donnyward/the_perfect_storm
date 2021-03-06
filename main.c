#include "game.h"
#include "types.h"
#include "SDL/SDL.h"

SDL_Surface * screen;
highScoresStruct_t highScores;
char * highScoresNameArray[] = 
{
	highScores.name0,
	highScores.name1,
	highScores.name2,
	highScores.name3,
	highScores.name4,
	highScores.name5,
	highScores.name6,
	highScores.name7,
	highScores.name8,
	highScores.name9
};

int debug_mode = 0;



void checkHighScores();

int main(int argc, char * argv[])
{
	SDL_Surface * splash;
	int i;
	boolean isFullScreen = false;
	
	for ( i = 1; i < argc; i++ )
	{
		if ( !strcmp(argv[i], "-fullscreen") )
			isFullScreen = true;
		else if ( !strcmp(argv[i], "-dev") )
			debug_mode = 1;
		else
			printf("[main]: unrecognized parameter '%s'\n", argv[i]);
	}


	printf("Loading SDL...\n");
	//init basics (event handling, filei/o and threading) + video + timer (for constant update rate) + audio
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) //SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO
	{
		printf("Error initializing SDL. Reason: %s\n", SDL_GetError()); // <1 = fail
		return false;
	}

	printf("Setting Video mode to %dx%d, %d bit depth...\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH);
	//set video mode
	/* from sdl documentation (on SDL_SetVideoMode):
	The returned surface is freed by SDL_Quit and must not be freed by the caller.
	This rule also includes consecutive calls to SDL_!SetVideoMode 
	(i.e. resize or resolution change) because the existing surface will be released automatically. 
	Whatever flags SDL_!SetVideoMode  could satisfy are set in the flags member of the returned surface.
	*/
	if (isFullScreen)
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_SWSURFACE | SDL_FULLSCREEN);
	
	else
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_SWSURFACE);
		
	SDL_ShowCursor( SDL_DISABLE );
	if (screen == NULL) //fail
	{
		printf("Error setting video mode to %dx%d, %d bit depth: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH, SDL_GetError());
		return false;
	}
	
	if ( !s_init() )
	{
		debug_msg("[main]: s_init failed\n");
		return false;
	}
	
	SDL_WM_SetCaption("The Perfect Storm", "PS"); //sets the title of the window. 1st string is the window title, 2nd is taskbar title

	//display splash screen
	splash = g_loadImage("./pictures/splash.bmp");//SDL_LoadBMP("./pictures/splash.bmp");
	
	if (splash == NULL)
		printf("[main]: No splash image found!\n");
	else
	{
		if ( !g_addSurface(0, 0, splash, screen, NULL) )
			printf("[main]: Splash failed to Blit!\n");
			
		SDL_Flip(screen); //updates entire screen
		SDL_FreeSurface(splash); //clear splash from memory, its on screen now
	}
	
	//set some initial values for the gameModule
	g_init();
	
	//checks for high scores list and creates default one if none found
	checkHighScores();
	
	SDL_Delay(2000); //wait, to see image for a bit


	m_init(); //setup menu
	g_loop(); //never returns
	return 0;
}

void checkHighScores()
{
	highScoresStruct_t data;
	FILE * f;
	
	f = fopen("hs.dat", "rb");
	
	if (f != NULL)
	{
		debug_msg("[checkHighScores]: hs.dat already exists!\n");
		
		fread(&highScores, sizeof(highScoresStruct_t), 1, f);

	}
	else
	{
		debug_msg("[checkHighScores]: creating new hs.dat... \n");
		f = fopen("hs.dat", "wb");
		
		strcpy(data.name0, "KKD");
		strcpy(data.name1, "JFK");
		strcpy(data.name2, "ERL");
		strcpy(data.name3, "BEN");
		strcpy(data.name4, "JON");
		strcpy(data.name5, "AJW");
		strcpy(data.name6, "TIM");
		strcpy(data.name7, "PAL");
		strcpy(data.name8, "MIK");
		strcpy(data.name9, "KIM");
	
		data.scores[0] = HIGH_SCORES_0;
		data.scores[1] = HIGH_SCORES_1;
		data.scores[2] = HIGH_SCORES_2;
		data.scores[3] = HIGH_SCORES_3;
		data.scores[4] = HIGH_SCORES_4;
		data.scores[5] = HIGH_SCORES_5;
		data.scores[6] = HIGH_SCORES_6;
		data.scores[7] = HIGH_SCORES_7;
		data.scores[8] = HIGH_SCORES_8;
		data.scores[9] = HIGH_SCORES_9;
	
		fwrite(&data, sizeof(highScoresStruct_t), 1, f);
		
		highScores = data;
	}
	
	fclose(f);
}

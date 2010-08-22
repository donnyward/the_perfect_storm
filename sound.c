#include "types.h"
#include "SDL/SDL_mixer.h"

//Mix_Music * music = NULL;
Mix_Chunk * downBlock = NULL;
Mix_Chunk * gameOver = NULL;
Mix_Chunk * lineClear = NULL;
Mix_Chunk * menuMove = NULL;
Mix_Chunk * menuSelect = NULL;
Mix_Chunk * moveSideways = NULL;
Mix_Chunk * rotate = NULL;

/*
Mix_Chunk * sound_effects[] = 
{
	menuMove,
	menuSelect,
	rotate,
	moveSideways,
	downBlock,
	lineClear,
	gameOver,
	NULL
};
*/

boolean s_init()
{
	if ( Mix_OpenAudio(FREQUENCY, MIX_DEFAULT_FORMAT, 2, SAMPLE_SIZE) == -1 )
	{
		printf("Mix_OpenAudio failed\n");
		return false;
	}
	
	//music = Mix_LoadMusic("./sounds/music.wav");
	downBlock = Mix_LoadWAV("./sounds/downblocked.wav");
	gameOver = Mix_LoadWAV("./sounds/game_over.wav");
	lineClear = Mix_LoadWAV("./sounds/lineclear.wav");
	menuMove = Mix_LoadWAV("./sounds/menu_move.wav");
	menuSelect = Mix_LoadWAV("./sounds/menu_select.wav");
	moveSideways = Mix_LoadWAV("./sounds/move_sideways.wav");
	rotate = Mix_LoadWAV("./sounds/rotate.wav");
/*
	if (downBlock == NULL || gameOver == NULL || lineClear == NULL || menuMove == NULL || menuSelect == NULL || moveSideways == NULL || rotate == NULL)
	{
		printf("A Mix_LoadWAV failed\n");
		return false;
	}
*/
	if (downBlock == NULL)
		printf("downBlock fail\n");
	else if (gameOver == NULL)
		printf("gameOver fail\n");
	else if (lineClear == NULL)
		printf("lineClear fail\n");
	else if (menuMove == NULL)
		printf("menuMove fail\n");
	else if (menuSelect == NULL)
		printf("menuSelect fail\n");
	else if (moveSideways == NULL)
		printf("moveSideways fail\n");
	else if (rotate == NULL)
		printf("rotate fail\n");
//	menuSelect = Mix_LoadWAV("./sounds/rotate.wav");
	//if (menuSelect == NULL)
//		return false;
		
	return true;
}

boolean s_playSound(Mix_Chunk * s)
{
	if (s == NULL)
		return true;
		
	if ( Mix_PlayChannel(-1, s, 0) == -1 )
	{
		debug_msg("[s_playSound]: failure!\n");
		return false;
	}
	else
	{
		debug_msg("[s_playSound]: success!\n");
		return true;
	}
}

void s_clean()
{
	//Mix_FreeMusic(music);
	Mix_FreeChunk(downBlock);
	Mix_FreeChunk(gameOver);
	Mix_FreeChunk(lineClear);
	Mix_FreeChunk(menuMove);
	Mix_FreeChunk(menuSelect);
	Mix_FreeChunk(moveSideways);
	Mix_FreeChunk(rotate);
	
	Mix_CloseAudio();
}

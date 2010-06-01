all :
	gcc main.c -o ps `sdl-config --cflags --libs`
#	gcc main.c -o ps -lSDL -lSDL_mixer
#	

CC = gcc
CFLAGS =
LIBS = `sdl-config --cflags --libs`
#SOURCES = main.c game.c
OBJECTS =			\
		main.o		\
		game.o		\
		tetromino.o	\
		menu.o
EXECUTABLE = ps

#all : $(OBJECTS)
#	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)
	
game : $(OBJECTS)
	gcc $(OBJECTS) `sdl-config --cflags --libs` -o ps 
	
main.o : main.c
	gcc -c main.c
	
game.o : game.c
	gcc -c game.c
	
block.o : block.c
	gcc -c block.c
	
tetromino.o : tetromino.c
	gcc -c tetromino.c
	
menu.o : menu.c
	gcc -c menu.c
	
clean :
	rm ps $(OBJECTS)

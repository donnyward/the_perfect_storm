CC = gcc
CFLAGS =
LIBS = `sdl-config --cflags --libs`
#SOURCES = main.c game.c
OBJECTS =			\
		main.o		\
		game.o		\
		block.o
EXECUTABLE = ps

#all : $(OBJECTS)
#	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LIBS)
	
game : main.o game.o block.o
	gcc main.o game.o block.o -o `sdl-config --cflags --libs` ps 
	
main.o : main.c
	gcc -c main.c
	
game.o : game.c
	gcc -c game.c
	
block.o : block.c
	gcc -c block.c
	
clean :
	rm ps $(OBJECTS)

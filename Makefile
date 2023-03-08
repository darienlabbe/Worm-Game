all: game 
CPP=g++
CFLAGS=-std=c++11

game: main.o Worm.o Screen.o Cell.o
	$(CPP) -g -ggdb -o game main.o Worm.o Screen.o Cell.o -lcurses -ltermcap

main.o: main.cpp Worm.hpp Screen.hpp Cell.hpp
	$(CPP) -g -c -ggdb main.cpp $(CFLAGS)

Screen.o: Screen.cpp Screen.hpp Cell.hpp
	$(CPP) -g -c -ggdb Screen.cpp $(CFLAGS)

Worm.o: Worm.cpp Worm.hpp Cell.hpp
	$(CPP) -g -c -ggdb Worm.cpp $(CFLAGS)

Cell.o: Cell.cpp Cell.hpp
	gcc -g -c -ggdb Cell.cpp $(CFLAGS)

clean:
	rm -f *.o *.gch *~ game

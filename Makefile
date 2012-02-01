LIBS = -lSDL -lSDL_image

all: srpg

srpg: SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o
	g++ -o srpg SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o ${LIBS}

SRPG.o:
	g++ -c SRPG.cpp

Util.o:
	g++ -c Util.cpp

Tile.o:
	g++ -c Tile.cpp

Character.o:
	g++ -c Character.cpp

Warrior.o:
	g++ -c Warrior.cpp

Archer.o:
	g++ -c Archer.cpp

Healer.o:
	g++ -c Healer.cpp

clean:
	rm -f srpg *.o

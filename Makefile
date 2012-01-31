LIBS = -lSDL -lSDL_image

all: srpg

srpg: SRPG.o Util.o Tile.o
	g++ -o srpg SRPG.o Util.o Tile.o ${LIBS}

SRPG.o: Util.h Tile.h
	g++ -c SRPG.cpp

Util.o: Util.h
	g++ -c Util.cpp

Tile.o: Tile.h
	g++ -c Tile.cpp

clean:
	rm -f srpg *.o

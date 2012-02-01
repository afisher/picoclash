LIBS = -lSDL -lSDL_image

all: srpg

srpg: SRPG.o Util.o Tile.o
	g++ -o srpg SRPG.o Util.o Tile.o ${LIBS}

SRPG.o:
	g++ -c SRPG.cpp

Util.o:
	g++ -c Util.cpp

Tile.o:
	g++ -c Tile.cpp

clean:
	rm -f srpg *.o

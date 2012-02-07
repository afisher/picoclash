LIBS = -lSDL -lSDL_image

all: srpg

srpg: SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o PlayerWarrior.o PlayerArcher.o PlayerHealer.o EnemyWarrior.o EnemyArcher.o EnemyHealer.o
	g++ -o srpg SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o PlayerWarrior.o PlayerArcher.o PlayerHealer.o EnemyWarrior.o EnemyArcher.o EnemyHealer.o ${LIBS}

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

PlayerWarrior.o:
	g++ -c PlayerWarrior.cpp

PlayerArcher.o:
	g++ -c PlayerArcher.cpp

PlayerHealer.o:
	g++ -c PlayerHealer.cpp

EnemyWarrior.o:
	g++ -c EnemyWarrior.cpp

EnemyArcher.o:
	g++ -c EnemyArcher.cpp

EnemyHealer.o:
	g++ -c EnemyHealer.cpp

clean:
	rm -f srpg *.o

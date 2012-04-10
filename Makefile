LIBS = -lSDL -lSDL_image -lSDL_ttf

all: srpg

srpg: SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o Grid.o IdleState.o SelectedState.o MovingState.o AttackingState.o HealingState.o StateMachine.o
	g++ -o srpg SRPG.o Util.o Tile.o Character.o Warrior.o Archer.o Healer.o Grid.o IdleState.o SelectedState.o MovingState.o AttackingState.o HealingState.o StateMachine.o ${LIBS}

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

Grid.o:
	g++ -c Grid.cpp

IdleState.o:
	g++ -c IdleState.cpp
	
SelectedState.o:
	g++ -c SelectedState.cpp
	
MovingState.o:
	g++ -c MovingState.cpp
	
AttackingState.o:
	g++ -c AttackingState.cpp
	
HealingState.o:
	g++ -c HealingState.cpp
	
StateMachine.o:
	g++ -c StateMachine.cpp

clean:
	rm -f srpg *.o

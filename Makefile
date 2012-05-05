#CXXFLAGS = -Wall
CC = g++
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lm
OBJS = \
	Picoclash.o \
	Util.o \
	Tile.o \
	RockTile.o \
	Character.o \
	Warrior.o \
	Archer.o \
	Healer.o \
	Grid.o \
	IdleState.o \
	MovingState.o \
	SelectedState.o \
	AttackingState.o \
	HealingState.o \
	StateMachine.o \
	MapSelector.o \
	MapButton.o \
	CheckButton.o

all: Picoclash

Picoclash: $(OBJS)

clean:
	rm -f SRPG *.o

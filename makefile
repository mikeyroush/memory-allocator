# makefile

all: memtest

BuddyAllocator.o : BuddyAllocator.cpp
	g++ -c -g BuddyAllocator.cpp

Main.o : Main.cpp
	g++ -c -g Main.cpp

memtest: Main.o BuddyAllocator.o
	g++ -o memtest Main.o BuddyAllocator.o

clean:
	rm *.o
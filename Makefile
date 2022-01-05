# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# Compile with -O3 flag if type=opt is passed in command line, else -g flag.
ifeq ($(type),opt)
	CPPFLAGS += -O3
	LDFLAGS += -O3
else
	CPPFLAGS += -g
	LDFLAGS += -g
endif

all: pathfinder actorconnections

pathfinder: pathfinder.cpp ActorGraph.o Actor.o Movie.o

actorconnections: actorconnections.cpp ActorGraph.o Actor.o Movie.o

ActorGraph.o: ActorGraph.h Actor.o Movie.o

Movie.o: Movie.h Actor.o

Actor.o: Actor.h

clean:
	rm -f *.o core*

CC=g++
CFLAGS = -g -Wall
INCLUDES = game.h othello.h colors.h piece.h
IMPLEM = game.cc main.cc othello.cc
OBJS = main.o game.o othello.o
EXEC = game

all: build archive

build: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<
clean: 
	-rm *.o
	-rm core
	-rm *.core
	-rm $(EXEC)
archive:
	tar -cvf $(EXEC).tgz $(INCLUDES) $(IMPLEM) makefile
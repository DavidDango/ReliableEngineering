CC=gcc 
CFLAGS=-Wall -g -O3

all: finder
program: finder.o
program.o: finder.c

clean: rm -f finder finder.o

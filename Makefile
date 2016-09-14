CC=gcc
CFLAGS=-Wall -std=c11

all: main

main: main.o mandlebrot.o bitmap.o
	$(CC) main.o mandlebrot.o bitmap.o -o main -lm

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

mandlebrot.o: mandlebrot.c
	$(CC) $(CFLAGS) -c mandlebrot.c

bitmap.o: bitmap.c
	$(CC) $(CFLAGS) -c bitmap.c
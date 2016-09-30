CC=gcc
CCX=g++-5
CFLAGS=-Wall -std=c11
CCXFLAGS=-Wall -std=c++14

all: main

main: main.o mandlebrot.o bitmap.o
	$(CCX) main.o mandlebrot.o bitmap.o -o main_cpp

main.o: main.cpp
	$(CCX) $(CCXFLAGS) -c main.cpp

mandlebrot.o: mandlebrot.cpp
	$(CCX) $(CCXFLAGS) -c mandlebrot.cpp

bitmap.o: bitmap.cpp
	$(CCX) $(CCXFLAGS) -c bitmap.cpp
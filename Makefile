CC=gcc
CCX=g++-5
CFLAGS=-Wall -std=c11
CCXFLAGS=-Wall -std=c++14

all: main

main: main.o mandlebrot.o bitmap.o parseinput.o
	$(CCX) main.o mandlebrot.o bitmap.o parseinput.o -o main_cpp

main.o: main.cpp
	$(CCX) $(CCXFLAGS) -c main.cpp

mandlebrot.o: mandlebrot.cpp
	$(CCX) $(CCXFLAGS) -c mandlebrot.cpp

parseinput.o: parseinput.cpp
	$(CCX) $(CCXFLAGS) -c parseinput.cpp

bitmap.o: bitmap.cpp
	$(CCX) $(CCXFLAGS) -c bitmap.cpp
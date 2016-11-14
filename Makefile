CC=gcc
CCX=g++-6
CFLAGS=-Wall -std=c11
CCXFLAGS=-Wall -Wextra -std=c++14 -mavx -O2

objects = main.o mandlebrot.o bitmap.o parseinput.o color_strategy.o 

all: main

main: $(objects)
	$(CCX) $(objects) -o main

main.o: main.cpp
	$(CCX) $(CCXFLAGS) -c main.cpp

mandlebrot.o: mandlebrot.cpp
	$(CCX) $(CCXFLAGS) -c mandlebrot.cpp

parseinput.o: parseinput.cpp
	$(CCX) $(CCXFLAGS) -c parseinput.cpp

bitmap.o: bitmap.cpp
	$(CCX) $(CCXFLAGS) -c bitmap.cpp

color_strategy.o: color_strategy.cpp
	$(CCX) $(CCXFLAGS) -c color_strategy.cpp

clean:
	rm main $(objects)
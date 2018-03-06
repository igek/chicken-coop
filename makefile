CC = g++
CFLAGS=-Wall -Wextra -std=c++17
SRC = 

farm: farm.cpp chicken.o egg.o coop.o incubator.o
	$(CC) $(CFLAGS) -o farm chicken.o egg.o coop.o incubator.o farm.cpp

debug: farm.cpp chicken.o egg.o coop.o incubator.o
	$(CC) $(CFLAGS) -g -o farm chicken.o egg.o coop.o incubator.o farm.cpp

chicken.o: chicken.cpp
	$(CC) $(CFLAGS) -c chicken.cpp

egg.o: egg.cpp
	$(CC) $(CFLAGS) -c egg.cpp

coop.o: coop.cpp
	$(CC) $(CFLAGS) -c coop.cpp

incubator.o: incubator.cpp
	$(CC) $(CFLAGS) -c incubator.cpp

clean:
	rm -f core *.o

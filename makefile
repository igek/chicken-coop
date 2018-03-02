CC = g++
CFLAGS=-Wall -Wextra -pthread -std=c++17

coop: coop.cpp
	$(CC) -o coop.o $(CFLAGS) coop.cpp

debug: coop.cpp
	$(CC) $(CFLAGS) -gdwarf-3 coop.cpp

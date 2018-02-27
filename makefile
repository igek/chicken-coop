CC = g++
CFLAGS=-Wall -Wextra -pthread -std=c++17

makecoop: coop.cpp
	$(CC) -o coop.o $(CFLAGS) coop.cpp

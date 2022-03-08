#
# Makefile for CSF Assignment 3

CC = gcc

CFLAGS = -g -Wall -Wextra -pedantic -std=c++11


main : main.o
	$(CFLAGS) -o main main.o

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean :
	rm -f main *.o csim

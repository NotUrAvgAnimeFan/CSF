#
# Makefile for CSF Assignment 3

CXX = g++

CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++11


csim : main.o
	$(CXX) -o csim main.o

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

clean :
	rm -f main *.o csim

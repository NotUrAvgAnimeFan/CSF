# CSF Assignment 3 Milestone 2
# Ricardo Morales Gonzalez rmorale5@jhu.edu
# Ana Kur  akuri1@jhu.edu
# Makefile for CSF Assignment 3

CXX = g++

CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++11


csim : main.o
	$(CXX) -o csim main.o

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

assign3.zip :
	rm -f assign3.zip
	zip -9r $@  Makefile README.txt main.cpp

clean :
	rm -f main *.o csim

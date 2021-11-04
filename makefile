CC = gcc

CFLAGS = -g -std=c++11 -lstdc++
 
all: scanner

scanner: main.cpp token.hpp scanner.hpp scanner.cpp
	$(CC) -o $@ $^ $(CFLAGS) 
 
clean:
	rm scanner 
	rm -r *.dSYM
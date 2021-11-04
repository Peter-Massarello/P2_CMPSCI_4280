CC = gcc

CFLAGS = -g -std=c++11 -lstdc++
 
all: parser

parser: main.cpp token.hpp scanner.hpp scanner.cpp parser.hpp parser.cpp
	$(CC) -o $@ $^ $(CFLAGS) 
 
clean:
	rm parser 
	rm -r *.dSYM
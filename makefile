# see readme.txt for information
# requires Steg.cpp Steg.h main.cpp

CC=g++
#CC=clang++
OP=-std=c++11 -stdlib=libc++

all: steg.e

steg.e: Steg.o main.o
	$(CC) $(OP) Steg.o main.o -o steg

main.o: main.cpp
	$(CC) $(OP) -c main.cpp

Steg.o: Steg.cpp Steg.h
	$(CC) $(OP) -c Steg.cpp

clean:
	rm steg *.o
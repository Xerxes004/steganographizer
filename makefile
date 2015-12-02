CC=g++ -std=c++11 -stdlib=libc++
#CC=clang++ -std=c++11 -stdlib=libc++

all: steg.e

steg.e: Steg.o main.o
	$(CC) Steg.o main.o -o steg

main.o: main.cpp
	$(CC) -c main.cpp

Steg.o: Steg.cpp Steg.h
	$(CC) -c Steg.cpp

clean:
	rm steg *.o
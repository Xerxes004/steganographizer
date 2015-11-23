CC=g++ -std=c++11 -stdlib=libc++
#CC=clang++ -std=c++11 -stdlib=libc++

all: steg.e

steg.e: Steganographizer.o main.o
	$(CC) Steganographizer.o main.o -o steg.e

main.o: main.cpp
	$(CC) -c main.cpp

Steganographizer.o: Steganographizer.cpp
	$(CC) -c Steganographizer.cpp

clean:
	rm *o steg.e
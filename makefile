CC=g++ -std=c++11 -stdlib=libc++
#CC=clang++ -std=c++11 -stdlib=libc++

all: steg

steg: Steganographizer.o main.o
	$(CC) Steganographizer.o main.o -o steg

main.o: main.cpp
	$(CC) -c main.cpp

Steganographizer.o: Steganographizer.cpp
	$(CC) -c Steganographizer.cpp

clean:
	rm *o steg
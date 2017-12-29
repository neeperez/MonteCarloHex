CFLAGS = -std=c++11 -g
CC = g++

main: main.o Graph.o 
	$(CC) $(CFLAGS) -o main main.o Graph.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm -f main main.o Graph.o

FLAGS=-std=c++11 -Wall -Wextra
CC=g++

gol: gol.cpp
	$(CC) $(FLAGS) gol.cpp -o gol

clean:
	rm gol

CFLAGS = -g -Wall -Wextra -Werror
CC = gcc

all:
	$(CC) -o chip8 main.c $(CFLAGS) `sdl2-config --cflags --libs`

main: main.o funcs.o
	$(CC) -o main.o funcs.o $(CCFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

funcs.o: func.c 
	$(CC) -c func.c $(CFLAGS)

CC = gcc

SRC = *.c

Flags = -Wall -O2

DFlags = -Wall -g -fsanitize=address -static-libasan

Out = readBin

ReadBinary:
	$(CC) $(SRC) -o $(Out) $(Flags)

Debug:
	$(CC) $(SRC) -o $(Out) $(DFlags)

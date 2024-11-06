CC = gcc
SRC = src

BUILD_DIR = build
OUTPUT = Compressor

CFLAGS = -O2 -Wall
DEBUGFLAGS = -g -Wall

Compressor: $(SRC)/*
	$(CC) $(SRC)/*.c $(CFLAGS) -o $(BUILD_DIR)/$(OUTPUT)

Debug:
	$(CC) $(SRC)/*.c $(DEBUGFLAGS) -o $(BUILD_DIR)/$(OUTPUT) -D DEBUG

Clean:
	rm $(BUILD_DIR)/$(OUTPUT)
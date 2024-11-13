CC = gcc
SRC = src

BUILD_DIR = build
OUTPUT = Compressor

CFLAGS = -O2 -Wall
DEBUGFLAGS = -g -Wall -fsanitize=address -static-libasan

Compressor: $(SRC)/* | Build
	$(CC) $(SRC)/*.c $(CFLAGS) -o $(BUILD_DIR)/$(OUTPUT)

Debug: Build
	$(CC) $(SRC)/*.c $(DEBUGFLAGS) -o $(BUILD_DIR)/$(OUTPUT) -D DEBUG

Clean:
	rm $(BUILD_DIR)/$(OUTPUT)

All:
	make Compressor
	make -C readBinary

AllDebug:
	make Debug
	make Debug -C readBinary

Build:
	mkdir -p $(BUILD_DIR)
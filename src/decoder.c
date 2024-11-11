#include <stdio.h>

#include "file_handler.h"

void decode(Flags* commandLineArgs) {
   // Open the necessary files
    Files myFiles = openDecoderFiles(commandLineArgs);

    // Read the first byte of the file to determine the number of characters used
    char numCharacters;
    size_t length = fread(&numCharacters, sizeof(char), 1, myFiles.inputFile);
    if(length == 0) {
        fprintf(stderr, "Unable to read from file");
        exit(EXIT_FAILURE);
    }

    // Read numCharacter bits to find the array used to make the huffman tree
    char* characters;        




}

char getNumCharacters(FILE* decodeFile) {

}

char* getCharacterArray(FILE* decodeFile) {
    
}
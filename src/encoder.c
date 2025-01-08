#include <stdio.h>

#include "file_handler.h"
#include "encoder.h"
#include "binary_tree.h"

static PathSeparator encoderPathSeparator;

static int compareFreqArray(const void* element1, const void* element2) {
    FrequencyArray* a = *(FrequencyArray**)element1;
    FrequencyArray* b = *(FrequencyArray**)element2;

    if (a->frequency < b->frequency) return -1;
    if (a->frequency > b->frequency) return 1;
    return 0;
}

void printFrequencyArray(FrequencyArray** array, unsigned char arrayLength) {
    for(unsigned char i = 0; i < arrayLength; i++) {
        printf("%c -> %lu\n", (*array)->character, (__uint64_t)((*array)->frequency));
        array++;
    }
}

void encode(FileIO* files) {
    // Make a frequency map for each character.
    unsigned char arrayLength;
    FrequencyArray** characterFrequencies = frequencyMap(files->inputFile, &arrayLength);

    #ifdef DEBUG
    
    printFrequencyElements(characterFrequencies, arrayLength);
    
    #endif    


    // Write Binary Tree to Output File -> Number of elements in the array, the order of elements
    // writeKeyPattern(myFiles.outputFile, characterFrequencies, (unsigned char)arrayLength);

    // Convert the frequency map to a binary tree
    BinaryTree* huffmanTree = createHuffmanCoding(characterFrequencies, arrayLength);

    // Free characterFrequencies
    freeFrequencyArray(characterFrequencies, arrayLength);

    // Write Binary to Output File using Binary Tree
    convertFile(myFiles.outputFile, huffmanTree, myFiles.inputFile);
}

void writeKeyPattern(FILE* outputFile, FrequencyArray** array, unsigned char arrayLength) {
    fwrite(&arrayLength, sizeof(unsigned char), 1, outputFile);
    for(unsigned char i = 0; i < arrayLength; i++) {
        fwrite(&(array[i]->character), sizeof(char), 1, outputFile);
    }
}

FrequencyArray** frequencyMap(FILE* textFile, unsigned char* frequencyArrayLength) {
    __uint128_t characterMap[128] = {0};  
    __uint128_t characterCount = 0;
    unsigned char uniqueCharacters = 0;

    int character;
    while((character = fgetc(textFile)) != EOF) {
        if(characterMap[character] == 0)
            uniqueCharacters++;
    
        characterMap[character]++;
        characterCount++;
    }

    rewind(textFile);

    if(uniqueCharacters == 0) {
        fprintf(stderr, "Unable to read from file");
        exit(EXIT_FAILURE);
    }

    // We are going to use the null byte to represent the end of the file
    characterMap[0]++;
    uniqueCharacters++;
    size_t arrayLength = uniqueCharacters;
    *frequencyArrayLength = uniqueCharacters;

    FrequencyArray** array = malloc(sizeof(*array) * uniqueCharacters);
    uniqueCharacters--;    
    
    for(char i = 0; i >= 0; i++) {
        if(characterMap[(unsigned char)i] != 0) {
            array[uniqueCharacters] = malloc(sizeof(**array));
            array[uniqueCharacters]->character = i;
            array[uniqueCharacters]->frequency = characterMap[(unsigned char)i];

            uniqueCharacters--;
        }
    }

    qsort(array, arrayLength, sizeof(*array), compareFreqArray);    

    return array;
}

void freeFrequencyArray(FrequencyArray** array, unsigned char arrayLength) {
    FrequencyArray** start = array;
    for(int i = 0; i < arrayLength; i++) {
        free(*array++);
    }

    free(start);
}

void convertFile(FILE* outputFile, BinaryTree* tree, FILE* inputFile) {
    char writeBuffer = 0;
    unsigned char bufferIndex = 7; // 2^7 is the firstIndex on the left 

    // Convert Binary Tree to a map
    TreeMap** map = createTreeMap(tree);

    // Write encoded version of the input file to the output file
    int character;
    while((character = fgetc(inputFile)) != EOF) {
        // Read character
        encoderPathSeparator.combined = map[(unsigned char)character]->combinedPath;
        char pathBits = encoderPathSeparator.pathIndex[0];

        // Find length of encoded version
        char pathLength = encoderPathSeparator.pathIndex[1];
        
        // Add necessary bits to the write buffer
        // Decrement the buffer index
        // If bufferIndex < 0 write current bits to the file and restart index at 7

        // Bits will extend into the next variable;
        char currentIndex = 7;
        for(char i = 0; i < pathLength; i++) {
            if(bufferIndex < 0) {
                fwrite(&writeBuffer, sizeof(char), 1, outputFile);
                writeBuffer = 0;
                bufferIndex = 7;
            }
            writeBuffer |= (((pathBits & (1<<(currentIndex - i))) > 0)>>bufferIndex);
            bufferIndex--; 
        }
    }

    encoderPathSeparator.combined = map[0]->combinedPath;
    char pathBits = encoderPathSeparator.pathIndex[0];
    char pathLength = encoderPathSeparator.pathIndex[1];

    char currentIndex = 7;
    for(char i = 0; i < pathLength; i++) {
        if(bufferIndex < 0) {
            fwrite(&writeBuffer, sizeof(char), 1, outputFile);
            writeBuffer = 0;
            bufferIndex = 7;
        }
        writeBuffer |= (((pathBits & (1<<(currentIndex - i))) > 0)>>bufferIndex);
        bufferIndex--; 
    }

    fwrite(&writeBuffer, sizeof(char), 1, outputFile);
    freeTreeMap(map);
}

void printFrequencyElements(FrequencyArray** array, unsigned char arrayLength) {
    for(unsigned char i = 0; i < arrayLength; i++) {
        printf("%c, ", (*array)->character);
        array++;
    }
    printf("\n");
}
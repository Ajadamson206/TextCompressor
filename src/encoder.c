#include <stdio.h>

#include "file_handler.h"
#include "encoder.h"
#include "binary_tree.h"

static PathSeparator encoderPathSeparator;

static int compareFreqArray(const void* element1, const void* element2) {
    Heap* a = *(Heap**)element1;
    Heap* b = *(Heap**)element2;

    if (a->frequency < b->frequency) return -1;
    if (a->frequency > b->frequency) return 1;
    return 0;
}

void printFrequencyArray(Heap** array, unsigned char arrayLength) {
    for(unsigned char i = 0; i < arrayLength; i++) {
        printf("%c -> %lu\n", (*array)->character, (__uint64_t)((*array)->frequency));
        array++;
    }
}

void encode(FileIO* files) {
    // Make a frequency map for each character.
    unsigned char arrayLength;
    Heap** characterFrequencies = frequencyMap(files->inputFile, &arrayLength);

    #ifdef DEBUG
    
    printFrequencyElements(characterFrequencies, arrayLength);
    
    #endif    


    // Write Binary Tree to Output File -> Number of elements in the array, the order of elements
    // writeKeyPattern(myFiles.outputFile, characterFrequencies, (unsigned char)arrayLength);

    // Convert the frequency map to a binary tree
    BinaryTree* huffmanTree = createHuffmanCoding(characterFrequencies, arrayLength);
    free(characterFrequencies);

    // Write Binary to Output File using Binary Tree
    cronvetFile(files->ouptutFile, huffmanTree, files->inputFile);
}

void writeKeyPattern(FILE* outputFile, Heap** array, unsigned char arrayLength) {
    fwrite(&arrayLength, sizeof(unsigned char), 1, outputFile);
    for(unsigned char i = 0; i < arrayLength; i++) {
        fwrite(&(array[i]->character), sizeof(char), 1, outputFile);
    }
}

/**
 * @brief Create a frequency map to determine how many times a particular character occurs in a file
 * @param textFile (FILE*) File pointer to the file that is going to be opened and read from
 * @param frequencyArrayLength (unsigned char*) Pointer to the variable that is going to hold the array length
 * @return (Heap**) Pointer to the Heap Array which contains both the character and how many 
 * times it occurred.
 */
Heap** frequencyMap(FILE* textFile, unsigned char* frequencyArrayLength) {
    // Get a 128 bit array of each ascii character, and how many times it occurred
    __uint128_t characterMap[128] = {0};  
    unsigned char uniqueCharacters = 0;

    // Read file character at a time and input it into the character map
    int character;
    while((character = fgetc(textFile)) != EOF) {
        if(characterMap[character] == 0)
            uniqueCharacters++;
    
        characterMap[character]++;
    }

    // Start at beginning again
    rewind(textFile);

    // Empty files raise an error
    if(uniqueCharacters == 0) {
        fprintf(stderr, "Unable to read from file");
        exit(EXIT_FAILURE);
    }

    // We are going to use the null byte to represent the end of the file
    characterMap[0]++;
    uniqueCharacters++;
    size_t arrayLength = uniqueCharacters;
    *frequencyArrayLength = uniqueCharacters;

    Heap** array = malloc(sizeof(*array) * uniqueCharacters);
    uniqueCharacters--;    
    
    // Move character map into frequency array
    for(unsigned char i = 0; i < 128; i++) {
        if(characterMap[i] != 0) {
            array[uniqueCharacters] = malloc(sizeof(**array));
            array[uniqueCharacters]->character = (char)i;
            array[uniqueCharacters]->frequency = characterMap[i];
            array[uniqueCharacters]->left = NULL;
            array[uniqueCharacters]->right = NULL;

            uniqueCharacters--;
        }
    }

    return array;
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
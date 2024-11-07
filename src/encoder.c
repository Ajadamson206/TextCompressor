#include <stdio.h>

#include "file_handler.h"
#include "encoder.h"
#include "binary_tree.h"

static int compareFreqArray(const void* element1, const void* element2) {
    FrequencyArray* a = *(FrequencyArray**)element1;
    FrequencyArray* b = *(FrequencyArray**)element2;

    if (a->frequency < b->frequency) return -1;
    if (a->frequency > b->frequency) return 1;
    return 0;
}

void printFrequencyArray(FrequencyArray** array, int arrayLength) {
    for(int i = 0; i < arrayLength; i++) {
        printf("%c -> %lu\n", array[i]->character, (__uint64_t)(array[i]->frequency));
    }
}

void encode(Flags* commandLineArgs) {
    // Open/Create the necessary files
    Files myFiles = openEncoderFiles(commandLineArgs);

    // Make a frequency map for each character.
    int arrayLength;
    FrequencyArray** characterFrequencies = frequencyMap(myFiles.inputFile, &arrayLength);

    // Convert the frequency map to a binary tree
    BinaryTree* huffmanTree = createHuffmanCoding(characterFrequencies, arrayLength);

    // Write Binary Tree to a Key File

    // Write Binary to Output File using Binary Tree

}

FrequencyArray** frequencyMap(FILE* textFile, int* frequencyArrayLength) {
    __uint128_t characterMap[128] = {0};  
    __uint128_t characterCount = 0;
    __uint8_t uniqueCharacters = 0;

    int character;
    while((character = fgetc(textFile)) != EOF) {
        if(characterMap[character] == 0)
            uniqueCharacters++;
    
        characterMap[character]++;
        characterCount++;
    }

    if(uniqueCharacters == 0) {
        fprintf(stderr, "Unable to read from file");
        exit(EXIT_FAILURE);
    }

    // We are going to use the null byte to represent the end of the file
    characterMap[0]++;

    int arrayLength = uniqueCharacters;

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

    *frequencyArrayLength = arrayLength;
    return array;
}
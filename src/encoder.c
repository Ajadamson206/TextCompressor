#include <stdio.h>

#include "file_handler.h"

typedef struct {
    __uint128_t frequency;
    unsigned char character;
} FrequencyArray;

int compareFreqArray(void* element1, void* element2) {
    return ((FrequencyArray*)element1)->frequency > ((FrequencyArray*)element2);
}

void encode(Flags* commandLineArgs) {
    Files myFiles = openEncoderFiles(commandLineArgs);

    // Make a frequency map for each character.
    FrequencyArray** characterFrequencies = frequencyMap(myFiles.inputFile);

}

FrequencyArray** frequencyMap(FILE* textFile) {
    __uint128_t characterMap[256] = {0};  
    __uint128_t characterCount = 0;
    __uint8_t uniqueCharacters = 0;

    int character;
    while((character = fgetc(textFile)) != EOF) {
        if(characterMap[(unsigned char)character] == 0)
            uniqueCharacters++;
    
        characterMap[(unsigned char)character]++;
        characterCount++;
    }

    if(uniqueCharacters == 0) {
        fprintf(stderr, "Unable to read from file");
        exit(EXIT_FAILURE);
    }

    int arrayLength = uniqueCharacters;

    FrequencyArray** array = malloc(sizeof(*array) * uniqueCharacters);
    uniqueCharacters--;    
    
    for(unsigned char i = 0; i <= 255; i++) {
        if(characterMap[i] != 0) {
            array[uniqueCharacters] = malloc(sizeof(**array));
            array[uniqueCharacters]->character = i;
            array[uniqueCharacters]->frequency = characterMap[i];

            uniqueCharacters--;
        }
    }

    qsort(array, arrayLength, sizeof(array), compareFreqArray);    

    return array;
}
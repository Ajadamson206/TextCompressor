#pragma once

#include <stdio.h>

#include "file_handler.h"

typedef struct {
    __uint128_t frequency;
    char character;
} FrequencyArray;

extern void encode(Flags* commandLineArgs);
extern FrequencyArray** frequencyMap(FILE* textFile, unsigned char* frequencyArrayLength);
extern void printFrequencyArray(FrequencyArray** array, unsigned char arrayLength);
extern void freeFrequencyArray(FrequencyArray** array, unsigned char arrayLength);
extern void writeKeyPattern(FILE* outputFile, FrequencyArray** array, unsigned char arrayLength);
extern void printFrequencyElements(FrequencyArray** array, unsigned char arrayLength);


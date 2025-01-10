#pragma once

#include <stdio.h>

#include "file_handler.h"

extern void encode(FileIO* files);
extern Heap** frequencyMap(FILE* textFile, unsigned char* frequencyArrayLength);
extern void writeKeyPattern(FILE* outputFile, FrequencyArray** array, unsigned char arrayLength);
extern void printFrequencyElements(FrequencyArray** array, unsigned char arrayLength);


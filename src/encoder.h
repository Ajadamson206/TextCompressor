#pragma once

#include <stdio.h>

#include "file_handler.h"

typedef struct {
    __uint128_t frequency;
    char character;
} FrequencyArray;

extern void encode(Flags* commandLineArgs);
extern FrequencyArray** frequencyMap(FILE* textFile, int* frequencyArrayLength);
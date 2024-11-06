#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    const char* inputFilePath;
    int isEncoding;
    const char* outputFileName;
    const char* keyFileName; 
} Flags;

typedef struct {
    FILE* inputFile;
    FILE* outputFile;
    FILE* keyFile;
} Files;

extern void checkFile(FILE* file, const char* filename);
extern void checkCreatedFile(FILE* file, const char* filename);
extern Files openEncoderFiles(Flags* commandLineArgs);
extern Files openDecoderFiles(Flags* commandLineArgs);
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
    FILE* ouptutFile;
    FILE* keyFile;
} FileIO;

typedef int Options;

#define ENCODING 1<<0
#define SEPARATE_KEY_BUILD 1<<1
#define MERGE_KEY_BUILD 1<<2
#define THREADED 1<<3

#define IS_ENCODING(x) (x & (1<<0))
#define IS_SEPARATE_KEY_BUILD(x) (x & (1<<1))
#define IS_MERGE_KEY_BUILD(x) (x & (1<<2))
#define IS_THREADED(x) (x & (1<<3))

typedef struct {
    FILE* inputFile;
    FILE* outputFile;
    FILE* keyFile;
} Files;

extern void checkFile(FILE* file, const char* filename);
extern void checkCreatedFile(FILE* file, const char* filename);
extern Files openEncoderFiles(Flags* commandLineArgs);
extern Files openDecoderFiles(Flags* commandLineArgs);
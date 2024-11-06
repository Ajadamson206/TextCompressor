#include <stdlib.h>
#include <stdio.h>

#include "file_handler.h"

/**
 * Encode -> {file}.txt -e -o(Optional) {output filename} -k(Optional) {key filename}
 * 
 * Decode -> {encoded filename} -d -o(Optional) {output filename} -k {key file for decryption} 
 * 
 * Help -> -h
 * 
*/

void checkFile(FILE* file, const char* filename) {
    if(file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
} 

void checkCreatedFile(FILE* file, const char* filename) {
    if(file == NULL) {
        fprintf(stderr, "Unable to create file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
}

Files openEncoderFiles(Flags* commandLineArgs) {
    Files myFiles;

    myFiles.inputFile = fopen(commandLineArgs->inputFilePath, "r");
    checkFile(myFiles.inputFile, commandLineArgs->inputFilePath);

    myFiles.keyFile = fopen(commandLineArgs->keyFileName, "w+");
    checkCreatedFile(myFiles.keyFile, commandLineArgs->keyFileName);

    myFiles.outputFile = fopen(commandLineArgs->outputFileName, "wb+");
    checkCreatedFile(myFiles.outputFile, commandLineArgs->outputFileName);

    return myFiles;
}


Files openDecoderFiles(Flags* commandLineArgs) { 
    Files myFiles;
    
    myFiles.inputFile = fopen(commandLineArgs->inputFilePath, "rb");
    checkFile(myFiles.inputFile, commandLineArgs->inputFilePath);

    myFiles.keyFile = fopen(commandLineArgs->keyFileName, "r");
    checkFile(myFiles.keyFile, commandLineArgs->keyFileName);

    myFiles.outputFile = fopen(commandLineArgs->keyFileName, "w+");
    checkCreatedFile(myFiles.outputFile, commandLineArgs->outputFileName);

    return myFiles;
}
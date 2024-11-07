#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "file_handler.h"
#include "encoder.h"
#include "decoder.h"

/**
 * Encode -> {file}.txt -e -o(Optional) {output filename} -k(Optional) {key filename}
 * 
 * Decode -> {encoded filename} -d -o(Optional) {output filename} -k {key file for decryption} 
 * 
 * Help -> -h
 * 
*/

void printHelp() {
    printf("Usage:\n\nEncoding:\t{inputFile} -e -o(optional) {output filename} -k(optional) \
    {key filename}\nDecoding:\t{inputFile} -d -o(optional) {output filename} -k {key filename}\n \
    \nUse -h to bring this menu up again\n");
}

int main(int argc, char* argv[]) {
    Flags currentFlags = {NULL, -1, NULL, NULL};
    int opt;

    if(argc <= 1 || argc > 7) {
        fprintf(stderr, "Unrecognized Input, use \"-h\" for help\n");
        return 1;
    }

    currentFlags.inputFilePath = argv[1];

    while((opt = getopt(argc, argv, "edo:k:h")) != -1) {
        switch (opt)
        {
        case 'e':
            currentFlags.isEncoding = 1;
            break;
        
        case 'd':
            currentFlags.isEncoding = 0;
            break;

        case 'h':
            printHelp();
            return 0;
        case 'o':
            currentFlags.outputFileName = optarg;
            break;

        case 'k':
            currentFlags.keyFileName = optarg;
            break;

        default:
            fprintf(stderr, "Unrecognized Input, use \"-h\" for help\n");
            return 1;
        }
    }

    if(currentFlags.isEncoding == -1) {
        fprintf(stderr, "Specify -e to encode or -d to decode\n");
        return 1;
    }

    if(currentFlags.isEncoding == 0 && currentFlags.keyFileName == NULL) {
        fprintf(stderr, "Specify the key file to decrypt from\n");
        return 1;
    }

    if(currentFlags.outputFileName == NULL) {
        currentFlags.outputFileName = "Compressed.hc";
    }

    if(currentFlags.keyFileName == NULL) {
        currentFlags.keyFileName = "Compressed.key";
    }

    if(currentFlags.isEncoding == 1) {
        encode(&currentFlags);
    } else {
        decode(&currentFlags);
    }

    return 0;
}
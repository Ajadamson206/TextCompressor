#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

static int verbose;

void printHelp() {
    printf("Usage:\n\nEncoding:\t{inputFile} -e -o(optional) {output filename} -k(optional) \
    {key filename}\nDecoding:\t{inputFile} -d -o(optional) {output filename} -k {key filename}\n \
    \nUse -h to bring this menu up again\n");
}

int main(int argc, char* argv[]) {
    Options flag_options = 0;
    int specified_option = 0;
    int opt;
    
    char* keyfileLocation = NULL;

    if(argc <= 1) {
        printHelp();
        return 1;
    }

    int digit_optind = 0;    

    while(1) {
        int this_option_optind = optind? optind : 1;
        int option_index = 0;

        static struct option long_options[] = {
            {"encode",      no_argument,        0,          'e'},
            {"separate",    no_argument,        0,          's'},
            {"merge",       no_argument,        0,          'm'},
            {"decode",      no_argument,        0,          'd'},
            {"key",         required_argument,  0,          'k'},
            {"help",        no_argument,        0,          'h'},
            {"extract",     no_argument,        0,          'x'},
            {"threads",     required_argument,  0,          't'},
            {"verbose",     no_argument,        &verbose,    1 },
            {0, 0, 0, 0}
        };

        opt = getopt_long(argc, argv, "esmdk:hxt:", long_options, &option_index);

        if(opt == -1) {
            break;
        }

        switch (opt) {
        case 'e':
            if(specified_option) {
                fprintf(stderr, "Specify either -e or -d\n");
                return 1;
            }

            specified_option = 1;
            flag_options |= ENCODING;
            break;
        
        case 's':
            if(specified_option == 0 || (flag_options & ENCODING) == 0) {
                fprintf(stderr, "-e must be specified before the usage of -s\n");
                return 1;
            }

            flag_options |= SEPARATE_KEY_BUILD;
            break;
        
        case 'm':
            if(specified_option == 0 || (flag_options & ENCODING) == 0) {
                fprintf(stderr, "-e must be specified before the usage of -m\n");
                return 1;
            }

            flag_options |= MERGE_KEY_BUILD;
            break;

        case 'd':
            if(specified_option) {
                fprintf(stderr, "Specify either -e or -d\n");
                return 1;
            }

            specified_option = 1;
            break;

        case 'k':
            if(specified_option == 0 || (flag_options & ENCODING) == 1) {
                fprintf(stderr, "-d must be specified before usage of -k\n");
                return 1;
            }

            keyfileLocation = optarg;
            break;

        case 'h':
            printHelp();
            return 0;

        case 'x':
            if(specified_option) {
                fprintf(stderr, "-x usage: compressor -x [file1] [file2] ...");
                return 1;
            }
            // Extract

            break;

        case '?':
            break;

        case 't':


        case '0':
        case '1':
            if(long_options[option_index].flag != 0) {
                break;
            }
            printf(stderr, "Unrecongnized input with case %c\n", opt);
            break;

        default:
            fprintf(stderr, "Unrecognized Input, use \"-h\" for help\n");
            return 1;
        }

    }

    if(!specified_option) {
        fprintf(stderr, "Specify -e to encode or -d to decode\n");
        return 1;
    }

    // Assume that the files have the decoder built into the file
    if(flag_options & ENCODING == 0 && keyfileLocation == NULL) {

        fprintf(stderr, "Specify the key file to decrypt from\n");
        return 1;
    }

    if(optind >= argc) {
        fprintf(stderr, "No files found, place all files at the end\n");
        return 1;
    }

    // Create File Queue
    int numFiles = argc - optind;
    FileIO** fileQueue = malloc(numFiles * sizeof(*fileQueue));
    if(fileQueue == NULL) {
        fprintf(stderr, "Unable to create file queue\n");
        return 1;
    }

    // Open the files
    for(int index = 0; optind < argc; optind++) {
        // Open input file
        fileQueue[index]->inputFile = fopen(argv[optind], IS_ENCODING(flag_options)? "r" : "rb");
        if(fileQueue[index] == NULL) {
            numFiles--;
            fprintf(stderr, "Unable to open file: %s\n", argv[optind]);
            continue;
        } else if (verbose) {
            printf("Successfully opened file %s\n", argv[optind]);
        }

        // Create Output File
        
        // Create File Name
        size_t inputNameLength = strlen(argv[optind]);
        char* outputName = malloc(sizeof(*outputName) * (inputNameLength + 5));
        if(outputName == NULL) {
            fprintf(stderr, "Memory Allocation Error: Unable to create output file name\n");
            return 1;
        }
        strcpy(outputName, argv[optind]);        
        outputName[inputNameLength++] = '.';
        outputName[inputNameLength] = 'h';
        outputName[inputNameLength + 1] = 'c';
        outputName[inputNameLength + 2] = 0;
        
        // Create File
        fileQueue[index]->ouptutFile = fopen(outputName, IS_ENCODING(flag_options)? "wb" : "w");
        if(fileQueue[index]->ouptutFile == NULL) {
            fprintf(stderr, "Unable to create file: %s\n", outputName);
            continue;
        } else if (verbose) {
            printf("Successfully opened/created file %s\n", outputName);
        }

        // Create key file
        if(!IS_ENCODING(flag_options) || !IS_SEPARATE_KEY_BUILD(flag_options)) {
            fileQueue[index]->keyFile = NULL; // Key is built into the output file
            free(outputName);
            continue;
        }

        outputName[inputNameLength] = 'k';
        outputName[inputNameLength + 1] = 'e';
        outputName[inputNameLength + 2] = 'y';
        outputName[inputNameLength + 3] = 0;

        fileQueue[index]->keyFile = fopen(outputName, "wb");
        if(fileQueue[index]->keyFile == NULL) {
            fprintf(stderr, "Unable to create file: %s\n", outputName);
            continue;
        } else if (verbose) {
            printf("Successfully opened/created file %s\n", outputName);
        }

        free(outputName);
        index++;
    }

    // Create thread pool and assign each to a file
    if(numFiles == 0) {
        fprintf(stderr, "Unable to open any file\nExiting...\n");
        return 1;
    }


    for(int i = 0; i < numFiles; i++) {
        if(flag_options & ENCODING == 1) {
            encode(fileQueue[i]);
        } else {
            decode(fileQueue[i]);
        }
    }

    free(fileQueue);
    return 0;
}
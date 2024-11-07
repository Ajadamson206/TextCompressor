#pragma once

#include "encoder.h"

typedef struct {
    char character;
    __uint128_t frequency;

    Heap* left;
    Heap* right;
} Heap;

typedef Heap BinaryTree;

extern Heap* createRoot(char character1, __uint128_t frequency1, char character2, __uint128_t frequency2);
extern Heap* mergeHeaps(Heap* heap1, Heap* heap2);
extern BinaryTree* createHuffmanCoding(FrequencyArray** frequencyMap, int mapLength);
extern void freeTree(BinaryTree* tree);
extern void freeHeap(Heap* heap);



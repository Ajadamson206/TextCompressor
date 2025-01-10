#pragma once

#include "encoder.h"

typedef struct Heap{
    char character;
    __uint128_t frequency;

    struct Heap* left;
    struct Heap* right;
} Heap;

typedef struct TreeMap {
    __uint16_t combinedPath; // Top 8 bits are path, Bottom 8 bits are index
} TreeMap;

typedef union PathSeparator {
    __uint16_t combined;
    char pathIndex[2];
} PathSeparator;

typedef Heap BinaryTree;

extern Heap* mergeHeaps(Heap* heap1, Heap* heap2);
extern BinaryTree* createHuffmanCoding(Heap** frequencyMap, int mapLength);
extern void freeTree(BinaryTree* tree);
extern void freeHeap(Heap* heap);
extern TreeMap** createTreeMap(BinaryTree* root);
extern void treeTraverser(Heap* root, char row, char path, TreeMap** map);
extern void freeTreeMap(TreeMap** map);
extern void convertFile(FILE* outputFile, BinaryTree* tree, FILE* inputFile);
extern int findTreeHeight(BinaryTree* root, int level);




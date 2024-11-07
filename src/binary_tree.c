#include "encoder.h"
#include "binary_tree.h"
#include <stdlib.h>

Heap* createRoot(char character1, __uint128_t frequency1, char character2, __uint128_t frequency2) {
    Heap* root = malloc(sizeof(*root));
    root->character = -1;
    root->frequency = frequency1 + frequency2;

    root->left = calloc(1, sizeof(*root));
    root->right = calloc(1, sizeof(*root));

    root->left->character = character1;
    root->left->frequency = frequency1;  

    root->right->character = character2;
    root->right->frequency = frequency2;  

    return root; 
}

Heap* mergeHeaps(Heap* heap1, Heap* heap2) {
    Heap* merged = malloc(sizeof(*merged));
    merged->character = -1;
    merged->frequency = heap1->frequency + heap2->frequency;
    merged->left = heap1;
    merged->right = heap2;

    return merged;
}

BinaryTree* createHuffmanCoding(FrequencyArray** frequencyMap, int mapLength) {
    // Move the elements of frequency map into a heap
    Heap** myHeap = malloc(sizeof(*myHeap) * ((mapLength + (mapLength & 1)) / 2));    
    int heapIndex = 0;
    for(int i = 0; i < mapLength - 1; i += 2) {
        myHeap[heapIndex] = createRoot(frequencyMap[i]->character, frequencyMap[i]->frequency, 
                               frequencyMap[i+1]->character, frequencyMap[i+1]->frequency);

        heapIndex++;
    }

    // If the heap is of an odd length then the remaining element moves onto the next iteration
    if(mapLength & 1) {
        myHeap[heapIndex] = malloc(sizeof(myHeap[heapIndex]));
        myHeap[heapIndex]->character = frequencyMap[mapLength - 1]->character;
        myHeap[heapIndex]->frequency = frequencyMap[mapLength - 1]->frequency;
        myHeap[heapIndex]->left = NULL; 
        myHeap[heapIndex]->right = NULL;
        heapIndex++;
    }

    // Merge the elements in the heap until only one element remains
    int heapLength = heapIndex;
    while(heapLength != 1) {
        heapIndex = 0;
        for(int i = 0; i < heapLength - 1; i += 2) {
            myHeap[heapIndex] = mergeHeaps(myHeap[i], myHeap[i + 1]);
            heapIndex++;
        }

        // If the heap is of an odd length then the remaining element moves onto the next iteration
        if(heapLength & 1) {
            myHeap[heapIndex] = myHeap[heapLength - 1];
            heapIndex++;
        }


        heapLength = heapIndex;
    }

    BinaryTree* root = (BinaryTree*)myHeap[0];
    free(myHeap);

    return root;
}

void writeTreeToFile(FILE* keyFile, BinaryTree* tree) {
    return;
}

void freeTree(BinaryTree* tree) {
    freeHeap((Heap*)tree);

    free(tree);
}

void freeHeap(Heap* heap) {
    if(heap == NULL)
        return;
    
    freeHeap(heap->left);
    free(heap->left);

    freeHeap(heap->right);
    free(heap->right);
}
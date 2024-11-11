#include "encoder.h"
#include "binary_tree.h"
#include <stdlib.h>

static PathSeparator globPathSeparator;

Heap* createRoot(char character1, char character2) {
    Heap* root = malloc(sizeof(*root));
    root->character = -1;

    root->left = calloc(1, sizeof(*root));
    root->right = calloc(1, sizeof(*root));

    root->left->character = character1;

    root->right->character = character2;

    return root; 
}

Heap* mergeHeaps(Heap* heap1, Heap* heap2) {
    Heap* merged = malloc(sizeof(*merged));
    merged->character = -1;
    merged->left = heap1;
    merged->right = heap2;

    return merged;
}

BinaryTree* createHuffmanCoding(FrequencyArray** frequencyMap, int mapLength) {
    // Move the elements of frequency map into a heap
    Heap** myHeap = malloc(sizeof(*myHeap) * ((mapLength + (mapLength & 1)) / 2));    
    int heapIndex = 0;
    for(int i = 0; i < mapLength - 1; i += 2) {
        myHeap[heapIndex] = createRoot(frequencyMap[i]->character, frequencyMap[i+1]->character);
        heapIndex++;
    }

    // If the heap is of an odd length then the remaining element moves onto the next iteration
    if(mapLength & 1) {
        myHeap[heapIndex] = malloc(sizeof(myHeap[heapIndex]));
        myHeap[heapIndex]->character = frequencyMap[mapLength - 1]->character;
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

TreeMap** createTreeMap(BinaryTree* root) {
    TreeMap** map = calloc(128, sizeof(*map));

    treeTraverser((Heap*)root, 1, 0, map);

    return map;
}

void treeTraverser(Heap* root, char row, char path, TreeMap** map) {
    if(root->character != -1) {
        map[root->character] = malloc(sizeof(*(map[root->character])));
        globPathSeparator.pathIndex[0] = path;
        globPathSeparator.pathIndex[1] = row;

        map[root->character]->combinedPath = globPathSeparator.combined;
        return;
    }

    treeTraverser(root->left, row + 1, path, map);

    treeTraverser(root->right, row + 1, path | (1<<(8-row)), map);
}

void freeTreeMap(TreeMap** map) {
    for(int i = 0; i < 128; i++) {
        free(map[i]);
    }

    free(map);
} 
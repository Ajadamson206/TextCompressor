#include "encoder.h"
#include "binary_tree.h"
#include <stdlib.h>

static PathSeparator globPathSeparator;

Heap* mergeHeaps(Heap* heap1, Heap* heap2) {
    Heap* merged = malloc(sizeof(*merged));
    merged->character = -1;
    merged->frequency = heap1->frequency + heap2->frequency;
    merged->left = heap1;
    merged->right = heap2;

    return merged;
}

int compareFrequencyMapElements(const void* element1, const void* element2) {
    Heap* ele1 = (Heap*)element1;
    Heap* ele2 = (Heap*)element2;

    if(ele1->frequency > ele2->frequency)
        return 1;
    if(ele1->frequency < ele2->frequency)
        return -1;
    return 0;
}

BinaryTree* createHuffmanCoding(Heap** frequencyMap, int mapLength) {
    // Sort the frequency map
    qsort(frequencyMap, mapLength, sizeof(*frequencyMap), compareFrequencyMapElements);

    while(mapLength != 1) {
        // Merge first two elements
        Heap* temp = mergeHeaps(frequencyMap[0], frequencyMap[1]);

        // Insertion Sort with the rest of the map
        int startIndex = 0;
        int rightIndex = 2;
        while(rightIndex < mapLength && frequencyMap[rightIndex]->frequency < temp->frequency) {
            frequencyMap[startIndex] = frequencyMap[rightIndex];
            startIndex++;
            rightIndex++;
        }
        frequencyMap[startIndex++] = temp;
        while(rightIndex < mapLength) {
            frequencyMap[startIndex++] = frequencyMap[rightIndex++];
        }

        // Decrement Map Length
        mapLength--;
    }

    BinaryTree* root = (BinaryTree*)frequencyMap[0];

    return root;
}

void printHeap(Heap** heap, int length) {
    for(int i = 0; i < length; i++) {
        printf("%c, ", heap[i]->character);
    }
    printf("\n");
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

    ((Heap*)root, 1, 0, map);

    return map;
}

void treeTraverser(Heap* root, char row, char path, TreeMap** map) {
    if(root->character != -1) {
        map[(unsigned char)root->character] = malloc(sizeof(*(map[(unsigned char)root->character])));
        globPathSeparator.pathIndex[0] = path;
        globPathSeparator.pathIndex[1] = row;

        map[(unsigned char)root->character]->combinedPath = globPathSeparator.combined;
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

int findTreeHeight(BinaryTree* root, int level) {
    if(root == NULL) {
        return level - 1;
    }

    if(root->left == NULL && root->right == NULL) {
        return level;
    }

    int left = findTreeHeight(root->left, level + 1);
    int right = findTreeHeight(root->right, level + 1);

    return (left > right)? left : right;
}
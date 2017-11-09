#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

// Structure to represent a min heap node
struct MinHeapNode {
    int  v;
    int dist;
};
 
// Structure to represent a min heap
struct MinHeap {
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist);

struct MinHeap* createMinHeap(int capacity);

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

void minHeapify(struct MinHeap* minHeap, int idx);

int isEmpty(struct MinHeap* minHeap);

struct MinHeapNode* extractMin(struct MinHeap* minHeap);

void decreaseKey(struct MinHeap* minHeap, int v, int dist);

int isInMinHeap(struct MinHeap *minHeap, int v);

#endif
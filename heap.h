#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "utils.h"


// Structure to represent a min heap node
struct MinHeapNode {
    long int  id;
    int type;
};
 
// Structure to represent a min heap
struct MinHeap {
    long int size;      // Number of heap nodes present currently
    long int capacity;  // Capacity of min heap
    int *pos;     // This is needed for decreaseKey()
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int type);

struct MinHeap* createMinHeap(int capacity, int heap_capacity);

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

void minHeapify(struct MinHeap* minHeap, int idx);

int isEmpty(struct MinHeap* minHeap);

struct MinHeapNode* extractMin(struct MinHeap* minHeap);

void decreaseKey(struct MinHeap* minHeap, int v, int type);

int isInMinHeap(struct MinHeap *minHeap, int v);

void freeHeap(struct MinHeap* minHeap);

#endif

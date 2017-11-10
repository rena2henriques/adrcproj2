#include "heap.h"
 
// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int type)
{
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->id = v;
    minHeapNode->type = type;
    return minHeapNode;
}
 
// A utility function to create a Min Heap

/**minHeap->array[0]->id :::: id do nó como maior prioridade
 * minHeap->pos[x] ::::: posição do nó de id = x, na minHeap->array[ ]
 * se minHeap->pos[10] == 0, então minHeap->array[0]->id == 10, i.e. o nó 10 é o com maior prioridade
 * 
 * minHeap->size = [(tamanho total de nós) - (nós já definitivos)]
 * 
 * quando um nó se torna definitivo, troca-se a info de array[0] com array[size] (depois é feito heapify e as prioridades voltam a bater certo)
 * e decrementa-se o minHeap->size, e nunca mais vais olhar para esse nó**/


struct MinHeap* createMinHeap(int capacity, int heap_capacity) {
    struct MinHeap* minHeap = NULL;
    minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = NULL;
    minHeap->pos = (int *) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = heap_capacity;
    minHeap->array = NULL;
    minHeap->array = (struct MinHeapNode**) malloc(heap_capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->type < minHeap->array[smallest]->type )
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->type < minHeap->array[smallest]->type )
      smallest = right;
 
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
 
        // Swap positions
        minHeap->pos[smallestNode->id] = idx;
        minHeap->pos[idxNode->id] = smallest;
 
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
 
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
 
// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
 
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
 
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
	minHeap->array[minHeap->size - 1] = root;
    // Update position of last node
    minHeap->pos[root->id] = minHeap->size-1;
    minHeap->pos[lastNode->id] = 0;
 
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
 
    return root;
}
 
// Function to decreasy dist value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int type)
{
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
 
    // Get the node and update its dist value
    minHeap->array[i]->type = type;
 
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->type < minHeap->array[(i - 1) / 2]->type)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->id] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->id] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
 
        // move to parent index
        i = (i - 1) / 2;
    }
}
 
// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
     return 1;
   return 0;
}


void freeHeap(struct MinHeap* minHeap){

    int i = 0;


    for (i = 0; i < minHeap->capacity; i++) {
        if(minHeap->array[i]) {
            free(minHeap->array[i]);
            minHeap->array[i] = NULL; 
        }

    }

    free(minHeap->array);
    minHeap->array = NULL;
    free(minHeap->pos);
    minHeap->pos = NULL;
    free(minHeap);
    minHeap = NULL;

    return;
}

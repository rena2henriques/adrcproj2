#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include "graph.h"
#include "heap.h"
#include "utils.h"

#define MIN(X, Y) (((X) < (Y)) ? (1) : (0))

#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0
#define UNREACHABLE 4

struct Tier1 {
	long int id;
	struct Tier1 *next;
};

int checkCycle(struct Graph* graph);

int checkCycleRec(int v, int visited[MAX_GRAPH], int recStack[MAX_GRAPH], struct Graph* graph);

void DFS (int v, int visited[MAX_GRAPH], int *visitedCounter,struct Graph* graph, int prevType);//,tier1List) {

int routeIsValid(int prevType, int currentType);

int commercially_connected(struct Graph *graph);

int checkTier1Connections(struct Tier1 **head, struct Graph *graph, long int id);

void freeList(struct Tier1 *head);

void electedRoute(struct Graph *graph, long int dest, unsigned int *provider, unsigned int *peer, unsigned int *customer, int commercialFlag,  struct MinHeap* minHeap);

#endif

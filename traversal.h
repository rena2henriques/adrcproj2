#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include "graph.h"

#define TRUE 1
#define FALSE 0
#define VALID 1
#define INVALID 0

int checkCycle(struct Graph* graph);

int checkCycleRec(int v, int visited[MAX_GRAPH], int recStack[MAX_GRAPH], struct Graph* graph);

void DFS (int v, int visited[MAX_GRAPH], int *visitedCounter,struct Graph* graph, int prevType);//,tier1List) {

int routeIsValid(int prevType, int currentType);


#endif

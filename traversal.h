#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include "graph.h"

#define TRUE 1
#define FALSE 0

int isCyclic(struct Graph* graph);
int isCyclicUtil(int v, int visited[MAX_GRAPH], int recStack[MAX_GRAPH], struct Graph* graph);


#endif
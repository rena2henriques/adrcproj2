#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_GRAPH 64000

// A structure to represent an adjacency list node
struct AdjListNode {
    long int id;
    int type;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    long int V;
    int total_nodes;
    int E;
    struct AdjList *array;
};

struct AdjListNode* newAdjListNode(long int id, int type);

struct Graph* createGraph(long int V);

void addEdge(struct Graph* graph, long int src, long int dest, int type);

void printGraph(struct Graph* graph);

struct Graph* fillGraph(int argc, char const *argv[]);

#endif

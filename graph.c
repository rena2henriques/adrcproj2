#include "graph.h"
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(long int id, int type) {
    struct AdjListNode* newNode = (struct AdjListNode*) mymalloc( sizeof(struct AdjListNode));
    newNode->id = id;
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(long int V) {
    int i;

    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

    graph->V = V;
    graph->total_nodes = 0;
    graph->E = 0;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    graph->firstNode = 64001;
 
     // Initialize each adjacency list as empty by making head as NULL
    for (i = 0; i < V; ++i){
        graph->array[i].head = NULL;
        graph->array[i].testTier1 = 0;
    }
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, long int src, long int dest, int type) {
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest, type);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // if we are inserting a provider route, then this node can't be a tier1
    if(type == 3)
        graph->array[src].testTier1++;

    // increments the number of edges
    graph->E++;
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph(struct Graph* graph) {
    int v;
    struct AdjListNode* pCrawl = NULL;

    for (v = 0; v < graph->V; ++v)
    {
        // only prints if the list has any nodes in it
        if (graph->array[v].head != NULL) {
            pCrawl = graph->array[v].head;
            printf("\n Adjacency list of vertex %d\n head ", v);
            while (pCrawl)
            {
                printf("-> %li type -> %d", pCrawl->id, pCrawl->type);
                pCrawl = pCrawl->next;
            }
            printf("\n");
        }
    }
}

// Reads a file with a graph described as "SRCNUM DESTNUM TYPE"
struct Graph* fillGraph(int argc, char const *argv[]) {

    FILE *file = NULL;
    char temp[15] = "";

    struct Graph* graph = createGraph(MAX_GRAPH);

    // temporary variables
    long int src = 0;
    long int dest = 0;
    int type = 0;

    /* opening the file, if the user gives a file on command line, it's used, 
    otherwise it uses the PrefixTable.txt file*/
    if (argc == 1) {
        if ( (file = fopen("LargeNetwork.txt", "r")) == NULL ) {
            printf("Error while reading file: %s\n", strerror(errno));
            exit(1);
        }
    } else {
        if ( (file = fopen(argv[1], "r")) == NULL ) {
            printf("Error while reading file: %s\n", strerror(errno));
            exit(1);
        }
    }

    while (!feof(file)) {
        if(fgets(temp, sizeof(temp), file) == NULL)
            continue;

        if(sscanf(temp, "%li %li %d", &src, &dest, &type) != 3)
            continue;

		if(graph->array[src].head == NULL) //absolute new node in this graph
			graph->total_nodes++;		   //if the node re-appears on the file, will not be incremented again

        // used for discovering the first valid node (prevents unecessary operations)
        if (src < graph->firstNode)
            graph->firstNode = src;

        addEdge(graph, src, dest, type);
    }

    if ( fclose(file) == EOF ) {
        printf("Error while closing file: %s\n", strerror(errno));
        exit(1);
    }

    return graph;
}


/*Free the graph*/
void freeGraph(struct Graph *graph) {
    if(graph != NULL) {
        if(graph->array != NULL) {

            int v;
            struct AdjListNode *tmp = NULL;
            /*Free up the nodes*/
            for (v = 0; v < MAX_GRAPH; v++) {

                tmp = graph->array[v].head;

                while (graph->array[v].head != NULL) {

                    tmp = graph->array[v].head;
                    graph->array[v].head = tmp->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->array);
        }
        /*Free the graph*/
        free(graph);
    }
}

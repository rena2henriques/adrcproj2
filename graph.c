#include "graph.h"
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(long int id, int type) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
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
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    for (i = 0; i < V; ++i){
        graph->array[i].head = NULL;
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
        if ( (file = fopen("network.txt", "r")) == NULL ) {
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
        fgets(temp, sizeof(temp), file);
        sscanf(temp, "%li %li %d", &src, &dest, &type);

        // TESTAR O SSCANF PARA O CASO QUE NOS DÃO UM FICHEIRO MAL FEITO

        addEdge(graph, src, dest, type);
    }

    if ( fclose(file) == EOF ) {
        printf("Error while closing file: %s\n", strerror(errno));
        exit(1);
    }

    return graph;
}



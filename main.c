#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[]) {

	// counter of the types of routes <- might be needed to change to long int
	unsigned int provider = 0;
	unsigned int peer = 0;
	unsigned int customer = 0;


	int i = 0;

	// flag of the state of the commercial connection
	int commercialFlag = 0;

	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	// graph->V size of pos vector
    // graph->total_nodes size of the heap (only contains relevant nodes
    // antigamente era isto => struct MinHeap* minHeap = createMinHeap(graph->total_nodes); DAVA SEG FAULT NO FILE DO PROF
    struct MinHeap* minHeap = NULL;
    minHeap = createMinHeap(network->V, network->total_nodes);
    

	#ifdef DEBUG
	printGraph(network);
	#endif

	if (checkCycle(network) == TRUE) {
		printf("The network has customer cycles!\n");
		printf("It will not converge!\n");
		return 0;
	} else {
		printf("The network doesn't have customer cycles!\n");
	}

	if((commercialFlag = commercially_connected(network)))
		printf("The network is commercially connected\n");
	else
		printf("The network isn't commercially connected\n");

	for(i = 0; i < MAX_GRAPH; i++){
		if(network->array[i].head != NULL)
			electedRoute(network, i, &provider, &peer, &customer, commercialFlag, minHeap);
	}

	// electedRoute(network, 42, &provider, &peer, &customer, commercialFlag);
	
	printf("Statistics:\n");
	printf("Nº of providers: %d\n", provider);
	printf("Nº of peer: %d\n", peer);
	printf("Nº of customer: %d\n", customer);

	free(minHeap->array);
    minHeap->array = NULL;
    free(minHeap->pos);
    minHeap->pos = NULL;
    free(minHeap);
    minHeap = NULL;

	freeGraph(network);

	return 0;
}

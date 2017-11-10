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
			electedRoute(network, i, &provider, &peer, &customer, commercialFlag);
		/*printf("counter = %li\n", counter);
		counter++;*/
	}

	//electedRoute(network, 1, &provider, &peer, &customer, commercialFlag);
	
	printf("Statistics:\n");
	printf("Nº of providers: %d\n", provider);
	printf("Nº of peer: %d\n", peer);
	printf("Nº of customer: %d\n", customer);

	freeGraph(network);

	return 0;
}

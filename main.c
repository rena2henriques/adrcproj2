#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[]) {

	// counter of the types of routes <- might be needed to change to long int
	int provider = 0;
	int peer = 0;
	int customer = 0;

	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	#ifdef DEBUG
	printGraph(network);
	#endif

	if (checkCycle(network) == TRUE) {
		printf("The network has customer cycles!\n");
	} else {
		printf("The network doesn't have customer cycles!\n");
	}

	if(commercially_connected(network))
		printf("The network is commercially connected\n");
	else
		printf("The network isn't commercially connected\n");

	electedRoute(network, 6, &provider, &peer, &customer);
	
	printf("Statistics:\n");
	printf("Nº of providers: %d\n", provider);
	printf("Nº of peer: %d\n", peer);
	printf("Nº of customer: %d\n", customer);

	freeGraph(network);

	return 0;
}

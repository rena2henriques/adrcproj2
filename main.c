#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[])
{
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

	electedRoute(network, 6);
	
	freeGraph(network);

	return 0;
}

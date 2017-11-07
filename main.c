#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[])
{
	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	printGraph(network);

	if (checkCycle(network) == TRUE) {
		printf("\nThe network has customer cycles!\n");
	} else {
		printf("\nThe network doesn't have customer cycles!\n");
	}

	return 0;
}
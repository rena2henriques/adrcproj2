#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[])
{
	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	printGraph(network);

	if (checkCycle(network) == TRUE) {
		printf("\nThe graph has a cycle!\n");
	} else {
		printf("\nThe graph is acyclic!\n");
	}

	return 0;
}
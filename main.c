#include "graph.h"

int main(int argc, char const *argv[])
{
	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	printGraph(network);

	return 0;
}
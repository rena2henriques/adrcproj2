#include "graph.h"
#include "traversal.h"

int main(int argc, char const *argv[])
{
	int visited[MAX_GRAPH];
	int visited_counter = 0;
	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	printGraph(network);

	if (checkCycle(network) == TRUE) {
		printf("\nThe network has customer cycles!\n");
	} else {
		printf("\nThe network doesn't have customer cycles!\n");
	}

	DFS(1, visited, &visited_counter, network, 3); // para começar pode se mandar o 3 como prev que bate sempre certo? temporario
	return 0;
}

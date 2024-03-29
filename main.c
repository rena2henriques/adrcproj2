#include "graph.h"
#include "traversal.h"

/*****************************************************************************/
/*               Algoritmia e Desempenho em Redes de Computadores            */
/*					  Static Analysis of Inter-AS 							 */
/*                      Paulo Eusébio & Renato Henriques                     */
/*****************************************************************************/

int main(int argc, char const *argv[]) {

	// counter of the types of routes
	unsigned int provider = 0;
	unsigned int peer = 0;
	unsigned int customer = 0;
	unsigned int total = 0;
	
	// variables for menu
	char menu_buff[15] = "";
	int do_all_network=-1;
	int user_choice=0;
	int select_destination=0;
	
	int i = 0;

	// flag of the state of the commercial connection
	int commercialFlag = 0;

	// gets a file with the network description, creates the graph and fill it correctly
	struct Graph* network = fillGraph(argc, argv);

	// graph->V size of pos vector
    // graph->total_nodes size of the heap (only contains relevant nodes)
    struct MinHeap* minHeap = NULL;
    
    //create heap (priority queue) for routes calculation
    minHeap = createMinHeap(network->V, network->total_nodes);
    
	#ifdef DEBUG
	printGraph(network);
	#endif

	if (checkCycle(network) == TRUE) {
		printf("\nThe network has customer cycles!\n");
		printf("It will not converge!\n");
		return 0;
	} else {
		printf("\nThe network doesn't have customer cycles!\n");
	}

	if((commercialFlag = commercially_connected(network)))
		printf("The network is commercially connected\n");
	else
		printf("The network isn't commercially connected\n");

	/*MENU*/
    printf("\nWelcome, please write:\n");
    printf("\n'1 x' to gather statistics from all nodes to node x (replace x by desired node)\n");
    printf("\n'2' to gather statistics from all nodes to all other nodes\n");
    printf("\n >>");
    
	if(fgets(menu_buff, sizeof(menu_buff), stdin) != 0);
    if(sscanf(menu_buff,"%d", &user_choice) == 1 && user_choice == 2)
		do_all_network=1;
    else if(sscanf(menu_buff,"%d %d", &user_choice, &select_destination) == 2 && user_choice == 1)
		do_all_network=0;
	else { 
		printf("Wrong inputs\n");
		return -1;
	}
	
	//According to user choice calculates all routes to all destinations or all routes to one destination
	if(do_all_network == 1){

		for(i = 0; i < MAX_GRAPH; i++){
			if(network->array[i].head != NULL)
				electedRoute(network, i, &provider, &peer, &customer, commercialFlag, minHeap);
		}

	} else if (do_all_network == 0) {
		electedRoute(network, select_destination, &provider, &peer, &customer, commercialFlag, minHeap);
	}
	
	total = provider + peer + customer;

	if(do_all_network == 0)
		printf("\nStatistics for node %d:\n", select_destination);
	if(do_all_network == 1)
		printf("\nStatistics for all nodes:\n");

	printf("Nº of providers routes: %d equals %.2f %% \n", provider, (float)100*provider/total);
	printf("Nº of peer routes: %d equals %.2f %%\n", peer, (float)100*peer/total);
	printf("Nº of customer routes: %d equals %.2f %%\n", customer, (float)100*customer/total);

	free(minHeap->array);
    minHeap->array = NULL;
    free(minHeap->pos);
    minHeap->pos = NULL;
    free(minHeap);
    minHeap = NULL;

	freeGraph(network);

	return 0;
}

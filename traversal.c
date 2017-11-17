#include "traversal.h"

 /**********************************************************************************
  * traversal.c																		   *
  *																				   * 
  * Has all the functions needed analyse the network							   *
  * *******************************************************************************/

//inverte a ligação
static inline int invert(int type) {
    if(type == 1)
        return 3;
    else if(type == 3)
        return 1;
    return type;
}


// search depth first ignoring non customer neighbours
int checkCycleRec(int v, int visited[MAX_GRAPH], int stack[MAX_GRAPH], struct Graph* graph) {

    if(visited[v] == FALSE) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = TRUE;
        stack[v] = TRUE;

        struct AdjListNode *aux = graph->array[v].head;
		
        // Recur for all the vertices adjacent to this aux node
        while(aux != NULL) {

        	// if the relation from v to this node is customer route 
        	if (aux->type == 1) {
        		// if the node has already been visited, then there is a cycle (there's a backedge)
	        	if (!visited[aux->id] && checkCycleRec(aux->id, visited, stack, graph) )
	        		return TRUE;
	        	// if the node was already part of this branch, then there's a cycle
	        	else if (stack[aux->id])
	        		return TRUE;
	        }
			//advances to next neighbour of node v
        	aux = aux->next;
        }
    }

    // there's no cycle in this branch
    stack[v] = FALSE;

    return FALSE;
}
 
// Returns true if the graph contains a cycle and false if acyclic
int checkCycle(struct Graph* graph) {
   
    int visited[MAX_GRAPH];
    int stack[MAX_GRAPH];
    int i = 0;

    // initialization of the arrays at false
    for(i = 0; i < MAX_GRAPH; i++) {
        visited[i] = FALSE;
        stack[i] = FALSE;
    }

    // for every node that is connected to others, call a function that
    // search depth first ignoring non customer neighbours
    for(i = 0; i < MAX_GRAPH; i++) {
    	if (graph->array[i].head != NULL) {
	        if (checkCycleRec(i, visited, stack, graph)){
	        	// there is a cycle
	            return TRUE;
	        }
	    }
    }
 	
 	// there isnt a cycle
    return FALSE;
}

// checks if a certain the route is valid according to routing and exportation rules
int routeIsValid(int prevType, int currentType) {   
        if(currentType == 0) //if we calculating routes, the destination is marked with 0 (simplifies de algolrithm)
            return VALID;
		else if(prevType == 1 && currentType == 1)
			return VALID;
		else if(prevType == 2 && currentType == 1)
			return VALID;
		else if(prevType == 3)
            if (currentType != 4)
                return VALID;
            else
                return INVALID;
        else 
            return INVALID;
}

//checks if all tier1 nodes have a peer relation with all other tier1s
int commercially_connected(struct Graph *graph){

    long int i = 0;

    // linked list with the tier1 nodes
    struct Tier1 *head = NULL;

    // finds the tier1 nodes
    for (i=graph->firstNode;i<MAX_GRAPH;i++){
        // uses the tier1 flag created while reading the file
        if(graph->array[i].testTier1 == 0 && graph->array[i].head != NULL){
            // if the tier1 is connected to all other tier1 in the list, we continue the loop
            // to find the next tier1, else we can conclude that the graph isn't commercially
            // connected

            if(checkTier1Connections(&head, graph, i))
                continue;
            else {
                freeList(head);
                return FALSE;
            }
        }
    }

    freeList(head);

    return TRUE;
}

//given a tier1, check connection with all tier1 found so far

int checkTier1Connections(struct Tier1 **head, struct Graph *graph, long int id){

	//if its first tier1, insert at the head 
    if ((*head) == NULL){
        (*head) = (struct Tier1*) mymalloc( sizeof(struct Tier1));
        (*head)->id = id;
        (*head)->next = NULL;
        return TRUE;
    }

    struct Tier1 *auxTier1 = *head;
    struct AdjListNode *auxGraph = graph->array[id].head;

    // search the tier1 list
    while(auxTier1 != NULL) {
        // search the adj list of the tier1 being tested
        while( auxGraph != NULL){
            // if it finds the connections then there's no need to search til the end
            if(auxTier1->id == auxGraph->id)
                break;

            auxGraph = auxGraph->next;
        }

        // if the adj list was search until the end then no connection was found
        if (auxGraph == NULL){
            return FALSE; //even if it was the last one, the break prevents auxgraph from getting to the null
        }

        // to start at the beginning of the list
        auxGraph = graph->array[id].head;
		// repeat for next node of tier1 list
        auxTier1 = auxTier1->next;
    }

    // inserts the tier1 node in the beginning of the list
    auxTier1 = (struct Tier1*) mymalloc( sizeof(struct Tier1));
    auxTier1->id = id;
    auxTier1->next = *head;
    *head = auxTier1;

    // if we got here it means that this tier1 is connected to any other tier1 node of the list, is now on the list
    return TRUE;
}

// free the linkedlist
void freeList(struct Tier1 *head){

    struct Tier1 *aux = head;

    while(head != NULL) {
        aux = head;
        head = aux->next;
        free(aux);
    }

    return;
}

/* Given a destination, calculates types of route from all other nodes 
 * Uses a heap to facilitate priority searches*/
 
void electedRoute(struct Graph *graph, long int dest, unsigned int *provider, unsigned int *peer, unsigned int *customer, int commercialFlag, struct MinHeap* minHeap){

    int V = graph->V; 
    int type[V];      // dist values used to pick minimum weight edge in cut
    int u = -1;
    struct AdjListNode* auxAdj = NULL;
    struct MinHeapNode* minHeapNode = NULL;
    long int id = -1;
    long int position = 0;
	long int v = 0;
    int invType = 0;

    // graph->V size of pos vector (indicates the position of the node in the heap)
    // graph->total_nodes size of the heap (only contains actual nodes of the network)
    
    // Initialize min heap with all vertices. 
    // Initialize type of route unreachable to all vertices except the destination
    for(v = 0; v < V; ++v) {

        type[v] = UNREACHABLE;

        if(graph->array[v].head != NULL) { //if node is valid
			if(v == dest) //creates top priority for destination node, needed for 1st iteration of algorithm
				type[v] = 0;
				
			minHeap->array[position] = newMinHeapNode(v, type[v]); //saves current node's info in the heap
			minHeap->pos[v] = position; //saves current node's position in the heap
			position++; //variables that keeps track of the actual nodes (that belong to the network)
		}
    }

    // Destin node has top priority, it is extracted first
    decreaseKey(minHeap, dest, type[dest]); // called updateHeap() in our report

    // Initially size of min heap is equal to nº of nodes in the network
    minHeap->size = position;
  
    // In the followin loop, min heap contains all nodes
    // whose type of route calculation is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with better value, decrement heap size
        minHeapNode = extractMin(minHeap);
        u = minHeapNode->id; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted vertex) and update their routes
        auxAdj = graph->array[u].head;

        // skip the nodes that are not included in our network
        if(auxAdj == NULL)
            continue;

        // if the network is commercialy connected and we get a provider route as Min
        // then it means that all the other nodes in the heap have provider route to dest
        if(commercialFlag == 1){
            if(type[u] == 3) {
                (*provider) += minHeap->size + 1; // +1 because of the provider node we took from this cycle
                freeHeap(minHeap);
                return;
            }
        }

        // for statistics data
        switch (type[u]) {
            case 1:
                (*customer)++;
                break;
            case 2:
                (*peer)++;
                break;
            case 3:
                (*provider)++;
                break;
        }
		//gets\updates info for all the adjacent nodes of the current node
        while(auxAdj != NULL) {

            id = auxAdj->id;
			
			//in this algorithm we look at the type of connection in the opposite direction
			//that being from the adjacent node to the current node
            invType = invert(auxAdj->type);

            // 1º condition -> checks if the node has already been decided
            // 2º condition -> checks if the node has isolated (unecessary if the network is commercialy connected)
            // 3º condition -> is the new path better? 
            if( MIN(minHeap->pos[id], minHeap->size) && type[u] != UNREACHABLE && invType<type[id]) {
				//check the validity of the route
                if(routeIsValid(invType,minHeapNode->type)) {
					//update the type of route that this adjacent node uses to get to destin (not definitive)
					type[id] = invType;

					//update adjacent node's priority in the heap, and heapify		
					decreaseKey(minHeap, id, type[id]); // called updateHeap() in our report
                }

            }
            //move to next adjacent node of the current node
            auxAdj = auxAdj->next;
        }
    }

    #ifdef DEBUG
	//imprime a rotas eleitas por cada nó para o destino 
	printf("destino = %li\n", dest);
	for(v = 0; v < MAX_GRAPH; v++)
		if(graph->array[v].head != NULL)
			printf("id = %li, tipo = %d\n", v, type[v]);
    #endif

    freeHeap(minHeap);

	return;
}


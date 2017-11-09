#include "traversal.h"

// COMPLEXITY O(V+E)
// goes through the DFS tree to check cycles
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
	        	// if the node was already part of this node, then there's a cycle
	        	else if (stack[aux->id])
	        		return TRUE;
	        }

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
    // goes through the DFS tree 
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


void DFS (int v, int visited[MAX_GRAPH], int *visitedCounter,struct Graph* graph, int prevType) {//,tier1List) {

    // set the node as visited
    visited[v] = TRUE;
    (*visitedCounter)++;

    // the check if the node is a Tier 1 ISP
    int tier1Flag = 1; // 1 if it's a Tier 1, 0 if isn't

    struct AdjListNode *aux = graph->array[v].head;
        
    // Recur for all the vertices adjacent to this aux node
    while(aux != NULL) {

        if(visited[aux->id] == FALSE) {
            // if the next step of the path is valid by the rules of BGP
            if (routeIsValid(prevType, aux->type)) {
                DFS(aux->id, visited, visitedCounter, graph, aux->type);
            }
        } 
		  
        // if the node has at least a provider, then it isnt a tier1
        if (aux->type == 3)
            tier1Flag = 0;

        aux = aux->next;
    }    

    // insert the id of the node in a list
    if (tier1Flag == 1) {
		printf("node: %d is tier 1\n", v); //temporary
        //insertTier1List(aux->id, tier1List); // CORRECT
    }

    return;
} 

// checks if a certain the route is valid
int routeIsValid(int prevType, int currentType) {
		if(prevType == 1 && currentType == 1)
			return VALID;
		else if(prevType == 2 && currentType == 1)
			return VALID;
		else if(prevType == 3 && currentType == 1)
			return VALID;
		else if(prevType == 3 && currentType == 2)
			return VALID;
		else if(prevType == 3 && currentType == 3)
			return VALID;
        else 
            return INVALID;
}


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
            else
                return FALSE;
        }
    }

    freeList(head);

    return TRUE;
}


int checkTier1Connections(struct Tier1 **head, struct Graph *graph, long int id){

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
        // search the adj list
        while( auxGraph != NULL){
            // if it finds the connections then there's no need to search til the end
            if(auxTier1->id == auxGraph->id)
                break;

            auxGraph = auxGraph->next;
        }

        // if the list was search until the end then no connection was found
        if (auxGraph == NULL){
            return FALSE; //even if it was the last one, the break prevents auxgraph from getting to the null
        }

        // to start at the beginning of the list
        auxGraph = graph->array[id].head;

        auxTier1 = auxTier1->next;
    }

    // inserts the tier1 node in the beginning of the list
    auxTier1 = (struct Tier1*) mymalloc( sizeof(struct Tier1));
    auxTier1->id = id;
    auxTier1->next = *head;
    *head = auxTier1;

    // if we got here it means that this tier1 is connected to any other tier1 node of the list
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


void electedRoute(struct Graph *graph, long int dest){

    int V = graph->V; // Get the number of valid vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut
    int u = -1;
    struct AdjListNode* auxAdj = NULL;
    struct MinHeapNode* minHeapNode = NULL;
    long int id = -1;

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices 
    for (int v = 0; v < V; ++v) {
        dist[v] = UNREACHABLE;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[dest] = newMinHeapNode(dest, dist[dest]);
    minHeap->pos[dest]   = dest;
    dist[dest] = 0;
    decreaseKey(minHeap, dest, dist[dest]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with minimum distance value
        minHeapNode = extractMin(minHeap);
        u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their routes
        auxAdj = graph->array[u].head;

        // for the nodes that are not included in our network
        if(auxAdj == NULL)
            continue;

        while(auxAdj != NULL) {

            id = auxAdj->id;

            // 1º condition -> checks if the node is already been decided
            // 2º condition -> checks if the node is isolated (unecessary if the network is commercialy connected)
            if( isInMinHeap(minHeap, id) && dist[u] != UNREACHABLE) {
                if(routeIsValid(minHeapNode->type, auxAdj->type CORRIGIR -> ESTÁ MAL)) {

                    se eu consigo chegar a um nó por 3, então o peso dele para mim é 1
                    se eu consigo chegar a um nó por 1, então o peso dele para mim é 3
                    se eu consigo chegar a um nó por 2, então o peso dele para mim é 2

                }

            }


            auxAdj = auxAdj->next;
        }

    }

    return;
}

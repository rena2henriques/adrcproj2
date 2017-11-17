#include "traversal.h"

//inverte a ligação
static inline int invert(int type) {
    if(type == 1)
        return 3;
    else if(type == 3)
        return 1;
    return type;
}

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

// Não é usado, mas em homenagem ao tempo que perdemos a bater tecla decidimos deixar
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
        if(currentType == 0) //if we are looking to a destination, the destination is marked with 0
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


void electedRoute(struct Graph *graph, long int dest, unsigned int *provider, unsigned int *peer, unsigned int *customer, int commercialFlag, struct MinHeap* minHeap){

    int V = graph->V; // Get the number of valid vertices in graph
    int type[V];      // dist values used to pick minimum weight edge in cut
    int u = -1;
    struct AdjListNode* auxAdj = NULL;
    struct MinHeapNode* minHeapNode = NULL;
    long int id = -1;
    long int position = 0;
	long int v = 0;
    int invType = 0;

    // graph->V size of pos vector
    // graph->total_nodes size of the heap (only contains relevant nodes
    // antigamente era isto => struct MinHeap* minHeap = createMinHeap(graph->total_nodes); DAVA SEG FAULT NO FILE DO PROF
    // struct MinHeap* minHeap = NULL;
    // minHeap = createMinHeap(graph->V, graph->total_nodes);
    
    // Initialize min heap with all vertices. dist value of all vertices 
    // minHeap->array é a priority queue, tem o tamanho igual ao numero de nós válidos, os elemenos do array são id's e os seus indices são a sua prioridade
    // assim as operações heapify não estão a fazer operações desnecessárias
    for(v = 0; v < V; ++v) {

        type[v] = UNREACHABLE;

        if(graph->array[v].head != NULL) {
			if(v == dest) //creates top priority for destination node, needed for 1st iteration of algorithm
				type[v] = 0;
				
			minHeap->array[position] = newMinHeapNode(v, type[v]);
			minHeap->pos[v] = position;
			position++;
		}
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    decreaseKey(minHeap, dest, type[dest]); // called updateHeap() in our report

    // Initially size of min heap is equal to V
    minHeap->size = position;
  
    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap)) {
        // Extract the vertex with better value
        minHeapNode = extractMin(minHeap);
        u = minHeapNode->id; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their routes
        auxAdj = graph->array[u].head;

        // for the nodes that are not included in our network
        if(auxAdj == NULL)
            continue;

        // if the network is commercialy connected and we get a provider route as Min
        // then it means that all the other nodes in the heap are providers
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

        while(auxAdj != NULL) {

            id = auxAdj->id;

            invType = invert(auxAdj->type);

            // 1º condition -> checks if the node has already been decided
            // 2º condition -> checks if the node has isolated (unecessary if the network is commercialy connected)
            // 3º condition -> is the new path better? 
            if( MIN(minHeap->pos[id], minHeap->size) && type[u] != UNREACHABLE && invType<type[id]) {
                if(routeIsValid(invType,minHeapNode->type)) {
					//a rota que o nó id utilizada para chegar ao destino neste momento é guardada (não definitiva)
					type[id] = invType;

					//se entramos aqui significa que a prioridade do nó = id, melhorou, altera-se a sua prioridade + heapify		
					decreaseKey(minHeap, id, type[id]); // called updateHeap() in our report
                }

            }

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


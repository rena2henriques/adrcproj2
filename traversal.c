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

    // holds the info about the visited nodes
    int visited[MAX_GRAPH];
    int visitedCounter = 0;

    int i = 0;

    // ADICIONAR LISTA DE TIER1

    // initialization of the arrays at false
    for(i = 0; i < MAX_GRAPH; i++) {
        visited[i] = FALSE;
    }

    // loops until it finds the first node with connections
    // only then the DFS is started
    for(i = 0; i < MAX_GRAPH; i++) {
        if (graph->array[i].head != NULL) {
            // we initiate the prevType as 3, because if prevType is 3 then the operation
            // is valid no matter the next step
            DFS (i, visited, &visitedCounter, graph, 3);
            break;
        }
    }

    return visitedCounter; // just because, temporary

    /*if (visitedCounter != nºde ISP) 
        return FALSE;

    if (checkTier1())
        return TRUE;
    else
        return FALSE;*/
}


/*

int checkTier1(stack , struct Graph *graph) {

    struct AdjListNode *aux;
    stack * aux_tier1 = NULL;

    while (stack->next != NULL) {

        id = popStack(stack);
        aux_tier1 = stack->head; // this is the updated stack

        aux = graph->array[id].head;
        while(aux != NULL) {

            while(aux_tier1 != NULL) { 
                if(aux_tier1->id == aux->id) {
                    
                }


                aux_tier1 = aux_tier1->next;
            }

            aux = aux->next;
        }


    }




}
*/

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

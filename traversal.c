#include "traversal.h"

int isCyclicUtil(int v, int visited[MAX_GRAPH], int recStack[MAX_GRAPH], struct Graph* graph) {

    if(visited[v] == FALSE) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = TRUE;
        recStack[v] = TRUE;

        struct AdjListNode *aux = graph->array[v].head;
		
        int i = 0;

        // Recur for all the vertices adjacent to this vertex
        while(aux != NULL) {
        	if (!visited[i] && isCyclicUtil(i, visited, recStack, graph) )
        		return TRUE;
        	else if (recStack[i])
        		return TRUE;

        	aux = aux->next;
        	i++;
        }
    }

    recStack[v] = FALSE;  // remove the vertex from recursion stack

    return FALSE;
}
 
// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in http://www.geeksforgeeks.org/archives/18212
int isCyclic(struct Graph* graph) {

    // Mark all the vertices as not visited and not part of recursion
    // stack
    int visited[MAX_GRAPH];
    int recStack[MAX_GRAPH];
    int i = 0;

    for(i = 0; i < MAX_GRAPH; i++) {
        visited[i] = FALSE;
        recStack[i] = FALSE;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(i = 0; i < MAX_GRAPH; i++) {
        if (isCyclicUtil(i, visited, recStack, graph)){
            return TRUE;
        }
    }
 
    return FALSE;
}

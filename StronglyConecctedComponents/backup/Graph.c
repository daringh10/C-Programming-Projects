/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA2
* Graph.c
* C File for GraphObj
*********************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Graph.h"


//Naming the states of a vertex
typedef enum colorNums { white = 0, gray = 1, black = 2} colorNums;




//GrabObj Struct
typedef struct GraphObj{
	int vertices; // Number of vertices
	int edges; // Size of Graph (AKA number of lines connecting vertices
	
	
	List *adj; //Adjanccey List for each vertix in Graph
	int *colors; //Array to store the color of each vertex, index represens the vertex
	int *parent; //Array that stores the parent of each vertex, index represents vertex
	int *discoverTime;
	int *finishTime;
} GraphObj;

//Function that determines whether a vertex is range
int inRange(Graph G,int x){
	if ( (x >= 1) && (x <= getOrder(G))){
		return 1;
	}
	
	else{
		return 0;
	}
}

//Constructor for a Graph, allocates to heap
Graph newGraph(int n){
	Graph g = calloc(1,sizeof(GraphObj));
	g->vertices = n;
	g->edges = 0;

	
	//Adj array has Lists for elements
	g->adj = calloc((n + 1),sizeof(List));
	for (int i = 1; i < n+1; i ++){
		g->adj[i] = newList();
	}
	
	//The rest of the arrays are ints, of size 1 -> n+1 so access is easier
	g->colors = calloc((n+1), sizeof(int));

	g->parent = calloc((n+1), sizeof(int));
	for (int i = 1; i < n+1; i++){
		g->parent[i] = NIL;
	}
	
	g->discoverTime = calloc((n+1), sizeof(int));
	for (int i = 1; i < n+1; i++){
		g->discoverTime[i] = UNDEF;
	}
	
	g->finishTime = calloc((n+1), sizeof(int));
	for (int i = 1; i < n+1; i++){
		g->finishTime[i] = UNDEF;
	}
	return g;
	
}	

//Frees a GraphObj's memory from the heap
void freeGraph(Graph* pG){
	free( (*pG)->colors);
	free( (*pG)->parent);
	free( (*pG)->discoverTime);
	free( (*pG)->finishTime);
	
	
	for (int i = 0; i < ((*pG)->vertices) + 1; i++){
		if ((*pG)->adj[i] != NULL){
			freeList(&(*pG)->adj[i]);
		}
	}
	free((*pG)->adj);
	free(*pG);
	(*pG) = NULL;
}

/*** Access functions ***/

//Returns the amount of vertices
int getOrder(Graph G){
	return G->vertices;
}

//Returns the amount of edges, AKA the amount of connections between verticies
int getSize(Graph G){
	return G->edges;
}


//Returns the parent of vertex u
int getParent(Graph G, int u){
	if (inRange(G,u)){
		return G->parent[u];
	}
	else{
		return NIL;
	}
	
	
}
int getDiscover(Graph G, int u){
	if (inRange(G,u)){
		return G->discoverTime[u];
	}
	else{
		return UNDEF;
	}
}

int getFinish(Graph G, int u){
	if (inRange(G,u)){
		return G->finishTime[u];
	}
	else{
		return UNDEF;
	}
	
	
}

/*** Manipulation procedures ***/


//For 2 vertices, adds 1 to the others adj. List, and the vice versa.
//Organizes number value in ascending order
void addEdge(Graph G, int u, int v){
	if ( ( inRange(G,u) && inRange(G,v) ) &&  (u !=v) ){

		List adjList_U = G->adj[u];
		moveFront(adjList_U);
		
		if (length(adjList_U) == 0){
			append(adjList_U,v);
		}
		
		else{
			for (int i = 0; i < length(adjList_U); i++){
				if (v < get(adjList_U)){
					insertBefore(adjList_U,v);
					break;
				}
				else{
					if (index(adjList_U) == length(adjList_U) - 1){
						append(adjList_U,v);
						break;
					}
					moveNext(adjList_U);
				}
			}	
		}
		
		List adjList_V = G->adj[v];
		moveFront(adjList_V);
		
		if (length(adjList_V) == 0){
			append(adjList_V,u);
		}
		
		else{
			for (int i = 0; i < length(adjList_V); i++){
				if (u < get(adjList_V)){
					insertBefore(adjList_V,u);
					break;
				}
				else{
					if (index(adjList_V) == length(adjList_V) - 1){
						append(adjList_V,u);
						break;
					}
					moveNext(adjList_V);
				}
			}	
		}
		
		G->edges +=1;
	}
}

//Like addEdge, except ONLY adds "v" to "u"'s adj List
void addArc(Graph G, int u, int v){
	if ( ( inRange(G,u) && inRange(G,v) ) ){

		List adjList_U = G->adj[u];
		moveFront(adjList_U);
		
		if (length(adjList_U) == 0){
			append(adjList_U,v);
		}
		
		else{
			for (int i = 0; i < length(adjList_U); i++){
				if (v < get(adjList_U)){
					insertBefore(adjList_U,v);
					break;
				}
				else{
					if (index(adjList_U) == length(adjList_U) - 1){
						append(adjList_U,v);
						break;
					}
					moveNext(adjList_U);
				}
			}	
		}
		G->edges +=1;
	}
}

void Visit(Graph G, int x, int *time, List *stack){
	*time +=1;
	G->discoverTime[x] = *time;
	G->colors[x] = gray;
	
	moveFront(G->adj[x]);
	for (int y = 0; y < length(G->adj[x]); y++){
		if (G->colors[get(G->adj[x])] == white){
			G->parent[get(G->adj[x])] = x;
			Visit(G,get(G->adj[x]),time,stack);
		}
		
		moveNext(G->adj[x]);
	}
	G->colors[x] = black;
	*time +=1;
	G->finishTime[x] = *time;
	append(*stack,x);
	
	
	
}

void DFS(Graph G, List S){
	List stack = newList();
	if ( (length(S) == getOrder(G) )) {
		moveFront(S);
		for (int x = 0; x < length(S); x++){
			G->colors[get(S)] = white;
			G->parent[get(S)] = NIL;
			moveNext(S);
		}
		int time = 0;
		moveFront(S);
		for (int x = 0; x < length(S); x++){
			if (G->colors[get(S)] == white){
				Visit(G,get(S),&time,&stack);
				
			}
			
			moveNext(S);
		}
		
		clear(S);
		moveBack(stack);
		for (int i =0; i <length(stack) ;i++){
			append(S,get(stack));
			movePrev(stack);
		}
		
	}
	
	else{
		return;
	}
	
	freeList(&stack);
	
}




	
/*** Other operations ***/


//Prints each vertex and it's adj. Matrix.
void printGraph(FILE* out, Graph G){
	for (int i = 1; i < getOrder(G) + 1;i++){
		fprintf(out,"%d: ",i);
		if (length(G->adj[i]) > 0){
			printList(out,G->adj[i]);
		}
		else{
			fprintf(out,"\n");
		}
	}
}

Graph transpose(Graph G){
	Graph gT = newGraph( getOrder(G) );
	for (int i = 1; i < getOrder(G) + 1; i++){
		moveFront(G->adj[i]);
		for (int j = 0; j < length(G->adj[i]); j++){
			addArc(gT,get(G->adj[i]),i);
			moveNext(G->adj[i]);
		}
	}
	return gT;
}

Graph copyGraph(Graph G){
	Graph gCopy = newGraph( getOrder(G) );
	for (int i = 1; i < getOrder(G) + 1; i++){
		gCopy->adj[i] = G->adj[i];
	}
	
	return gCopy;




}


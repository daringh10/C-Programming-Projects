/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA2
* FindComponents.c
* Executes the steps to find the SCC of a graph
*********************************************************************************/

#include "Graph.h"
#include<stdlib.h>
#include "List.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {

	// I.O Initialization
	FILE *inputs;
	FILE *outputs;
	
	//Error if the wrong amount of command line arguments given.
	if (argc == 3){
		inputs = fopen(argv[1],"r");
		outputs = fopen(argv[2],"w");
		
		if (inputs == NULL || outputs == NULL){
			fprintf(stderr,"Please supply correct txt files.\n");
			return 0;
		}
	}
	else{
		fprintf(stderr,"Please supply the correct amount of Arguments\n");
		return 0;
	}
	
	//Establish the variables to hold the pair of inputs from the infile
	//Line by line they are stored into u and v. Also variable to hold
	//The number of vertices
	int vertices;
	int u,v;
	fscanf(inputs,"%d\n",&vertices);
	
	//Creats the graph ADT
	Graph g = newGraph(vertices);
	
	//Scan the infiles vertex pairs into u and v.
	while(fscanf(inputs,"%d %d\n",&u,&v) != EOF){
		if (u == 0 || v== 0){
			break;
		}
		else{
			addArc(g,u,v);
		}
	}
	
	//Print the adjacney List of each vertex
	fprintf(outputs,"Adjacency list representation of G:\n");
	printGraph(outputs,g);
	fprintf(outputs,"\n");
	
	//Creates gT for the SCC algorithm.
	Graph gT = transpose(g);
	
	//List to store the order in which we proccess the verticies.
	List L = newList();
	for (int i = 1; i < getOrder(g) + 1; i++){
		append(L,i);
	}
	
	//Perfrom a DFS on G
	DFS(g,L);
	
	//After the DFS on G, the List L now has the vertices on order of descending time,
	//And DFS on gT on descending times will organize the SCC
	//NOTE: Vertices are apart of the same SCC until a vertex has a null parent,
	//Signifying that a new tree in the forest is starting.
	DFS(gT,L);
	
	//This part is to gather the amount of SCC, and while it is redundant
	//and increases the runtime, I couldn't figure out a way to print the
	//number of SCC before the SCC without looping through first
	int sccCount = 0;
	moveBack(L);
	for (int i =0; i < length(L); i++){
		if (getParent(gT,get(L)) == NIL ){
			sccCount +=1;
		}
		movePrev(L);
	}
	fprintf(outputs,"G contains %d strongly connected components:\n",sccCount);
	
	//Now we iteratre through the List L to print the SCC, a new SCC 
	//starting with a null parent.
	moveBack(L);
	List SCC = newList();
	int sccNum = 0;
	for (int i =0; i < length(L); i++){
		if (getParent(gT,get(L)) != NIL ){
			prepend(SCC,get(L));
		}
		
		else{
			prepend(SCC,get(L));
			sccNum +=1;
			fprintf(outputs,"Component %d: ",sccNum);
			printList(outputs,SCC);
			clear(SCC);
		}
		movePrev(L);
	}
	
	//Frees heap memory.
	freeGraph(&g);
	freeGraph(&gT);
	freeList(&L);
	freeList(&SCC);
	fclose(inputs);
	fclose(outputs);
	return 0;

}

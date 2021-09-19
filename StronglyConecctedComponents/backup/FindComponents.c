/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA2
* FindPath.c
* Greats a Graph ADT and finds shortest path from source to destination
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
	
	int vertices;
	int u,v;
	fscanf(inputs,"%d\n",&vertices);
	Graph g = newGraph(vertices);
	while(fscanf(inputs,"%d %d\n",&u,&v) != EOF){
		if (u == 0 || v== 0){
			break;
		}
		else{
			addArc(g,u,v);
		}
	}
	fprintf(outputs,"Adjacency list representation of G:\n");
	printGraph(outputs,g);
	fprintf(outputs,"\n");
	
	Graph gT = transpose(g);
	List L = newList();
	for (int i = 1; i < getOrder(g) + 1; i++){
		append(L,i);
	}
	DFS(g,L);
	//printList(stdout,L);
	
	//printList(stdout,temp);
	DFS(gT,L);
	//printList(stdout,temp);
	
	int sccCount = 0;
	moveBack(L);
	
	for (int i =0; i < length(L); i++){
		if (getParent(gT,get(L)) == NIL ){
			sccCount +=1;
		}
		movePrev(L);
	}
	fprintf(outputs,"G contains %d strongly connected components:\n",sccCount);
	
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
	
	freeGraph(&g);
	freeList(&L);
	freeList(&SCC);
	fclose(inputs);
	fclose(outputs);
	return 0;

}

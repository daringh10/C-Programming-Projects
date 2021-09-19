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


int main(void){
	Graph A = newGraph(100);
	List L = newList();
	
	addArc(A, 64, 4);
	addArc(A, 64, 3);
	addArc(A, 42, 2);
	addArc(A, 2, 64);
	addArc(A, 4, 2);
	addArc(A, 3, 42);
	for (int i = 1; i <= 100; i++) {
	  prepend(L, i);
	}
	
	DFS(A,L);
	DFS(A,L);
	
	printf("%d",getDiscover(A, 4));
	
	return 0;
}

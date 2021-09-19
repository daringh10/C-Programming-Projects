/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA2
* Graph.g
* Header file for Graph ADT
*********************************************************************************/

#ifndef __GRAPH_H__
#define __GRAPH_H__


#include <stdio.h>
#include "List.h"

#define UNDEF -2
#define NIL 0
typedef struct GraphObj* Graph;


/*** Constructors-Destructors ***/
Graph newGraph(int n);
void freeGraph(Graph* pG);

/*** Access functions ***/
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u);
int getDiscover(Graph G, int u);
int getFinish(Graph G, int u);



/*** Manipulation procedures ***/
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void DFS(Graph G, List S);



/*** Other operations ***/
void printGraph(FILE* out, Graph G);
Graph transpose(Graph G);
Graph copyGraph(Graph G);

#endif


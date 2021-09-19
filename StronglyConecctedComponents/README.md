#Purpose

Utilizies the Graph And List ADT's to find the SCC of a graph. First Does a DFS on a graph G,
and GT (g transpose). For GT, proccess vertices in order of decreasing finishing times from DFS on G.
The List created from DFS on GT will have the SCC in order, in which each vertex with a NULL parent
represents a new tree in the DFS forest.


#Build

Important Commands:

$make FindComponents:
	Executes neccesary makefile commands to create the FindComponents binary file.

#Run

-Run the executable "Lex" with two program arguments:
	./FindComponents <inputfile.txt> <outputfile.txt>

#Erros

None as of final submission
 		


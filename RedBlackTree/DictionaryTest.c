/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#7
* DictionaryTest.c
* Scratchwork file for testing newly functions and such
*********************************************************************************/
#include "Dictionary.h"

int main(int argc, char **argv) {

	Dictionary A = newDictionary(0);
	Dictionary B = newDictionary(0);

	
	insert(A,"pokery",NULL);
	insert(A,"timbrologist",NULL);
	insert(A,"cabbalist",NULL);
	insert(A,"varietally",NULL);
	insert(A,"geothermic",NULL);
	insert(A,"unenquiring",NULL);
	insert(A,"theatricalises",NULL);
	insert(A,"pliosaurs",NULL);
	insert(A,"teleses",NULL);
	insert(A,"phaenology",NULL);
	insert(A,"reapportionment",NULL);
	insert(A,"mcguigan",NULL);
	insert(A,"snowfall",NULL);
	insert(A,"reinfuse",NULL);
	insert(A,"psammitic",NULL);
	insert(A,"punctulated",NULL);
	insert(A,"imprinters",NULL);
	insert(A,"menagaries",NULL);
	insert(A,"gooseries",NULL);
	insert(A,"jemmied",NULL);

	delete(A,"pokery");
	delete(A,"timbrologist");
	

	insert(A, "m",NULL);
	insert(A, "n", NULL);
	insert(A, "o", NULL);
	insert(A, "p", NULL);
	insert(A, "q", NULL);
	insert(A, "f", NULL);
	insert(A, "e", NULL);
	beginForward(A);
	makeEmpty(A);
	if (currentKey(A) != KEY_UNDEF || currentVal(A) != VAL_UNDEF || size(A) != 0) return 1;
	return 0;
	



	printDictionary(stdout,A,"post");


	return 0;
}

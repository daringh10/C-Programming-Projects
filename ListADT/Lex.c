// Darin Gharib
// 1713980
// pa1

#include<stdlib.h>
#include "List.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFF_SIZE 2056
#define TOTAL 2056


//Function for alphabetically sorting the List. This function very similar to insertion
//sort, however, since we are only storing the indices of the array in our List, I create a 
//second list to itterate through to facilitate comparing elements.
List alphabeticalSort(char words[TOTAL][BUFF_SIZE],List L){
    for (int i = 1; i < length(L); i++) {
    	List temp = copyList(L);
    	moveFront(temp);
    	moveFront(L);
    	for (int z = 0; z < i; z++){
    		moveNext(temp);
    		moveNext(L);
    	}
        char *word = words[get(L)];
        movePrev(temp);
        while (index(temp) >= 0 && strcmp(word,words[get(temp)]) < 0) {
        	set(L,get(temp));
        	movePrev(L);
        	movePrev(temp);      	
        }
       set(L,i);
    freeList(&temp);
    }
    return L;
}

int main(int argc, char **argv) {
	
	// Input/Output initialization
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
	
	//Creating an array of TOTAL elements. Each element is a character array or string
	// of size BUFF_SIZE
	
	//BUFF_SIZE stores the characters from fget and adds them into the line array.
	//We can store multiple words per line by stopping our additions into line when we 
	//reach the null character
	char line[TOTAL][BUFF_SIZE];
	int inputFileLineCount = 0;
	int i = 0;
	while (fgets(line[i],BUFF_SIZE,inputs)) {
		line[i][strlen(line[i]) - 1] = '\0';
		i++;
		inputFileLineCount +=1;
    }
    
    //Here we are creating and adding elements to our list.
    List indexList = newList();
    for (int i = 0; i < inputFileLineCount; i++){
    	append(indexList,i);
    }
    indexList = alphabeticalSort(line,indexList);
    
    //Now we print out the elements from line in the correct order based of our List.
    moveFront(indexList);
    for (int i = 0; i < inputFileLineCount; i++){
    	fprintf(outputs,"%s\n",line[get(indexList)]);
    	moveNext(indexList);
    }
    
    //Close our files and free all heap associated memory.
    fclose(inputs);
    fclose(outputs);
 	freeList(&indexList);
	return 0;
}

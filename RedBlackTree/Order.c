/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#7
* Order.c
* Binary file for inserting strings as keys for the Dict ADT
*********************************************************************************/

#include<stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "Dictionary.h"

#define MAX_LEN 300

int main(int argc, char **argv) {
	
	// Input/Output initialization
	FILE *infile;
	FILE *outfile;
	if (argc == 3){
		infile = fopen(argv[1],"r");
		outfile = fopen(argv[2],"w");
		
		if (infile == NULL || outfile == NULL){
			fprintf(stderr,"Please supply correct txt files.\n");
			return 0;
		}
	}
	else{
		fprintf(stderr,"Please supply the correct amount of Arguments\n");
		return 0;
	}

	//Creates a dictionary A
	Dictionary A = newDictionary(0);

	//Creates variables for counting tokens and lines
	int token_count, line_count;
	//Creates an array to hold the contents of the to infile line
	//before inserted into the token buffer
   	char line[MAX_LEN];
   	//A buffer to hold a line's string.
   	char tokenBuffer[MAX_LEN];
   	char* token;

   	//Gets tokens from a line in a text file and inserts that
   	//Whole line as a string into the Dictionary.
	line_count = 0;
	while( fgets(line, MAX_LEN, infile) != NULL)  {
		line_count++;

      	// get tokens in this line
      	token_count = 0;
      	tokenBuffer[0] = '\0';

      	// get first token
      	token = strtok(line, " \n");
      
      	while( token!=NULL ){ 
			
			strcat(tokenBuffer, " ");
			strcat(tokenBuffer, token);
			token_count++;

			
			token = strtok(NULL, " \n");
         }

         insert(A,tokenBuffer,NULL);
	}
   
   	//Iterates through the Dictionary and prints
   	//Nodes in pre,in, and post order.
  	printDictionary(outfile,A,"pre");
  	fprintf(outfile,"\n\n");
  	printDictionary(outfile,A,"in");
  	fprintf(outfile,"\n\n");
  	printDictionary(outfile,A,"post");
    

    //Frees heap memory.
    freeDictionary(&A);
    fclose(infile);
    fclose(outfile);
   
    return 0;
    
}

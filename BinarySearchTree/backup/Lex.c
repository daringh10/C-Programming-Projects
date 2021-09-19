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

	
	Dictionary A = newDictionary(0);
	int token_count, line_count;
   	char line[MAX_LEN];
   	char tokenBuffer[MAX_LEN];
   	char* token;

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

         insert(A,tokenBuffer,line_count);
	}
   
   	beginForward(A);
    for(int i = 0; i < line_count; i++){

    	fprintf(outfile,"%s\n",currentKey(A));
    	next(A);
    }
    


   
    return 0;
    
}

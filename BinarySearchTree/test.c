//-----------------------------------------------------------------------------
// FileIO.c
// Illustrates file input-output commands and text processing using the
// string functions strtok and strcat.
//
// compile:
//
//     gcc -std=c11 -Wall -o FileIO FileIO.c
//
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Dictionary.h"

#define MAX_LEN 300

int main(int argc, char * argv[]){
   Dictionary A = newDictionary(0);
   insert(A, "c", 3);
   insert(A, "b", 2);
   insert(A, "a", 1);
   beginForward(A);
   makeEmpty(A);
   if (currentKey(A) != KEY_UNDEF){
      printf("1\n");
   }
   if(currentVal(A) != VAL_UNDEF){
      printf("2\n");
   }

   if(size(A) != 0){
      printf("3\n");

   }
   return 0;
   
}
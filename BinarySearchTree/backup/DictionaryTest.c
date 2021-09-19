#include "Dictionary.h"



int main(void){
	Dictionary D = newDictionary(0);
	Dictionary E = newDictionary(0);
	Dictionary F = newDictionary(0);

	printf("%d\n",size(D));
	printf("%d\n",getUnique(D));

	insert(D,"a",5);
	insert(D,"a",5);
	insert(D,"b",2);
	insert(D,"b",2);

	insert(E,"a",5);
	insert(E,"a",5);
	insert(E,"b",2);
	insert(E,"b",2);

	printDictionary(stdout,D);
	printf("\n\n");
	printDictionary(stdout,E);

	VAL_TYPE a = lookup(D,"test");
	printf("%d\n\n",a);

	delete(E,"a");
	printDictionary(stdout,E);
	
	delete(E,"g");
	insert(E,"f",6);
	printf("Before the empty\n");
	printDictionary(stdout,E);
	makeEmpty(E);
	printf("After the empty");
	printDictionary(stdout,E);

	char* word[] = { "n","z","w","k","i","c","l","d","t","a", 
                    "e","y","b","h","v","f","s","m","x","r",
                    "o","u","p","g","j","q" };

    for(int i=0; i<26; i++){
      insert(E, word[i], i);
   }

  	printDictionary(stdout,E);

   // add pairs to B
   for(int i=26; i>=0; i--){
      insert(F, word[i], i);
   }
   
   VAL_TYPE x;
   VAL_TYPE y;
   // forward iteration over A ----------------------------
   printf("forward E:\n");
   for(x=beginForward(E); currentVal(E)!=VAL_UNDEF; x=next(E)){
      printf("key: "KEY_FORMAT" value: "VAL_FORMAT"\n", currentKey(E), x);
   }
   printf("\n\n");


    // reverse iteratation over B 
   printf("reverse F:\n");
   for(y=beginReverse(F); currentVal(F)!=VAL_UNDEF; y=prev(F)){
      printf("key: "KEY_FORMAT" value: "VAL_FORMAT"\n", currentKey(F), y);
   }
   printf("\n\n");



	

	return 0;
}
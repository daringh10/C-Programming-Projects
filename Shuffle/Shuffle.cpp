#include<iostream>
#include<fstream>
#include<string>
#include <cmath>
#include "List.h"
using namespace std;

//Shuffle Algorithm
void shuffle(List& D){

	//Create 2 lists to divide D into.
	List A, B;
	D.moveFront();

	//Im using floats to easily determine if a list has an odd # of 
	//items
	double size = D.size();
	double low = floor(size / 2);
	double high = ceil(size / 2);

	//Insert the contents of D into each list in a shuffled manner
	for(int i = 0; i < low; i++){
		A.insertBefore(D.moveNext());
	}
	
	for(int i = 0; i < high; i++){
		B.insertBefore(D.moveNext());
	}

	//Clear D and insert items from A and B 
	//alternating 1 by 1 into D
	D.clear();
	A.moveFront();
	B.moveFront();
	while(A.size() > 0 || B.size() > 0){
		D.insertBefore(B.moveNext());
		B.eraseBefore();
		if(A.size() > 0){
			D.insertBefore(A.moveNext());
			A.eraseBefore();
		}
	}
}




int main(int argc, char * argv[]){
	//Error checking command args.
	if( argc != 2 ){
      cerr << "Usage: " << argv[0] << " <integer>" << endl;
      return(EXIT_FAILURE);
   }

   //Convert the number of shuffles to an int
   int n = stoi(argv[1]);

   //Create the List A which will  be the shuffled List and B
   //which will stand as the first sequence of A. Shuffled until 
   //A == B
   List A,B;
   int count = 0;
   cout << "deck size\tshuffle count"<<endl;
   cout << "------------------------------"<<endl;

   //Continue to run shuffle until both Lists have the same values
   for (int i =1; i < n+1; i++){
   		count = 0;
   		A.insertBefore(i);
   		B = A;

   		shuffle(A);
   		count +=1;
   		while((A==B) == false) {
   			shuffle(A);
   			count +=1;
   		}
   		cout << " " << i <<"\t\t " << count << endl;
   }




   


}
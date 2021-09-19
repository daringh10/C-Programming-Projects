#include<iostream>
#include<fstream>
#include<string>
#include <cmath>
#include "List.h"
using namespace std;

void shuffle(List& D){
	List A, B;

	D.moveFront();
	double size = D.size();
	double low = floor(size / 2);
	double high = ceil(size / 2);

	for(int i = 0; i < low; i++){
		A.insertBefore(D.moveNext());
	}
	

	for(int i = 0; i < high; i++){
		B.insertBefore(D.moveNext());
	}

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
	
	if( argc != 2 ){
      cerr << "Usage: " << argv[0] << " <integer>" << endl;
      return(EXIT_FAILURE);
   }
   int n = stoi(argv[1]);

   List A,B;
   int count = 0;

   cout << "deck size\tshuffle count"<<endl;
   cout << "------------------------------"<<endl;
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
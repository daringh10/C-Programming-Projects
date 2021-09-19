#include<iostream>
#include<string>
#include <fstream>


#include"BigInteger.h"

using namespace std;

int main(int argc, char * argv[]){
	string  s1;
	string  s2;
	ifstream inputs;
	ofstream outputs;
	//Error if the wrong amount of command line arguments given.
	if (argc == 3){
		
		inputs.open(argv[1]);
		outputs.open(argv[2]);

		
	}
	else{
		fprintf(stderr,"Please supply the correct amount of Arguments\n");
		return 0;
	}
	getline(inputs,s1);
	getline(inputs,s2);
	getline(inputs,s2);
	

	BigInteger A = BigInteger(s1);
	BigInteger B = BigInteger(s2);

	cout << A << endl <<endl;
	cout << B << endl <<endl;
	cout << A + B << endl <<endl;
	cout << A - B << endl <<endl;
	cout << A - A << endl <<endl;
	cout << (A+A+A)-(B+B) << endl <<endl;
	cout << (A*B) << endl <<endl;
	cout << (A*A) << endl << endl;
	cout << (B*B) <<endl <<endl;
	BigInteger C = B;

	for(int i = 1; i < 5; i++){
		C = C * B;
		
		
	}	

	

	BigInteger D = C;
	for(int i = 1; i < 16; i++){
		D = D + C;
		
		
	}	

	cout << D <<endl <<endl;
	


	






	return 0;
}
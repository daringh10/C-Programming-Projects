/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#5
* Arithmetic.cpp
* Main Program Client
*********************************************************************************/

#include<iostream>
#include<string>
#include <fstream>


#include"BigInteger.h"

using namespace std;

int main(int argc, char * argv[]){

	//Creates the strings which will hold the inputs and 
	//later be converted to BigIntegers.
	string  s1;
	string  s2;

	//I/O initilization
	ifstream inputs;
	ofstream outputs;
	//Error if the wrong amount of command line arguments given.
	if (argc == 3){
		inputs.open(argv[1]);
		outputs.open(argv[2]);
	}
	//Error Handling for invalid arguments
	else{
		fprintf(stderr,"Please supply the correct amount of Arguments\n");
		return 0;
	}

	//Correctly reads infile for big integer numbers, excluding line 2.
	getline(inputs,s1);
	getline(inputs,s2);
	getline(inputs,s2);
	
	//Creates 2 BigInteger ADT's
	BigInteger A = BigInteger(s1);
	BigInteger B = BigInteger(s2);

	//Prints all the specified calculations in order, with a line in between each.
	outputs << A << endl <<endl;
	outputs << B << endl <<endl;
	outputs << A + B << endl <<endl;
	outputs << A - B << endl <<endl;
	outputs << A - A << endl <<endl;
	outputs << (A+A+A)-(B+B) << endl <<endl;
	outputs << (A*B) << endl <<endl;
	outputs << (A*A) << endl << endl;
	outputs << (B*B) <<endl <<endl;

	//In order to calculate the last expressions, I used 4 for loops.
	//If I wanted, I think I could have made a new BigInteger "4" or "9" 
	//and then multiply inputs by that, but then the LIST ADT's would be uneven
	//when multipled together.
	BigInteger X = A;
	for(int i = 1; i < 4; i++){
		X = X * A;
	}	
	BigInteger Y = X;
	for(int i = 1; i < 9; i++){
		Y = Y + X;
	}	


	BigInteger C = B;
	for(int i = 1; i < 5; i++){
		C = C * B;
	}	
	BigInteger D = C;
	for(int i = 1; i < 16; i++){
		D = D + C;
	}	

	outputs << D + Y <<endl <<endl;

	return 0;
}
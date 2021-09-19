#include <iostream>
#include <string>
#include <climits>
#include <math.h>
#include "BigInteger.h"

using namespace std;

#define POWER 9
#define BASE 1000000000

// Helper Functions ----------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L){
	List temp = L;
	L.clear();
	temp.moveFront();
	for(int i = 0; i < temp.size(); i++){
		L.insertBefore(temp.moveNext()*-1);

	}
}

// scalarMultList()
// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, LIST_ELEMENT m){
	L.moveBack();
	for(int i =0; i <L.size();i++){
		LIST_ELEMENT item = L.movePrev();
		item = item * m;
		L.eraseAfter();
		L.insertAfter(item);
	}

	
}



// sumList()
// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn){
	A.moveBack();
	B.moveBack();
	S.clear();

	int n = A.size();
	if(B.size() > n){
		n = B.size();
	}





	LIST_ELEMENT sum;
	for(int i = 0; i < n; i++){
		LIST_ELEMENT A_digit = A.movePrev();
		LIST_ELEMENT B_digit = B.movePrev();

		
		if(sgn == 1){
			sum = A_digit + B_digit;
			
		}

		else if(sgn == -1){
			sum = A_digit - B_digit;
		}
		
		

		S.insertAfter(sum);
	}

	

}

// normalizeList()
// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used
// by add(), sub() and mult().
int normalizeList(List& L){
	
	List temp;
	bool neg = false;
	int lead_sign = 0;
	L.moveBack();
	int carry = 0;
	LIST_ELEMENT L_digit;


	for (int i = 0; i < L.size(); i++)

	{	


		L_digit = L.movePrev();
		L_digit += carry;
		carry = L_digit/BASE;

	
		if( (i == L.size() - 1) && (L.size()  >1)){
			if (L_digit  < 0){
				
				temp.insertAfter(L_digit);
				neg = true;

				break;
			}
			else if(L_digit == 0){

				LIST_ELEMENT temp1 = temp.moveNext();
				
				if (temp1  < 0){
					
					neg = true;
					break;
				}
				else{
					temp.moveFront();
				}
				
			}
		}
		L_digit = L_digit % BASE;

		if(L_digit > 0){
			lead_sign = 1;
		}
		
		
		
		
		temp.insertAfter(L_digit);
	}

	
	if(neg == true){
		
		
		lead_sign = -1;
		negateList(temp);
		
		normalizeList(temp);
		
	}

	if(carry != 0){
		temp.insertAfter(carry);

	}


	
	if(neg != true){
		temp.moveBack();
		int temp_carry = 0;
		for (int i = 0; i < L.size(); i++){	
			
			L_digit = temp.movePrev();
			
			temp.eraseAfter();
			L_digit += temp_carry;
			if(L_digit < 0){
				L_digit += BASE;
				temp_carry = -1;
				
				
			}
			else{
				temp_carry = 0;
			}
			
			temp.insertAfter(L_digit);
		}	
	}
	


	L = temp;

	
	return lead_sign;

}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p){
	L.moveBack();
	for(int i = 0; i <p; i++){
		L.insertBefore(0);
	}

	
}




// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger(){
	signum = 0;
	digits = digits;
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s){
	signum = 1;
	if(s.length() == 0){
		cerr<<"BigInteger Error: constructor called on empty string"<<endl;
		exit(EXIT_FAILURE);
	}
	int i;
	if(s[0] == '-'){
		i = 1;
		signum = -1;
	}

	else if(s[0] == '+'){
		i = 1;
		signum = 1;
	}
	
	for(unsigned long int k = i; k < s.length();k++){
		if(isdigit(s[k])){
			continue;
		}
		else{
			cerr<< "BigInteger Error: constructor called on non-numeric string"<<endl;
			exit(EXIT_FAILURE);
		}
	}

	
	int sum = 0;
	int count = 1;
	string temp = "";
	digits.moveBack();
	int j = 1;
	for(unsigned long int k = 0; k < s.length() - i; k++){
		while(count <= POWER){
			if(isdigit(s[s.length() - j]) == false){
				k = s.length();
				break;
			}
			temp.insert(0,1,s[s.length() - j]);
			count +=1;
			j+=1;
		}
		

		count = 1;
		
		
		if(temp.length() > 0){
			LIST_ELEMENT n = stol(temp);
			sum += n;
			temp = "";
			digits.insertAfter(n);
			
		}

		else{
			break;
		}
		
		
	}
	if(sum == 0){
		makeZero();
	}


}


// BigInteger()
// Constructor that copies the BigInteger N.
BigInteger::BigInteger(const BigInteger& N){
	this->signum = N.signum;
	this->digits = N.digits;
}


// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign(){
	return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(BigInteger N){
	int n = digits.size();
	if(N.digits.size() > n){
		n = N.digits.size();
	}
	if(sign() > N.sign()){
		return 1;
	}
	else if (N.sign() > sign()){
		return -1;
	}

	int sum1,sum2 = 0;
	digits.moveFront();
	N.digits.moveFront();
	sum1 = 0;
	sum2 = 0;
	int i = 1;
	while(sum1 == sum2 && i < n){
		if(i >= digits.size()){
			sum1 = 0;
		}
		if(i <= N.digits.size()){
			sum2 = 0;
		}
		sum1 = digits.moveNext();
		sum2 = N.digits.moveNext();
		i +=1;

		if(sum2 > sum1){
			return -1;
		}
		else if( sum2 < sum1){
			return 1;
		}
		else{
			continue;
		}
	}

	return 0;
}



// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
	digits.clear();
	signum = 0;
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
	signum = signum *-1;
}



// BigInteger Arithmetic operations ----------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N){
	BigInteger New;
	List temp = N.digits;

	if(this->signum == -1){
		negateList(this->digits);
	}
	if(N.signum== -1){
		negateList(temp);
	}

	sumList(New.digits,this->digits,temp,1);

	New.signum = normalizeList(New.digits);

	if(New.signum == 0){
		New.makeZero();
	}
	
	return New;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N){
	BigInteger New;
	List temp = N.digits;

	if(this->signum == -1){
		negateList(this->digits);
	}
	if(N.signum== -1){
		negateList(temp);
	}

	sumList(New.digits,this->digits,temp,-1);
	
	New.signum = normalizeList(New.digits);
	if(New.signum == 0){
		New.makeZero();
	}
	return New;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N){
	BigInteger New;
	New.digits = this->digits;
	List tempA = this->digits;
	List tempB = N.digits;
	List iterator = tempB;
	int n = tempB.size();
	New.signum = this->signum * N.signum; 
	if(New.signum == 0){
		New.digits.clear();
		return New;
	} 

	LIST_ELEMENT curr_digit;
	for (int i = 0; i < n; i++){
		curr_digit = iterator.movePrev();
		scalarMultList(tempA,curr_digit);
		
		shiftList(tempA,i);
		
		if(i > 0){
			//cout << tempA << "OG" << curr_digit<< endl;
			sumList(tempA,tempA,tempB,1);
		}
		//cout<<i<<endl;
		//cout << "test" <<endl <<tempB<<endl << tempA << endl<<endl;
		
		normalizeList(tempA);
		

		New.digits = tempA;
		tempB = tempA;
		tempA = this->digits;
	}
	New.digits.moveFront();
	
	int check = New.digits.moveNext();
	while (check == 0){
		New.digits.eraseBefore();
		check = New.digits.moveNext();
	}

	if(New.signum == 0){
		New.makeZero();
	}




	return New;
}





// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
	string s = "";
	if(signum == 0){
		s+='0';
		return s;
	}
	if(signum == -1){
		s+= "-";
	}

	int j = 0;
	digits.moveFront();
	if(digits.moveNext() == 0){
		j = 1;
		
	}

	digits.moveFront();
	if(j == 1){
		digits.moveNext();
	}
	string temp = "";
	for( int i = 0; i < digits.size() - j;i++){
		temp += std::to_string(digits.peekNext());
		
		if( i != 0){
			
			while(temp.length() != POWER){
			s+= "0";
			temp+="0";
			}
		}
		
		temp = "";
		s+= std::to_string(digits.moveNext());
	}
	

	return s;
}


 // Overriden Operators -----------------------------------------------------
   
// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
	return stream << N.BigInteger::to_string();
}
// operator==()
// Returns true if and only if A equals B. 
bool operator==( BigInteger& A, const BigInteger& B ){
	if (A.BigInteger::compare(B) == 0){
		return true;
	}
	else{
		return false;
	}
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( BigInteger& A, const BigInteger& B ){
	if (A.BigInteger::compare(B) == -1){
		return true;
	}
	else{
		return false;
	}
}



// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( BigInteger& A, const BigInteger& B ){
	if(A.BigInteger::compare(B) == 0 || A.BigInteger::compare(B) == -1 ){
		return true;
	}
	else{
		return false;
	}
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( BigInteger& A, const BigInteger& B ){
	if(A.BigInteger::compare(B) == 1 ){
		return true;
	}
	else{
		return false;
	}
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( BigInteger& A, const BigInteger& B ){
	if(A.BigInteger::compare(B) == 0 || A.BigInteger::compare(B) == 1 ){
		return true;
	}
	else{
		return false;
	}
}




// operator+()
// Returns the sum A+B. 
BigInteger operator+( BigInteger A, const BigInteger& B ){
	return A.BigInteger::add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
	A = A.BigInteger::add(B);
	return A;
}


// operator-()
// Returns the difference A-B. 
BigInteger operator-( BigInteger A, const BigInteger& B ){
	return A.BigInteger::sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
	A = A.BigInteger::sub(B);
	return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( BigInteger A, const BigInteger& B ){
	return A.BigInteger::mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
	A = A.BigInteger::mult(B);
	return A;
}









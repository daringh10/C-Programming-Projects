#include <iostream>
#include <string>
#include <climits>
#include <math.h>
#include <ctype.h>
#include "BigInteger.h"

using namespace std;

//Macros to define a base and power
#define POWER 9
#define BASE 1000000000

// Helper Functions ----------------------------------------

// negateList()
// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L){

	//Creates a temp List equal to the first List, clears the original List,
	//then  inserts each item from temp into L but multipled by -1.
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

	//Starts from the back of L and moves forward multiplying each item
	//in L by m, then removing the old item and replacing it with the new number
	//item.
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

	//Iterates through both Lists A and B to add each alligned item
	//together and stick that new item in S.
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
	
	//Works on Lists that are illegal in current Base/Power bounds by
	//Correcting values in a List to perform mathematical carries if 
	//Greater than the base or less than 0.
	List temp;
	bool neg = false;
	int lead_sign = 0;
	L.moveBack();
	int carry = 0;
	LIST_ELEMENT L_digit;

	for (int i = 0; i < L.size(); i++)
	{
		//Adds the carry to a number. The first carry will always be 0.
		L_digit = L.movePrev();
		L_digit += carry;
		carry = L_digit/BASE;

		//If the first number in the List is a negative, we know that
		//The number itself is negative which complicates things.
		if( (i == L.size() - 1) && (L.size()  >1)){
			//If Less than 0, then insert the digit normally,but enable the neg
			//bool so we know to negate the list and normalize once more.
			if (L_digit  < 0){
				
				temp.insertAfter(L_digit);
				neg = true;

				break;
			}
			//If the lead digit is 0, check the list for the first value above or
			//below 0 to see if the list is negative or not.
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

	//If the first digit was a negative, we know the number
	//has to be a negative number so change the sign to -1
	if(neg == true){
		lead_sign = -1;
		negateList(temp);
		
		normalizeList(temp);
	}

	//If there is a carry at the end, insert it before the lead list item.
	if(carry != 0){
		temp.insertAfter(carry);

	}

	//Set up so if a number is still negative, it will be changed to fit into
	//its correct value bounded by the base. Only applies to non negative numbers, 
	//So the lead integer must be positive.
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

	//Set the List L to the list we manipulated.
	L = temp;

	
	return lead_sign;

}

// shiftList()
// Prepends p zero digits to L, multiplying L by base^p. Used by mult().

void shiftList(List& L, int p){
	//Add zeroes to a BigInteger, for each consecutuve digit in any number base 10 number is 
	//increased by a power of 10.
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

	//By default sets sign to 1
	signum = 1;

	//Error handling
	if(s.length() == 0){
		cerr<<"BigInteger Error: constructor called on empty string"<<endl;
		exit(EXIT_FAILURE);
	}

	//Checks to see if there is an optional sign at the beginning of the string, 
	//and lets the main loop know that we should iterate through the string starting at 
	//index[1]
 	int i = 0;
	if(s[0] == '-'){
		i = 1;
		signum = -1;
	}
	else if(s[0] == '+'){
		i = 1;
		signum = 1;
	}
	
	//Checks to see if each element of s is a digit.
	for(unsigned long int k = i; k < s.length();k++){
		if(isdigit(s[k])){
			continue;
		}
		else{
			cerr<< "BigInteger Error: constructor called on non-numeric string"<<endl;
			exit(EXIT_FAILURE);
		}
	}

	
	//Main Loop;
	//Iterates through a string s in iteratrions of POWER
	//and adds POWER number of digits to a temp string temp which is converted
	//to a long and inserted to our LIST part of BigInteger.
	int sum = 0;
	int count = 1;
	string temp = "";
	int length = s.length();
	digits.moveBack();
	int j = 1;

	//Main loop for a string s
	for(unsigned long int k = 0; k < s.length() - i; k++){
		//While the iteration is still belonging to a sequence of digits no bigger 
		//than POWER
		while(count <= POWER){
			//If the counter for the number of digits proccesed has
			//finally exceeded s in length.
			if(j > length){
				k = s.length();
				break;
			}
			//If the sign of a number is found, then we know we are at the beginning
			//of a string.
			if(isdigit(s[s.length() - j]) == false){
				k = s.length();
				break;
			}
			//Insert into our string temp the digit we are on
			temp.insert(0,1,s[s.length() - j]);
			count +=1;
			j+=1;
		}
		count = 1;
		
		//If we get to POWER number of iterations, reset temp but first 
		//convert it to a LIST_ELEMENT and insert it into the List digits.
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
	//If all the digits were zero, then digits will be 0.
	if(sum == 0){
		makeZero();
	}
	//If there are any leading zeroes, then get rid of them
	digits.moveFront();
	digits.moveNext();
	for(int i = 0; i <digits.size()-1; i++){
		if(digits.peekNext() == 0 && digits.peekPrev() == 0){
			digits.eraseBefore();
		}
		else{
			break;
		}
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

	//Compares to BigIntegers by individually comparing numbers 
	//from the diigts list.

	//In case the BigIntegers are uneven, set n to the largest one.
	//Doesn't mean it's greater though, could just mean extra zeroes.
	int n = digits.size();
	if(N.digits.size() > n){
		n = N.digits.size();
	}

	//Quickly check to compare the signs.
	if(sign() > N.sign()){
		return 1;
	}
	else if (N.sign() > sign()){
		return -1;
	}

	//Main loop
	//Creates variables sum1 and sum2, one for each list, and compares that size to see
	//if a BigInteger is bigger or smaller.
	int sum1,sum2 = 0;
	digits.moveFront();
	N.digits.moveFront();
	sum1 = 0;
	sum2 = 0;
	int i = 1;
	while(sum1 == sum2 && i < n){

		//Beginning compare check.
		if(i >= digits.size()){
			sum1 = 0;
		}
		if(i <= N.digits.size()){
			sum2 = 0;
		}

		//The sums represent each LIST_ELEMENT per iteration.
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

	//Main Loop
	//Iterates through N's BigIntegers digit List,
	//LIST_ELEMENT by LIST_ELEMENT, each being multiplied to the other list,
	// and storted in a List type temp variable. That variable is then
	//shifted appropriatrley, and added to the old current value of temp.
	//Then normalized to account for carries from the addidtion and then continues
	//on to the next digit of the N digit list.
	LIST_ELEMENT curr_digit;
	for (int i = 0; i < n; i++){
		curr_digit = iterator.movePrev();
		scalarMultList(tempA,curr_digit);
		
		shiftList(tempA,i);
		
		if(i > 0){
			
			sumList(tempA,tempA,tempB,1);
		}
		
		
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

	//Creates a string s which will hold the neccesary digits.
	//First accounts for the 0 BigInteger and negative numbers.
	string s = "";
	if(signum == 0){
		s+='0';
		return s;
	}
	if(signum == -1){
		s+= "-";
	}

	//Find the right spot to begin adding digits to S, negating leading zeroes.
	int j = 0;
	digits.moveFront();
	if(digits.moveNext() == 0){
		j = 1;
		
	}
	digits.moveFront();
	if(j == 1){
		digits.moveNext();
	}

	//Creates a bool to notify us when the front is found, and we can start
	//adding zeroes between numbers. If we don't have this, then we might print leading
	//zeroes.
	bool front_found = false;
	string temp = "";
	for( int i = 0; i < digits.size() - j;i++){
		
		
		temp += std::to_string(digits.peekNext());
		int check = digits.peekNext();
		if( i != 0 && front_found == true){
			//Add zeroes that otherwise wouldn't have been there based
			//on how we stored our numbers with no leading zeroes.
			while(temp.length() != POWER ){
			s+= "0";
			temp+="0";
			}
		}
		
		temp = "";

		//If front is found, then we know we can add zeroes,
		//otherwise, don't yet.
		if(check > 0 || front_found ==true){
			s+= std::to_string(digits.moveNext());
		}
		else{
			digits.moveNext();
		}
		
		if(digits.peekPrev()>0){
			front_found = true;

		}
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









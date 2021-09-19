#include <iostream>
#include <string>
#include <climits>
#include "List.h"



using namespace std;

// Private Constructor ---------------------------------------------------

//Node constructor
List::Node::Node(int x){
	data = x;
	next = nullptr;
	prev = nullptr;
}



// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List(){

	frontDummy = new Node(INT_MIN);
	backDummy  = new Node(INT_MAX);

	frontDummy->next = backDummy;
	backDummy->prev = frontDummy;

	beforeCursor = frontDummy;
	afterCursor = backDummy;

	pos_cursor = 0;
	num_elements = 0;
}

//Copy Constructor
List::List(const List& L){
	frontDummy = new Node(INT_MIN);
	backDummy  = new Node(INT_MAX);

	frontDummy->next = backDummy;
	backDummy->prev = frontDummy;

	beforeCursor = frontDummy;
	afterCursor = backDummy;

	pos_cursor = 0;
	num_elements = 0;

	Node *n = L.frontDummy->next;
	for (int i = 0; i < L.num_elements; i++){
		this->insertBefore(n->data);
		n = n->next;
		
	}

	
}

//Destructor
List::~List(){
	clear();
	delete frontDummy;
	delete backDummy;
}



// Access functions --------------------------------------------------------

// isEmpty()
// Returns true if this List is empty, false otherwise.
bool List::isEmpty(){
	return (num_elements == 0);
}

// size()
// Returns the size of this List.
int List::size(){
	return num_elements;
}

// position()
// Returns the position of the cursor in this List. The value returned
// will be in the range 0 to size().
int List::position(){
	return pos_cursor;
}





// Manipulation procedures -------------------------------------------------

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront(){
	beforeCursor = frontDummy;
	afterCursor = frontDummy->next;
	pos_cursor = 0;

}

// moveBack()
// Moves cursor to position size() in this List.
void List::moveBack(){
	beforeCursor = backDummy->prev;
	afterCursor = backDummy;
	pos_cursor = num_elements;
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<size()
int List::peekNext(){
	if( position() < size() ){
		return afterCursor->data;
	}
	return INT_MIN;
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
int List::peekPrev(){
	if( position() > 0 ){
		return beforeCursor->data;
	}
	return INT_MIN;
}


// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<size() 
int List::moveNext(){
	if( position() < size() ){
		afterCursor = afterCursor->next;
		beforeCursor = beforeCursor->next;
		pos_cursor += 1;
		return beforeCursor->data;
	}
	return INT_MIN;
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
int List::movePrev(){
	if( position() > 0 ){
		afterCursor = afterCursor->prev;
		beforeCursor = beforeCursor->prev;
		pos_cursor -=1;
		return afterCursor->data;
	}
	return INT_MIN;
}	

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(int x){
	Node *n = new Node(x);

	n->next = afterCursor;
	n->prev = beforeCursor;

	beforeCursor->next = n;
	afterCursor->prev = n;

	afterCursor = n;

	num_elements +=1;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(int x){
	Node *n = new Node(x);

	n->prev = beforeCursor;
	n->next = afterCursor;

	beforeCursor->next = n;
	afterCursor->prev = n;

	beforeCursor = n;

	pos_cursor +=1;
	num_elements +=1;

}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<size()
void List::eraseAfter(){
	if( position() < size() ){
		Node *temp = afterCursor->next;

		delete afterCursor;

		afterCursor = temp;
		temp->prev = beforeCursor;
		beforeCursor->next = temp;


		num_elements -=1;
	}
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore(){
	if( position() > 0 ){
		Node *temp = beforeCursor->prev;
		delete beforeCursor;

		beforeCursor = temp;
		temp->next = afterCursor;
		afterCursor->prev = temp;

		num_elements -=1;
		pos_cursor -=1;
	}
}


// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of the element x.
// If x is found, places the cursor immediately after the found element (so 
// eraseBefore() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position size(),
// and returns -1. 
int List::findNext(int x){
	while(pos_cursor != num_elements){
		moveNext();
		if(peekPrev() == x){
			return pos_cursor;
		}
	}
	return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of the element x.
// If x is found, places the cursor immediately before the found element (so 
// eraseAfter() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position 0, and 
// returns -1. 

int List::findPrev(int x){
	while(pos_cursor != 0){
		movePrev();
		if(peekNext() == x){
			return pos_cursor;
		}
	}
	return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique data
// values. The order of the remaining elements is obtained by retaining
// the frontmost occurrance of each element, and removing all other 
// occurances. The cursor is not moved with respect to the retained 
// elements, i.e. it lies between the same two retained elements that it 
// did before cleanup() was called.


void List::cleanup(){
	int current_pos = pos_cursor;
	moveFront();
	moveNext();
	Node *temp1 = frontDummy->next;
	int x = 1;
	while (pos_cursor != num_elements){
		while(pos_cursor != num_elements){
			if (temp1->data == afterCursor->data){
				eraseAfter();
				if (pos_cursor <= current_pos - 1){
					current_pos -=1;
				}
			}
			else{
				moveNext();
				continue;
			}
		}
		x++;
		moveFront();
		for (int i =0; i < x; i++){
			moveNext();
		}
		temp1 = temp1->next;
	}
	moveFront();

	for (int i =0; i <current_pos; i++){
		moveNext();
	}
}

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear(){
	moveFront();
	while (size() != 0){
		eraseAfter();
	}
}


// concat()
// Returns a new List consisting of the elements of this List, followed
// the elements of L. The returned List's cursor will be at its front
// (position 0).
List List::concat(const List& L){
	List New;

	Node *N = L.frontDummy->next;
	Node *M = this->frontDummy->next;

	for (int i = 0; i < this->num_elements; i++){
		New.insertBefore(M->data);
		M = M->next;
		
	}

	for (int i = 0; i < L.num_elements; i++){
		New.insertBefore(N->data);
		N = N->next;
		
	}

	New.moveFront();
	
	cout << New.position() << endl;

	return New;


}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string(){
	Node *N = frontDummy->next;
	string s = "(";

	for (int i = 0; i < size(); i++){
		s += std::to_string(N->data);
		if (i != size() - 1){
			s+=", ";
		}
		N = N->next;
	}
	s+=")";
	return s;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R){

	if(this->num_elements != R.num_elements){
		return false;
	}
	Node *M = this->frontDummy->next;
	Node *N = R.frontDummy->next;

	for(int i = 0; i< R.num_elements; i++){
		if(M->data == N->data){
			M = M->next;
			N = N->next;
			continue;
		}
		else{
			return false;
		}
	}

	return true;


}

// Overriden Operators -----------------------------------------------------
// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ){
	if (this != &L){
		List temp = L;
	

		std::swap(frontDummy,  temp.frontDummy);
		std::swap(backDummy,   temp.backDummy);
		std::swap(afterCursor, temp.afterCursor);
		std::swap(beforeCursor,temp.beforeCursor);
		std::swap(pos_cursor,  temp.pos_cursor);
		std::swap(num_elements,temp.num_elements);

	}

	return *this;
}

// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, List& L ){
	return stream << L.List::to_string();
}

bool operator==( List& A, const List& B ){
	return A.List::equals(B);
}


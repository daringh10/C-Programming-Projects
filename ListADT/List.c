// Darin Gharib
// 1713980
// pa1

#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define DEFAULT_STATE -1
#define BOUNDARY_DATA -1

//Defines a NodeObj ADT
typedef struct NodeObj {
	int data;
    struct NodeObj *next;
    struct NodeObj *prev;
} NodeObj;

//Reference to a NodeObj struct called Node
typedef NodeObj *Node;

//Constructor for a Node
Node newNode(int data){
	Node n = calloc(1,sizeof(NodeObj));
	n->data = data;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

//Frees all heap memory allocated for Nodes
void freeNode(Node* pN){
	if(*pN != NULL && pN != NULL){
		free(*pN);
		*pN = NULL;
	}
}

//Defines a ListObj ADT
typedef struct ListObj{
	Node front;
	Node back;
	int len;
	int cursor;
} ListObj;

//Constructor for a List
List newList(void){
	List l = calloc(1,sizeof(ListObj));
	l->len = 0;
	l->cursor = DEFAULT_STATE;
	l->front = newNode(BOUNDARY_DATA);
	l->back = newNode(BOUNDARY_DATA);
	l->front->next = l->back;
	l->back->prev = l->front;
	return l;
}

//Frees all heap memory allocated for the List
void freeList(List* pL){
	Node temp_node1 = (*pL)->front;
    while (temp_node1 != NULL) {
    	
        Node temp_node2 = temp_node1;
        temp_node1 = temp_node2->next;
        freeNode(&temp_node2);
    }
    free(*pL);
    *pL = NULL;
}

// Access functions -----------------------------------------------------------

//Returns a List's length
int length(List L){
	return L->len;
}

//Returns a List's index current index value
int index(List L){
	return L->cursor;
	
}

//Returns the the data value at index 0
int front(List L){
	if (L->len > 0){
		return L->front->next->data;
	}
	
	else{
		return DEFAULT_STATE;
	}
}

//Returns the data value at index length - 1
int back(List L){
	if (L->len > 0){
		return L->back->prev->data;
	}
	
	else{
		return DEFAULT_STATE;
	}
}

//Returns the data value at index.
int get(List L){
	if( index(L) < 0 || index(L) >= length(L)){
		return -1;
	}


	Node temp = L->front;
	for (int i = 0; i < index(L)+1 ; i++){
		temp = temp->next;
	}
	
	return temp->data;


}

//Returns true if all elements in List A are the same as the elements in
//List B, otherwise false
int equals(List A, List B){
	if (A->len != B->len){
		return 0;
	}
	
	if ( (A->len == 0) && (B->len == 0) ){
		return 1;
	}
	
	Node temp_A = A->front->next;
	Node temp_B = B->front->next;
	
	for (int i = 0; i <length(A); i++){
		if (temp_A->data == temp_B->data){
			temp_A = temp_A->next;
			temp_B = temp_B->next;
		}
		
		else{
			return 0;
		}
	}
	
	return 1;
	
	
}

// Manipulation procedures ----------------------------------------------------

//Clears the whole list so that it becomes empty. No elements.
//Does NOT delete the list
void clear(List L){
	Node temp_node1 = L->front->next;
	for (int i =0; i < length(L); i++){
		Node temp_node2 = temp_node1;
		temp_node1 = temp_node2->next;
		L->front->next = temp_node1;
		temp_node1->prev = L->front;
		freeNode(&temp_node2);
	}
	L->len = 0;
	L->cursor = -1;
	
}

//Sets the index data value to x
void set(List L, int x){
	if ( (length(L) > 0) && index(L) >= 0){
		Node temp = L->front;
		for (int i = 0; i < index(L)+1 ; i++){
			temp = temp->next;
		}
	temp->data = x;
	}
}


//Move's the index to position 0
void moveFront(List L){
	if (L->len > 0){
		L->cursor = 0;
	}
}

//Move the index to position length(L) - 1
void moveBack(List L){
	if (L->len > 0){
		L->cursor = (L->len) - 1;
	}
}

//Moves the index towards the front by one element
void movePrev(List L){
	if ( (L->cursor > 0) && (L->cursor < L->len) ){
		L->cursor -=1;
	}
	
	else if (L->cursor == 0){
		L->cursor = -1;
	}
	
}

//Moves the index towards the back by one element
void moveNext(List L){
	if ( (L->cursor < (L->len - 1) ) && (L->cursor >= 0) ){
		L->cursor +=1;
	}
	
	else if (L->cursor == L->len - 1){
		L->cursor = -1;
	}
	
}

//Inserts an element before the current first element
void prepend(List L, int x){
	Node freshNode;
	freshNode = newNode(x);
	freshNode->next = L->front->next;
	freshNode->prev = L->front;
	L->front->next = freshNode;
	freshNode->next->prev = freshNode;
	L->len +=1;
	L->cursor +=1;
}

//Inserts an element before the current cursor element
void insertBefore(List L, int x){

	if( index(L) < 0 || index(L) >= length(L)){
		return;
	}

	if ( (length(L) > 0) && index(L) >=0){
		if ( (L->cursor >=0) && (L->cursor < L->len) ){
			Node freshNode;
			Node temp;
			freshNode = newNode(x);
			temp = L->front->next;
			for (int i = 0; i < index(L); i++){
				temp = temp->next;
			}
			
			freshNode->next = temp;
			freshNode->prev = temp->prev;
			
			temp->prev = freshNode;
			freshNode->prev->next = freshNode;
			L->cursor +=1;
			L->len +=1;
		}
		
	}
	
	
}

//Inserts an element after the current cursor element
void insertAfter(List L, int x){
if( index(L) < 0 || index(L) >= length(L)){
		return;
	}

	if ( (length(L) > 0) && index(L) >=0){
		if ( (L->cursor >=0) && (L->cursor < L->len) ){
			Node freshNode;
			Node temp;
			freshNode = newNode(x);
			temp = L->front->next;
			for (int i = 0; i < index(L); i++){
				temp = temp->next;
			}
			
			freshNode->next = temp->next;
			freshNode->prev = temp;
			
			temp->next = freshNode;
			freshNode->next->prev = freshNode;
			L->len +=1;
			
		}
		
	}
	
}


//Inserts an element after the current final element
void append(List L, int x){
	Node freshNode;
	freshNode = newNode(x);
	freshNode->next = L->back;
	freshNode->prev = L->back->prev;
	L->back->prev = freshNode;
	freshNode->prev->next = freshNode;
	L->len +=1;
}

//Deletes the first List element and restructures the list
void deleteFront(List L){
	Node temp = L->front->next;
	L->front->next->next->prev = L->front;
	L->front->next = L->front->next->next;
	freeNode(&temp);
	L->len -=1;
	L->cursor -=1;
}

//Deletes the last List element and restructures the list
void deleteBack(List L){
	Node temp = L->back->prev;
	L->back->prev->prev->next = L->back;
	L->back->prev = L->back->prev->prev;
	freeNode(&temp);
	L->len -=1;
	if (L->cursor >= length(L)){
		L->cursor = -1;
	}
	
	
}	

//Deletes the current cursor element and restructures the list
void delete(List L){
	if( index(L) < 0 || index(L) >= length(L)){
		return;
	}
	
	Node iterator_node = L->front->next;
	for (int i = 0; i < index(L); i++){
		iterator_node = iterator_node->next;
	}
	
	Node temp = iterator_node->prev;
	temp->next->next->prev = temp;
	temp->next = temp->next->next;
	freeNode(&iterator_node);
	L->len -=1;
	L->cursor = -1;
	
	
}


// Other operations -----------------------------------------------------------

//Prints the List
void printList(FILE* out, List L){

	if (length(L) < 1){
		return;
	}
	Node temp;
    temp = L->front->next;

    for (int i = 0; i < length(L); i++) {
        fprintf(out,"%d ",temp->data);
        temp = temp->next;
    }
}

//Creates a new list and copies all elements from L into the new list
List copyList(List L){
	List new = newList();
	Node tempOldList = L->front->next;
	for (int i = 0; i < length(L); i++){
		append(new,tempOldList->data);
		tempOldList = tempOldList->next;	
	}
	return new ; 
}






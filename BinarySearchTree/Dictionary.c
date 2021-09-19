/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#6
* Dictionary.c
* Dictionary ADT file
*********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Dictionary.h"


//Creates a copy of a string by copying len elements from a
//mem lodcation s to the new location new and returns the new string.
char *str_dup(const char *s) {

    //Creates a pointer a character pointer for the new word
    char *new;

    //Records the amount of characters in s. The + 1
    //is for the \0
    size_t len = strlen(s) + 1;

    //Allocates enough memory for a new string
    //the duplicate of s.
    new = malloc(len);

    //Takes into account if malloc fails.
    if (new == NULL) {
        free(new);
        return NULL;
    }
    //Copes len characters from mem  location s to new
    memcpy(new, s, len);

    return new;
}


// Private Constructors/Destructors ---------------------------------------------------

//Defines a NodeObj ADT
typedef struct NodeObj {
	KEY_TYPE key;
	VAL_TYPE val;
    struct NodeObj *left_child;
    struct NodeObj *right_child;
    struct NodeObj *parent;
} NodeObj;

//Reference to a NodeObj struct called Node
typedef NodeObj *Node;

//Constructor for a Node
Node newNode(KEY_TYPE key,VAL_TYPE val){
	Node n = calloc(1,sizeof(NodeObj));
	n->key =  str_dup(key);
	n->val =  val;
	n->left_child = NULL;
	n->right_child = NULL;
	n->parent = NULL;
	return n;
}

//Frees all heap memory allocated for Nodes
void freeNode(Node* pN){
	if((*pN)->key != NULL){
		free((*pN)->key);
	}
	free(*pN);
	*pN = NULL;
}

//Returns the minimum node in a tree
Node TreeMinimum(Node x){
	if (x == NULL){
		return NULL;
	}
	while(x->left_child != NULL){
		x = x->left_child;
	}
	return x;
}
//Returns the next node in a tree
//Give a node as an argument
Node TreeSuccessor(Node x){
	if(x->right_child != NULL){
		return TreeMinimum(x->right_child);
	}
	Node y = x->parent;
	while((y!=NULL) && (x== y->right_child)){
		x = y;
		y = y->parent;
	}
	return y;
}

// Constructors-Destructors ---------------------------------------------------

//Defines a Dictionary ADT
typedef struct DictionaryObj{
	int size;
	int unique;
	Node current_node;
	Node root;
}DictionaryObj;

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique){
	Dictionary d = calloc(1,sizeof(DictionaryObj));
	d->root = NULL;
	d->unique = unique;
	d->size = 0;
	d->current_node = NULL;
	return d;
}


//Frees the dictionary by first emptying it, then 
//freeing the memory allocated for the dict.
void freeDictionary(Dictionary* pD){
	if(*pD != NULL){
		makeEmpty(*pD);
		free(*pD);
		*pD = NULL;
	}
	
}




// Access functions -----------------------------------------------------------

// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D){
	return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D){
	return D->unique;
}

Node TreeSearch(Node x, KEY_TYPE k){
	if(x == NULL || KEY_CMP(k,x->key) == 0){
		return x;
	}
	else if(KEY_CMP(k,x->key) < 0){
		return TreeSearch(x->left_child,k);
	}
	else{
		return TreeSearch(x->right_child,k);
	}
	
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){
	Node n = TreeSearch(D->root,k);
	if(n == NULL){
		return VAL_UNDEF;
	}
	else{
		return n->val;
	}
}

// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){
	Node z = newNode(k,v);
	
	Node y = NULL;
	Node x = D->root;

	if(getUnique(D) == 1){
		if(lookup(D,k) != VAL_UNDEF){
			return;
		}
	}
	while(x!= NULL){
		y = x;
		if(KEY_CMP(z->key,x->key) < 0){
			x = x->left_child;
		}
		else{
			x = x->right_child;
		}
	}
	z->parent = y;
	if(y == NULL){
		D->root = z;
	}
	else if(KEY_CMP(z->key,y->key) < 0){
		y->left_child = z;
	}

	else{
		y->right_child = z;
	}
	D->size +=1;

}

//Returns the maximum node in a tree
Node TreeMaximum(Node x){
	if (x == NULL){
		return NULL;
	}
	while(x->right_child!= NULL){
		x = x->right_child;
	}
	return x;
}

//A helper function for delete, rearranges a section
//of the tree to fit BST properties after a node has been
//deleted
void Transplant(Dictionary D, Node u, Node v){
	if(u->parent == NULL){
		D->root = v;
	}
	else if (u == u->parent->left_child){
		u->parent->left_child = v;
	}
	else{
		u->parent->right_child = v;
	}

	if(v!= NULL){
		v->parent = u->parent;
	}
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k){
	Node n = TreeSearch(D->root,k);
	if(n == NULL){
		return;
	}	

	if(n == D->current_node){
		D->current_node = NULL;
	}

	if(n->left_child == NULL){
		Transplant(D,n,n->right_child);
	}
	else if (n->right_child == NULL){
		Transplant(D,n,n->left_child);
	}
	else{
		Node y = TreeMinimum(n->right_child);
		if(y->parent != n){
			Transplant(D,y,y->right_child);
			y->right_child = n->right_child;
			y->right_child->parent = y;
		}	

		Transplant(D,n,y);
		y->left_child = n->left_child;
		y->left_child->parent = y;
	}

	freeNode(&n);

	D->size -=1;

}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D){
	Node temp_node1 = TreeMinimum(D->root);
	while(temp_node1!=D->root && temp_node1 != NULL){
		
		delete(D,temp_node1->key);
		temp_node1 = TreeMinimum(D->root);
	}
	temp_node1 = TreeMaximum(D->root);
	while(temp_node1!=D->root && temp_node1 != NULL){
		
		delete(D,temp_node1->key);
		temp_node1 = TreeMaximum(D->root);
	}
	temp_node1 = D->root;
	if(temp_node1 != NULL){
		
		delete(D,temp_node1->key);
	}
    D->current_node = NULL;
    

    D->size = 0;

}

// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D){
	if(size(D) == 0){
		return VAL_UNDEF;
	}
	D->current_node = TreeMinimum(D->root);
	return D->current_node->val;
}

// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D){
	if(size(D) == 0){
		return VAL_UNDEF;
	}
	D->current_node = TreeMaximum(D->root);
	return D->current_node->val;
}

// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D){
	if(D->current_node == NULL){
		return KEY_UNDEF;
	}
	return D->current_node->key;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D){
	if(D->current_node == NULL){
		return VAL_UNDEF;
	}
	return D->current_node->val;
}


//Returns the previous node in a tree,
//given a node.
Node TreePredecessor(Node x){
	if(x->left_child != NULL){
		return TreeMaximum(x->left_child);
	}

	Node y = x->parent;
	while(y!= NULL && x==y->left_child){
		x = y;
		y = y->parent;
	}
	return y;
}

// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D){
	if( (D->current_node == TreeMaximum(D->root)) || (D->current_node == NULL)){
		D->current_node = NULL;
		return VAL_UNDEF;
	}

	D->current_node = TreeSuccessor(D->current_node);
	return D->current_node->val;
}

// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D){
	if( (D->current_node == TreeMinimum(D->root)) || (D->current_node == NULL)){
		D->current_node = NULL;
		return VAL_UNDEF;
	}

	D->current_node = TreePredecessor(D->current_node);
	return D->current_node->val;
}



// Other operations -----------------------------------------------------------

//A standard tree walk, going in order starting from
//the root
void InOrderTreeWalk(Node x,FILE* out){
	if(x != NULL){
		InOrderTreeWalk(x->left_child,out);
		fprintf(out,"%s\n",x->key);
		InOrderTreeWalk(x->right_child,out);
	}
}

void PreOrderTreeWalk(Node x, FILE* out){
	if(x!= NULL){
		fprintf(out,"%s\n",x->key);
		PreOrderTreeWalk(x->left_child,out);
		PreOrderTreeWalk(x->right_child,out);
	}
}

void PostOrderTreeWalk(Node x, FILE* out){
	if(x!= NULL){
		PreOrderTreeWalk(x->left_child,out);
		PreOrderTreeWalk(x->right_child,out);
		fprintf(out,"%s\n",x->key);
	}
}
 
// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D){
	InOrderTreeWalk(D->root,out);
	fprintf(out,"\n");
}

// printDictionary()
// Prints the keys (only) of D in an order determined by the parameter ord.
// If ord is "pre", "in" or "post", executes the corresponding tree traversal
// on the underlying RBT, printing keys only (no values).  If ord is some
// other string, prints nothing.
void printDictionary(FILE* out, Dictionary D, const char* ord){
	if(ord == "pre"){
		PreOrderTreeWalk(D->root,out);
		
	}

	else if(ord == "in"){
		InOrderTreeWalk(D->root,out);
	}

	else if(ord == "post"){
		PostOrderTreeWalk(D->root,out);
	}
	else{
		return;
	}
}
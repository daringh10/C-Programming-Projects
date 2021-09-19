/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#7
* Dictionary.c
* Dictionary ADT file
*********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Dictionary.h"

#define RED 1
#define BLACK 0


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

    //Color Red = 1;
    int color;
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
	n->color = BLACK;
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



// Constructors-Destructors ---------------------------------------------------

//Defines a Dictionary ADT
typedef struct DictionaryObj{
	int size;
	int unique;
	Node current_node;
	Node root;
	Node NIL;
	
}DictionaryObj;

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique){
	Dictionary d = calloc(1,sizeof(DictionaryObj));
	d->unique = unique;
	d->size = 0;
	d->NIL = newNode("",NULL);
	d->current_node = d->NIL;
	d->root = d->NIL;
	return d;
}


//Frees the dictionary by first emptying it, then 
//freeing the memory allocated for the dict.
void freeDictionary(Dictionary* pD){
	if(*pD != NULL){
		makeEmpty(*pD);
		freeNode(&(*pD)->NIL);
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

Node TreeSearch(Dictionary D, Node x, KEY_TYPE k){
	if(x == D->NIL || KEY_CMP(k,x->key) == 0){
		return x;
	}
	else if(KEY_CMP(k,x->key) < 0){
		return TreeSearch(D,x->left_child,k);
	}
	else{
		return TreeSearch(D,x->right_child,k);
	}
	
}

//Returns the minimum node in a tree
Node TreeMinimum(Dictionary D, Node x){
	if (x == D->NIL){
		return D->NIL;
	}
	while(x->left_child != D->NIL){
		x = x->left_child;
	}
	return x;
}
//Returns the next node in a tree
//Give a node as an argument
Node TreeSuccessor(Dictionary D,Node x){
	if(x->right_child != D->NIL){
		return TreeMinimum(D,x->right_child);
	}
	Node y = x->parent;
	while((y!=D->NIL) && (x== y->right_child)){
		x = y;
		y = y->parent;
	}
	return y;
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){

	Node n = TreeSearch(D,D->root,k);
	if(n == D->NIL){
		return VAL_UNDEF;
	}
	else{
		return n->val;
	}
}

// Manipulation procedures ----------------------------------------------------

void leftRotate(Dictionary D, Node x){
	Node y = x->right_child;

	x->right_child = y->left_child;	
	if(y->left_child != D->NIL){
		y->left_child->parent = x;
	}

	y->parent = x->parent;
	if(x->parent == D->NIL){
		D->root = y;
	}
	else if (x == x->parent->left_child){
		x->parent->left_child = y;
	}
	else{
		x->parent->right_child = y;
	}

	y->left_child = x;
	x->parent = y;
}

void rightRotate(Dictionary D, Node x){
	Node y = x->left_child;

	x->left_child = y->right_child;
	if(y->right_child!= D->NIL){
		y->right_child->parent = x;
	}

	y->parent = x->parent;
	if(x->parent == D->NIL){
		D->root = y;
	}

	else if (x == x->parent->right_child){
		x->parent->right_child = y;
	}

	else{
		x->parent->left_child = y;
	}

	y->right_child = x;
	x->parent = y;

}

void RB_InsertFixUp(Dictionary D, Node z){
	Node y;
	

	while(z->parent->color == RED){

		if(z->parent == z->parent->parent->left_child){

			y = z->parent->parent->right_child;
			if(y->color == RED){
				z->parent->color = BLACK;           // CASE 1
				y->color = BLACK;                   // CASE 1
				z->parent->parent->color = RED;     // CASE 1
				z = z->parent->parent;              // CASE 1
			}

			else{
				if (z == z->parent->right_child){
					z = z->parent;
					leftRotate(D,z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRotate(D,z->parent->parent);
			}
		}



		else{

			y = z->parent->parent->left_child;
			
			if(y->color == RED){

				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;


			}	


			else{

				if(z == z->parent->left_child){

					z = z->parent;
					rightRotate(D,z);
				}

				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				leftRotate(D,z->parent->parent);
			}
		}
	}
	

	D->root->color = BLACK;

}



// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){

	if(getUnique(D) == 1){
		if(lookup(D,k) != VAL_UNDEF){
			return;
		}
	}


	D->size+=1;
	Node z = newNode(k,v);
	Node y = D->NIL;
	Node x = D->root;
	
	while(x!= D->NIL){
		y = x;
		if(KEY_CMP(z->key,x->key) < 0){
			x = x->left_child;
		}
		else{
			x = x->right_child;
		}
	}
	
	z->parent = y;
	if(y == D->NIL){
		D->root = z;
	}
	else if(KEY_CMP(z->key,y->key) < 0){
		y->left_child = z;
	}
	else{
		y->right_child = z;
	}
	
	z->left_child = D->NIL;
	z->right_child = D->NIL;
	z->color = RED;
	
	RB_InsertFixUp(D,z);
	

}

//Returns the maximum node in a tree
Node TreeMaximum(Dictionary D,Node x){
	if (x == D->NIL){
		return D->NIL;
	}
	while(x->right_child!= D->NIL){
		x = x->right_child;
	}
	return x;
}

//A helper function for delete, rearranges a section
//of the tree to fit BST properties after a node has been
//deleted
void Transplant(Dictionary D, Node u, Node v){
	if(u->parent == D->NIL){
		D->root = v;
	}
	else if (u == u->parent->left_child){
		u->parent->left_child = v;
	}
	else{
		u->parent->right_child = v;
	}

	
	v->parent = u->parent;
	
}

void RB_DeleteFixUp(Dictionary D, Node x){
	while( (x != D->root) && (x->color == BLACK) ){
		if(x== x->parent->left_child){
			Node w = x->parent->right_child;
			if(w->color == RED){
				w->color = BLACK;
				x->parent->color = RED;
				leftRotate(D,x->parent);
				w = x->parent->right_child;
			}

			if( (w->left_child->color == BLACK)&& (w->right_child->color == BLACK) ){
				w->color = RED;
				x = x->parent;
			}
			else{
				if(w->right_child->color == BLACK){
					w->left_child->color = BLACK;
					w->color = RED;
					rightRotate(D,w);
					w = x->parent->right_child;
				}
				w->color = x->parent->color;
				x->parent->color =BLACK;
				w->right_child->color = BLACK;
				leftRotate(D,x->parent);
				x = D->root;
			}
		}

		else{
			Node w = x->parent->left_child;
			if(w->color == RED){
				w->color = BLACK;
				x->parent->color = RED;
				rightRotate(D,x->parent);
				w = x->parent->left_child;
			}
			if( (w->right_child->color == BLACK) && (w->left_child->color == BLACK) ){
				w->color = RED;
				x = x->parent;
			}
			else{
				if(w->left_child->color == BLACK){
					w->right_child->color = BLACK;
					w->color = RED;
					leftRotate(D,w);
					w = x->parent->left_child;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left_child->color = BLACK;
				rightRotate(D,x->parent);
				x = D->root;
			}
		}
	}
	x->color = BLACK;
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k){
	Node z = TreeSearch(D,D->root,k);
	if(z == D->NIL){
		return;
	}	

	if(z == D->current_node){
		D->current_node = D->NIL;
	}

	D->size -=1;
	Node x;
	Node y = z;
	int y_original_color = y->color;
	if(z->left_child == D->NIL){
		x = z->right_child;
		Transplant(D,z,z->right_child);
	}

	else if(z->right_child == D->NIL){
		x = z->left_child;
		Transplant(D,z,z->left_child);
	}

	else{
		y = TreeMinimum(D,z->right_child);
		y_original_color = y->color;
		x = y->right_child;
		if(y->parent == z){
			x->parent = y;
		}
		else{
			Transplant(D,y,y->right_child);
			y->right_child = z->right_child;
			y->right_child->parent = y;
		}

		Transplant(D,z,y);
		y->left_child = z->left_child;
		y->left_child->parent = y;
		y->color = z->color;
	}
	if(y_original_color == BLACK){
		RB_DeleteFixUp(D,x);
	}


	freeNode(&z);


}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D){
	Node temp_node1 = TreeMinimum(D,D->root);
	while(temp_node1!=D->root && temp_node1 != D->NIL){
		
		delete(D,temp_node1->key);
		temp_node1 = TreeMinimum(D,D->root);
	}
	temp_node1 = TreeMaximum(D,D->root);
	while(temp_node1!=D->root && temp_node1 != D->NIL){
		
		delete(D,temp_node1->key);
		temp_node1 = TreeMaximum(D,D->root);
	}
	temp_node1 = D->root;
	if(temp_node1 != D->NIL){
		
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
	D->current_node = TreeMinimum(D,D->root);
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
	D->current_node = TreeMaximum(D,D->root);
	return D->current_node->val;
}

// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D){
	if(D->current_node == D->NIL|| D->current_node == NULL){
		return KEY_UNDEF;
	}
	return D->current_node->key;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D){
	if(D->current_node == D->NIL || D->current_node == NULL){
		return VAL_UNDEF;
	}
	return D->current_node->val;
}


//Returns the previous node in a tree,
//given a node.
Node TreePredecessor(Dictionary D,Node x){
	if(x->left_child != D->NIL){
		return TreeMaximum(D,x->left_child);
	}

	Node y = x->parent;
	while(y!= D->NIL && x==y->left_child){
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
	if( (D->current_node == TreeMaximum(D,D->root)) || (D->current_node == D->NIL)){
		D->current_node = D->NIL;
		return VAL_UNDEF;
	}

	D->current_node = TreeSuccessor(D,D->current_node);
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
	if( (D->current_node == TreeMinimum(D,D->root)) || (D->current_node == D->NIL)){
		D->current_node = D->NIL;
		return VAL_UNDEF;
	}

	D->current_node = TreePredecessor(D,D->current_node);
	return D->current_node->val;
}



// Other operations -----------------------------------------------------------

//A standard tree walk, going in order starting from
//the root
void InOrderTreeWalk(Dictionary D,Node x,FILE* out){
	if(x != D->NIL){
		InOrderTreeWalk(D,x->left_child,out);
		fprintf(out,"%s\n",x->key);
		InOrderTreeWalk(D,x->right_child,out);
	}
}

void PreOrderTreeWalk(Dictionary D, Node x, FILE* out){
	if(x!= D->NIL){
		fprintf(out,"%s\n",x->key);
		PreOrderTreeWalk(D,x->left_child,out);
		PreOrderTreeWalk(D,x->right_child,out);
	}
}

void PostOrderTreeWalk(Dictionary D, Node x, FILE* out){
	if(x!= D->NIL){
		PostOrderTreeWalk(D,x->left_child,out);
		PostOrderTreeWalk(D,x->right_child,out);
		fprintf(out,"%s\n",x->key);
	}
}
 


// printDictionary()
// Prints the keys (only) of D in an order determined by the parameter ord.
// If ord is "pre", "in" or "post", executes the corresponding tree traversal
// on the underlying RBT, printing keys only (no values).  If ord is some
// other string, prints nothing.
void printDictionary(FILE* out, Dictionary D, const char* ord){
	if(strcmp(ord,"pre") == 0){
		fprintf(out,"******************************************************\n"
					"PRE-ORDER:\n"
					"******************************************************\n");
		PreOrderTreeWalk(D,D->root,out);
		
	}

	else if(strcmp(ord,"in") == 0){
		fprintf(out,"******************************************************\n"
					"IN-ORDER:\n"
					"******************************************************\n");
		InOrderTreeWalk(D,D->root,out);
	}

	else if(strcmp(ord,"post") == 0){
		fprintf(out,"******************************************************\n"
					"POST-ORDER:\n"
					"******************************************************\n");
		PostOrderTreeWalk(D,D->root,out);
	}
	else{
		return;
	}
}
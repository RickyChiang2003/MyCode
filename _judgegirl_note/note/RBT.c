# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef enum ColorType {RED, BLACK} ColorType; 
typedef struct RBTnode{
    int value;
    struct RBTnode* left;
    struct RBTnode* right;
    struct RBTnode* p;
    ColorType color;
} RBTnode;
typedef struct RBTroot{
    RBTnode* root;
    RBTnode* nil;
} RBTroot;

void RBTLeft_Rotate(RBTroot* T, RBTnode* x){
    RBTnode* y = x->right;
    x->right = y->left;
    if (y->left != T->nil)
       	y->left->p = x;
    y->p = x->p;
    if (x->p == T->nil)
       	T->root = y;
    else if (x == x->p->left)
	x->p->left = y;
    else
	x->p->right = y;
    y->left = x;
    x->p = y;
}
void RBTRight_Rotate(RBTroot* T, RBTnode* y){
    RBTnode* x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
       	x->right->p = y;
    x->p = y->p;
    if (y->p == T->nil)
       	T->root = x;
    else if (y == y->p->right)
	y->p->right = x;
    else
	y->p->left = x;
    x->right = y;
    y->p = x;
}
void RBTInsert_Fixup(RBTroot* T, RBTnode* x){
    RBTnode* y = T->nil;
    while (x->p->color == RED){
	if (x->p == x->p->p->left){
	    y = x->p->p->right;
	    if (y->color == RED){
		x->p->color = BLACK;
		y->color = BLACK;
		x->p->p->color = RED;
		x = x->p->p;
	    }
	    else{ //y->color = BLACK
		if (x == x->p->right){
		    x = x->p;
		    RBTLeft_Rotate(T, x);
		}
		x->p->color = BLACK;
		x->p->p->color = RED;
		RBTRight_Rotate(T, x->p->p);
	    }
	}
	else if (x->p == x->p->p->right){
	    y = x->p->p->left;
	    if (y->color == RED){
		x->p->color = BLACK;
		y->color = BLACK;
		x->p->p->color = RED;
		x = x->p->p;
	    }
	    else{ //y->color = BLACK
		if (x == x->p->left){
		    x = x->p;
		    RBTRight_Rotate(T, x);
		}
		x->p->color = BLACK;
		x->p->p->color = RED;
		RBTLeft_Rotate(T, x->p->p);
	    }
	}
	else printf("You Fucked up, haiyaa......\n"), exit(-1);
    }
    T->root->color = BLACK;
}
void RBTDelete_Fixup(RBTroot* T, RBTnode* x){
    RBTnode* w = T->nil;
    while (x != T->root && x->color == BLACK){
	if (x == x->p->left){
	    w = x->p->right;
	    if (w->color == RED){
		w->color = BLACK;
		x->p->color = RED;
		RBTLeft_Rotate(T, x->p);
		w = x->p->right;
	    }
	    if (w->left->color == BLACK && w->right->color == BLACK){
		w->color = RED;
		x = x->p;
	    }
	    else {
		if (w->right->color == BLACK){
		    w->left->color = BLACK;
		    w->color = RED;
		    RBTRight_Rotate(T, w);
		    w = x->p->right;
		}
		w->color = x->p->color;
		x->p->color = BLACK;
		w->right->color = BLACK;
		RBTLeft_Rotate(T, x->p);
		x = T->root; 
	    }
	}
	else if (x == x->p->right){
	    w = x->p->left;
	    if (w->color == RED){
		w->color = BLACK;
		x->p->color = RED;
		RBTRight_Rotate(T, x->p);
		w = x->p->left;
	    }
	    if (w->right->color == BLACK && w->left->color == BLACK){
		w->color = RED;
		x = x->p;
	    }
	    else {
		if (w->left->color == BLACK){
		    w->right->color = BLACK;
		    w->color = RED;
		    RBTLeft_Rotate(T, w);
		    w = x->p->left;
		}
		w->color = x->p->color;
		x->p->color = BLACK;
		w->left->color = BLACK;
		RBTRight_Rotate(T, x->p);
		x = T->root; 
	    }
	}
	else printf("You Fucked up, haiyaa......\n"), exit(-1);
    }
    x->color = BLACK;
}
void RBTInsert(RBTroot* T, int key){
    RBTnode* newnode = T->root;
    RBTnode* p = newnode;
    while (newnode != T->nil){
	p = newnode;
	if (newnode->value == key)
	    { printf("repeated key: %d\n", key); return; }
	newnode = (newnode->value > key) ? newnode->left : newnode->right;
    }

    newnode = (RBTnode*)malloc(sizeof(RBTnode));
    if (newnode == NULL) 
	printf("newnode's calloc failed\n"), exit(-1); // return (-1);
    newnode->value = key;
    newnode->color = RED;
    newnode->left = newnode->right = T->nil;
    newnode->p = p;

    if(p == T->nil)
	T->root = newnode;
    else if (newnode->value >= p->value)
	p->right = newnode;
    else
	p->left = newnode;
    RBTInsert_Fixup(T, newnode);
}
void RBTDelete(RBTroot* T, int key){
    if(T->root == T->nil) printf("the tree is empty\n"), exit(-1);
    RBTnode* delnode = T->root;
    while(delnode->value != key){
	delnode = (delnode->value > key) ? delnode->left : delnode->right;
	if(delnode == T->nil) 
	    printf("key not found: %d\n", key), exit(-1);
    }
    
    if(delnode->left != T->nil && delnode->right != T->nil){
	RBTnode* tmp = delnode->right;
	if(tmp != T->nil) 
	    while(tmp->left != T->nil)
		tmp = tmp->left;
	key = delnode->value = tmp->value;
	delnode = tmp;
    }
    RBTnode* x = (delnode->left == T->nil) ? delnode->right : delnode->left;
    if(delnode->p == T->nil)
	T->root = x;
    else if(delnode == delnode->p->left)
	delnode->p->left = x;
    else
	delnode->p->right = x;
    x->p = delnode->p;

    if (delnode->color == BLACK)
	RBTDelete_Fixup(T, x);
    free(delnode);
    T->nil->p = T->nil->left = T->nil->right = NULL;
    T->nil->color = BLACK; // don't know if this is essential
}
RBTroot* Creat_RBT(){
    RBTroot* T = (RBTroot*)malloc(sizeof(RBTroot));
    if(T == NULL) printf("Creat_RBT malloc failed.\n"), exit(-1);
    T->nil = (RBTnode*)malloc(sizeof(RBTnode));
    if(T->nil == NULL) printf("Creat_RBT malloc failed.\n"), exit(-1);
    T->nil->color = BLACK;
    T->nil->left = T->nil->right = T->nil->p = NULL;
    T->root = T->nil;
    return T;
}


void inorder(RBTroot* T, RBTnode* node){
    if(node != T->nil){
	inorder(T, node->left);
	printf("%d ", node->value);
	inorder(T, node->right);
    }
}


int main(){
    RBTroot* T = Creat_RBT();
    RBTInsert(T, 3);
    RBTInsert(T, 5);
    RBTInsert(T, 1);
    RBTInsert(T, 4);
    RBTInsert(T, 6);
    RBTInsert(T, 9);
    RBTInsert(T, 2);
    inorder(T, T->root);
    printf("\n");
    RBTDelete(T, 2);
    inorder(T, T->root);
    printf("\n");
    RBTDelete(T, 1);
    inorder(T, T->root);
    printf("\n");
    RBTDelete(T, 9);
    inorder(T, T->root);
    printf("\n");
    RBTDelete(T, 6);
    inorder(T, T->root);
    printf("\n");


    return 0;
}

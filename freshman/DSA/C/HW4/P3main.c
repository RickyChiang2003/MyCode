# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
# define SIZE 100001
# define m 1000000007
# define ll long long


// RBT
typedef enum ColorType {RED, BLACK} ColorType; 
typedef struct RBTnode{
    // |ai-bi|, subT_size, inorder_traversal_odd-even
    ll abs;
    ll subT_size;
    ll subT_iome; // subT_inorder_odd_minus_even
    struct RBTnode* left;
    struct RBTnode* right;
    struct RBTnode* p;
    ColorType color;
} RBTnode;
typedef struct RBTroot{
    RBTnode* root;
    RBTnode* nil;
} RBTroot;

ll dailyprice[SIZE] = {0};
ll dailyabs[SIZE] = {0};

void RBTLeft_Rotate(RBTroot* T, RBTnode* x){
    RBTnode* y = x->right;
    ll a1 = x->left->subT_size;
    ll a2 = x->left->subT_iome;
    ll b1 = y->left->subT_size;
    ll b2 = y->left->subT_iome;
    ll c1 = y->right->subT_size;
    ll c2 = y->right->subT_iome;

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

    // maintain
    x->subT_size = a1 + b1 + 1;
    y->subT_size = x->subT_size + c1 + 1;
    x->subT_iome = a2 + (x->abs - b2)*((a1 & 1) ? -1 : 1);
    y->subT_iome = x->subT_iome + (y->abs - c2)*((x->subT_size & 1) ? -1 : 1);
}
void RBTRight_Rotate(RBTroot* T, RBTnode* y){
    RBTnode* x = y->left;
    ll a1 = x->left->subT_size;
    ll a2 = x->left->subT_iome;
    ll b1 = x->right->subT_size;
    ll b2 = x->right->subT_iome;
    ll c1 = y->right->subT_size;
    ll c2 = y->right->subT_iome;

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

    // maintain
    y->subT_size = b1 + c1 + 1;
    x->subT_size = a1 + y->subT_size + 1;
    y->subT_iome = b2 + (y->abs - c2)*((b1 & 1) ? -1 : 1);
    x->subT_iome = a2 + (x->abs - y->subT_iome)*((a1 & 1) ? -1 : 1);
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
	// else printf("You Fucked up, haiyaa......\n"), exit(-1);
    }
    T->root->color = BLACK;
}
void RBTDelete_Fixup(RBTroot* T, RBTnode* x){
    //printf("delete fixup start\n");
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
	// else printf("You Fucked up, haiyaa......\n"), exit(-1);
    }
    x->color = BLACK;
}
void RBTInsert(RBTroot* T, ll key){
    RBTnode* newnode = T->root;
    RBTnode* p = T->nil;
    while (newnode != T->nil){
	p = newnode;
	//if (newnode->value == key)
	//    return; //{ printf("repeated key: %d\n", key); return; }
	newnode = (newnode->abs > key) ? newnode->left : newnode->right;
    }
    newnode = (RBTnode*)malloc(sizeof(RBTnode));
    if (newnode == NULL) 
	printf("newnode's calloc failed\n"), exit(-1); // return (-1);
    newnode->abs = key;
    newnode->subT_size = 1;
    newnode->subT_iome = key;
    newnode->color = RED;
    newnode->left = newnode->right = T->nil;
    newnode->p = p;
    if(p == T->nil)
	T->root = newnode;
    else if (newnode->abs >= p->abs)
	p->right = newnode;
    else
	p->left = newnode;

    //maintain
    for(RBTnode* tmp = newnode; tmp != T->nil; tmp = tmp->p)
	tmp->subT_size = (tmp->left->subT_size)+(tmp->right->subT_size)+1;
    for(RBTnode* tmp = newnode; tmp != T->nil; tmp = tmp->p){
	ll type = (tmp->left->subT_size & 1) ? -1 : 1;
	tmp->subT_iome = (tmp->left->subT_iome)
	    +(tmp->abs - tmp->right->subT_iome)*type;
    }

    RBTInsert_Fixup(T, newnode);
}
void RBTDelete(RBTroot* T, ll key){
    //printf("delete start\n");
    if(T->root == T->nil) printf("the tree is empty\n"), exit(-1);
    RBTnode* delnode = T->root;
    while(delnode->abs != key){
	delnode = (delnode->abs > key) ? delnode->left : delnode->right;
	if(delnode == T->nil) 
	    printf("key not found: %lld\n", key), exit(-1);
    }

    if(delnode->left != T->nil && delnode->right != T->nil){
	RBTnode* tmp = delnode->right;
	while(tmp->left != T->nil)
	    tmp = tmp->left;
	key = delnode->abs = tmp->abs;
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
    //printf("delete BST no RE\n");

    // maintain
    for(RBTnode* tmp = delnode->p; tmp != T->nil; tmp = tmp->p)
	tmp->subT_size = (tmp->left->subT_size)+(tmp->right->subT_size)+1;
    for(RBTnode* tmp = delnode->p; tmp != T->nil; tmp = tmp->p){
	ll type = (tmp->left->subT_size & 1) ? -1 : 1;
	tmp->subT_iome = (tmp->left->subT_iome)
	    +(tmp->abs - tmp->right->subT_iome)*type;
    }

    //printf("delete maintain no RE\n");
    if (delnode->color == BLACK)
	RBTDelete_Fixup(T, x);
    free(delnode);
    T->nil->p = T->nil->left = T->nil->right = NULL;
    T->nil->abs = T->nil->subT_size = T->nil->subT_iome = 0;
    T->nil->color = BLACK;
    //printf("delete no RE\n");
}
RBTroot* Creat_RBT(){
    RBTroot* T = (RBTroot*)malloc(sizeof(RBTroot));
    if(T == NULL) printf("Creat_RBT malloc failed.\n"), exit(-1);
    T->nil = (RBTnode*)malloc(sizeof(RBTnode));
    if(T->nil == NULL) printf("Creat_RBT malloc failed.\n"), exit(-1);
    T->nil->abs = T->nil->subT_size = T->nil->subT_iome = 0;
    T->nil->left = T->nil->right = T->nil->p = NULL;
    T->nil->color = BLACK;
    T->root = T->nil;
    return T;
}

ll find_costA(RBTroot* T, ll totalprice, ll costtype){
    return ((totalprice + (T->root->subT_iome)*costtype) >> 1) ;
}


int main(){
    int N, M;
    ll costA, totalprice = 0, costtype;
    scanf("%d%d", &N, &M);
    costtype = (N & 1) ? -1 : 1;
    RBTroot* T = Creat_RBT();
    for(int cnt = 1; cnt <= N; cnt++){
	ll a, b;
	scanf("%lld%lld", &a, &b);
	RBTInsert(T, llabs(a-b));

	totalprice += a + b;
	dailyprice[cnt] = a + b;
	dailyabs[cnt] = llabs(a-b);
    }
    costA = find_costA(T, totalprice, costtype);
    printf("%lld\n", costA);

    ll t,c,d,e,f,price1, price2;
    for(int cnt = 1; cnt < M; cnt++){
	scanf("%lld%lld%lld%lld%lld", &t, &c, &d, &e, &f);
	price1 = (((c * (costA % m)) % m) + d) % m;
	price2 = (((e * (costA % m)) % m) + f) % m;
	RBTDelete(T, dailyabs[t]);
	RBTInsert(T, llabs(price1-price2));

	totalprice += price1 + price2 - dailyprice[t];
	dailyprice[t] = price1 + price2;
	dailyabs[t] = llabs(price1-price2);
	costA = find_costA(T, totalprice, costtype);
	printf("%lld\n", costA);
    }

    return 0;
}

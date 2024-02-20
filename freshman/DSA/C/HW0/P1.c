#include <stdio.h>
#include <string.h>
# define SIZE 258


typedef struct{
    char num[SIZE];
    int L;
} bignum;


void init(bignum* a, bignum* b, bignum* ans, bignum* two){
    memset(a->num, 0, SIZE);
    memset(b->num, 0, SIZE);
    memset(ans->num, 0, SIZE);
    memset(two->num, 0, SIZE);
    ans->num[0] = 1;
    ans->L = 1;
    two->num[0] = 2;
    two->L = 1;
}


void adjust(bignum* x){
    char t[SIZE] = {0};
    int L = strlen(x->num);
    for(int i = 0; i < L; i++)
       	t[i] = x->num[i];
    for(int i = 0; i < L; i++) 
	x->num[L-i-1] = t[i] - '0';
    x->L = L;
}


int needswap(bignum a, bignum b){ // if a smaller, return 0;
    if(a.L > b.L)
	return 1;
    if(a.L == b.L)
	for(int i = a.L-1; i >= 0; i--){
	    if(a.num[i] < b.num[i])
		return 0;	
	    if(a.num[i] > b.num[i])
		return 1;
	}
    
    return 0;

}


void swap(bignum *a, bignum *b){
    char tmp[SIZE] = {0};
    for(int i = 0; i < a->L; i++)
	tmp[i] = a->num[i];
    for(int i = 0; i < a->L; i++)
	a->num[i] = b->num[i];
    for(int i = 0; i < a->L; i++)
	b->num[i] = tmp[i];

    int t = a->L;
    a->L = b->L;
    b->L = t;
}


int iseven(bignum x){
    return (x.num[0] % 2 == 0);
}

void DIV(bignum *x, char c){  // int slack off version
    char r = 0;
    for(int i = x->L-1; i >= 0; i--){
	r = 10*r + x->num[i];
	x->num[i] = r / c;
	r = r % c;
    }
    if(x->num[x->L-1] == 0)
	x->L--;
}


void MUL(bignum *a, bignum *b){
    char tmp[SIZE] = {0};
    for(int i = 0; i < a->L; i++)
	for(int j = 0; j < b->L; j++)
	    tmp[i+j] += (a->num[i]) * (b->num[j]);

    for(int i = 0; i < a->L + b->L; i++){
	a->num[i+1] = tmp[i+1] += tmp[i] / 10;
	a->num[i] = tmp[i] %= 10;
    }

    a->L = 0;
    for(int i = SIZE - 1; i >= 0; i--)
	if(a->num[i] != 0){
	    a->L = i+1;
	    return;
	}
}


void SUB(bignum *b, bignum *a){
    for(int i = 0; i < b->L; i++)
	b->num[i] -= a->num[i];
    for(int i = 0; i < b->L; i++)
	if(b->num[i] < 0){
	    b->num[i+1]--;
	    b->num[i] += 10;
	}

    b->L = 0;
    for(int i = SIZE-1; i >= 0; i--)
	if(b->num[i] != 0){
	    b->L = i+1;
	    return;
	}
}


void print(bignum x){
    for(int i = x.L-1; i >= 0; i--)
	printf("%c", (x.num[i]+'0'));
    printf("\n");
}


int main(){
    bignum a, b, ans, two;
    init(&a, &b, &ans, &two);
    scanf("%s%s", a.num, b.num);

    adjust(&a);
    adjust(&b);

    if(needswap(a, b))
	swap(&a, &b);

    while((a.L != 0) && (b.L != 0)){
	int ae = iseven(a);
	int be = iseven(b);
	if(ae && be){
	    DIV(&a, 2);
	    DIV(&b, 2);
	    MUL(&ans, &two);
	}
	else if(ae)
	    DIV(&a, 2);
	else if(be)
	    DIV(&b, 2);
		
	if(needswap(a, b))
	    swap(&a, &b);
	SUB(&b, &a);
    }
    MUL(&ans, &a);
    print(ans);


    return 0;
}

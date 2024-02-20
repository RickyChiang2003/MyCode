# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
# define ll long long
# define SIZE 200001
// n,m <= 200000

int ans_dead[SIZE] = {0};

int hasEOF = 0;
int readchar(){
    static int N = 1 << 20;
    static char buf[1 << 20];
    static char *p = buf, *end = buf;
    if (p == end){
        if ((end = buf + fread(buf, 1, N, stdin)) == buf){
            hasEOF = 1;
            return EOF;
        }
        p = buf;
    }
    return *p++;
}

int ReadInt(int *x){
    char c;
    while ((c = readchar()) < '-'){
        if (c == EOF)
            return 0;
    }
    *x = c - '0';
    while ((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c - '0';
    return 1;
}


int Readll(long long *x){
    char c;
    while ((c = readchar()) < '-'){
        if (c == EOF)
            return 0;
    }
    *x = c - '0';
    while ((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c - '0';
    return 1;
}


typedef struct Knight{
    ll h;          // real h is (h-total_a)
    ll a;
    int time;      // real time is (time+times)
    int dead;
    int id;
    struct SET* set;
} Knight;


typedef struct SET{
    ll total_a;       // total atk
    ll total_d;       // total dmg
    int len;          // real len of arr is (len+1)
    int arrlen;       // the total length of the arr(some of the nodes are empty)
    int times;
    struct Knight* arr;
} SET;

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

void swapll(ll* a, ll* b){
    ll t = *a;
    *a = *b;
    *b = t;
}

void swapaddr(Knight** a, Knight** b){
    Knight* tmp = *a; 
    *a = *b; 
    *b = tmp;
}

void swapnode(Knight** addr, Knight* a, Knight* b){
    swapll(&(a->h), &(b->h));
    swapll(&(a->a), &(b->a));
    swap(&(a->time), &(b->time));
    swap(&(a->dead), &(b->dead));
    swap(&(a->id), &(b->id));
    swapaddr(&(addr[a->id]), &(addr[b->id]));
}

int isdead(Knight *p){
    return (p->dead);
}

void heapify(Knight** addr, Knight *arr, int len){
    int i = 1, ii = 2, done = 0;
    while(!done){
	if(ii >= len)
	    done = 1;
	else if(ii+1 >= len){
	    if(arr[i].h > arr[ii].h)
		swapnode(addr, &(arr[i]), &(arr[ii]));
	    done = 1;
	}
	else{
	    if(arr[i].h > arr[ii].h){
		if(arr[ii].h > arr[ii+1].h)
		    swapnode(addr, &(arr[i]), &(arr[ii+1])), i = ii+1;
		else
		    swapnode(addr, &(arr[i]), &(arr[ii])), i = ii;
	    }
	    else if(arr[i].h > arr[ii+1].h)
		swapnode(addr, &(arr[i]), &(arr[ii+1])), i = ii+1;
	    else
		done = 1;
	}
	ii = i << 1;
    }
}

void UNION(Knight** addr, int a1, int a2){
    // set1: long, set2: short
    // adjust arr
    SET* set1 = addr[a1]->set, *set2 = addr[a2]->set;
    int len1 = set1->len, len2 = set2->len;
    int time_change = set2->times - set1->times;
    int h_change = set1->total_d - set2->total_d;
    Knight* arr1 = set1->arr,* arr2 = set2->arr;
    while(len2 > 0){
	len1++;
	int l = 0;
	if(len1 >= set1->arrlen-1){
	    l = (set1->arrlen) << 1;
	    Knight* tmp = calloc(l, sizeof(*tmp));
	    for(int i = 1; i < len1; i++){
		tmp[i].h = arr1[i].h;
		tmp[i].a = arr1[i].a;
		tmp[i].time = arr1[i].time;
		tmp[i].dead = arr1[i].dead;
		tmp[i].id = arr1[i].id;
		tmp[i].set = set1;
		addr[arr1[i].id] = &(tmp[i]);
	    }
	    free(arr1);
	    arr1 = set1->arr = tmp;
	    if(l != 0)
		set1->arrlen = (set1->arrlen) << 1;
	}
	arr1[len1].h = arr2[len2].h + h_change;
	arr1[len1].a = arr2[len2].a;
	arr1[len1].time = arr2[len2].time + time_change;
	arr1[len1].dead = arr2[len2].dead;
	arr1[len1].id = arr2[len2].id;
	arr1[len1].set = set1;
	addr[arr2[len2].id] = &(arr1[len1]);
	len2--;
	for(int i = len1; arr1[i>>1].h > arr1[i].h && i > 1; i = i>>1)
	    swapnode(addr, &(arr1[i>>1]), &(arr1[i]));
    }

    // adjust set
    set1->len = len1;
    set1->total_a += set2->total_a;
}


int main(){
    int n,m;
    ReadInt(&n);ReadInt(&m);
    SET* set = calloc(n+1, sizeof(*set));
    Knight** addr = calloc(n+1, sizeof(*addr));
    for(int i = 1; i <= n; i++){
        set[i].arr = calloc(2, sizeof(*(set[i].arr)));
	set[i].arr[1].set = &(set[i]);
	set[i].arr[1].id = i;
	set[i].arrlen = 2;
	set[i].len = 1;
	addr[i] = &(set[i].arr[1]);
        Readll(&(set[i].arr[1].h));
    }
    for(int i = 1; i <= n; i++)
	Readll(&(set[i].arr[1].a)),
	set[i].total_a = set[i].arr[1].a;
    

    for(int i = 0; i < m; i++){
	int x, y;
	ReadInt(&x), ReadInt(&y);
	SET* setx = addr[x]->set, *sety = addr[y]->set;
	if(isdead(addr[x])||isdead(addr[y])||setx->arr[1].id == sety->arr[1].id)
	    continue;

	// adjust x
	setx->times += 1;

	// adjust y
	sety->total_d += setx->total_a;
	while(sety->len > 0){
	    if(sety->total_d >= (sety->arr[1]).h){
		sety->total_a -= sety->arr[1].a;
		sety->arr[1].dead = 1;
		addr[sety->arr[1].id]->dead = 1;
		sety->arr[1].time += sety->times;
		addr[sety->arr[1].id]->time = sety->arr[1].time;
		ans_dead[sety->arr[1].id] = 1000000 + addr[sety->arr[1].id]->time;
		if(sety->len > 1){
		    swapnode(addr, &(sety->arr[1]), &(sety->arr[sety->len]));
		    heapify(addr, sety->arr, sety->len);
		}
		sety->len -= 1;
	    }
	    else break;
	}
	if(sety->len == 0)
	    continue;

	// union
	if(setx->len > sety->len)
	    UNION(addr, x, y);
	else
	    UNION(addr, y, x);
    }

    for(int i = 1, t; i <= n; i++){
	t = (ans_dead[i] != 0) ? ans_dead[i]-1000000 : addr[i]->set->times + addr[i]->time;
	printf("%d%c", t, (i == n) ? '\n' : ' ');
    }



    return 0;
}

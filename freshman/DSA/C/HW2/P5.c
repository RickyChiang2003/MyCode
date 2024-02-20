# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

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
    char c, neg;
    while ((c = readchar()) < '-')
        if (c == EOF)
            return 0;
    if(c == '-') neg = 1, *x = 0;
    else neg = 0, *x = c - '0';
    while ((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c - '0';
    if (neg) *x *= -1;
    return 1;
}

typedef struct TRI{
    int a;
    int b;
    int c;
} TRI;

typedef struct pair{
    int a;
    int b;
} pair;

typedef struct CAT{
    int a;
    int c;
    pair* addr_c;  // address of the node of color "array"
    struct CAT* prev;
    struct CAT* next;
} CAT;

typedef struct COLOR{
    int c;
    int head;  // index of the arr's head
    pair* arr;  // arr[0].a means the length of the arr, arr[1].a~arr[arr[0]].a are values, a[i].b = cataddr index of the value
} COLOR;

void swap(int* a, int* b){
    (*a) ^= (*b) ^= (*a) ^= (*b);
}

void TRIheapify_a(int N, TRI* incat, int j, int jj){
    //the real index are j-1 and jj-1
    int done = 0;        
    while(!done){
        if(jj > N)
            done = 1;
        else if(jj+1 > N){
            if(incat[j-1].a < incat[jj-1].a)
                swap(&(incat[j-1].a), &(incat[jj-1].a)), 
                swap(&(incat[j-1].b), &(incat[jj-1].b)),
		swap(&(incat[j-1].c), &(incat[jj-1].c));
            done = 1;
        }
        else{
            if(incat[j-1].a < incat[jj-1].a){
                if(incat[jj-1].a < incat[jj].a)
                    swap(&(incat[j-1].a), &(incat[jj].a)), 
                    swap(&(incat[j-1].b), &(incat[jj].b)), 
		    swap(&(incat[j-1].c), &(incat[jj].c)), j = jj+1;
                else
                    swap(&(incat[j-1].a), &(incat[jj-1].a)), 
                    swap(&(incat[j-1].b), &(incat[jj-1].b)), 
		    swap(&(incat[j-1].c), &(incat[jj-1].c)), j = jj;
            }
            else if(incat[j-1].a < incat[jj].a)
                swap(&(incat[j-1].a), &(incat[jj].a)), 
                swap(&(incat[j-1].b), &(incat[jj].b)),
	        swap(&(incat[j-1].c), &(incat[jj].c)), j = jj+1;
            else
                done = 1;
        }
        jj = j << 1;
    }
}

void TRIheapify_b(int N, TRI* incat, int j, int jj){
    //the real index are j-1 and jj-1
    int done = 0;        
    while(!done){
        if(jj > N)
            done = 1;
        else if(jj+1 > N){
            if(incat[j-1].b < incat[jj-1].b || (incat[j-1].b == incat[jj-1].b && incat[j-1].a < incat[jj-1].a))
                swap(&(incat[j-1].a), &(incat[jj-1].a)), 
                swap(&(incat[j-1].b), &(incat[jj-1].b)), 
                swap(&(incat[j-1].c), &(incat[jj-1].c));
            done = 1;
        }
        else{
            if(incat[j-1].b < incat[jj-1].b || (incat[j-1].b == incat[jj-1].b && incat[j-1].a < incat[jj-1].a)){
                if(incat[jj-1].b < incat[jj].b || (incat[jj-1].b == incat[jj].b && incat[jj-1].a < incat[jj].a))
                    swap(&(incat[j-1].a), &(incat[jj].a)), 
                    swap(&(incat[j-1].b), &(incat[jj].b)), 
                    swap(&(incat[j-1].c), &(incat[jj].c)), j = jj+1;
                else
                    swap(&(incat[j-1].a), &(incat[jj-1].a)), 
                    swap(&(incat[j-1].b), &(incat[jj-1].b)), 
                    swap(&(incat[j-1].c), &(incat[jj-1].c)), j = jj;
            }
            else if(incat[j-1].b < incat[jj].b || (incat[j-1].b == incat[jj].b && incat[j-1].a < incat[jj].a))
                swap(&(incat[j-1].a), &(incat[jj].a)), 
                swap(&(incat[j-1].b), &(incat[jj].b)), 
                swap(&(incat[j-1].c), &(incat[jj].c)), j = jj+1;
            else
                done = 1;
        }
        jj = j << 1;
    }
}

void InitCat(int N, TRI* incat, CAT** cataddr, CAT* catpod, int* MAXa, int* MINa, CAT** MAXcat, CAT** MINcat){
    // O(n) build heap
    for (int i = 0; i < N; i++)
	incat[i].c = i;
    for (int i = N >> 1; i >= 0; i--)
        TRIheapify_a(N, incat, i+1, (i+1) << 1);
    // O(nlogn) heap sort
    for (int i = N-1; i > 0; i--){
//    printf("heap incat :   (i = %d)\n", i); 
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].a);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].b);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].c);printf("\n");	
        swap(&(incat[0].a), &(incat[i].a)), swap(&(incat[0].b), &(incat[i].b)), swap(&(incat[0].c), &(incat[i].c));
//    printf("heap incat swap:\n"); 
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].a);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].b);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].c);printf("\n");	
        TRIheapify_a(i, incat, 1, 2);
    }
    CAT* tmp = NULL, *pre = NULL;
    for(int i = 0; i < N; i++){
        cataddr[incat[i].c] = &(catpod[i]);
        tmp = cataddr[incat[i].c];
	tmp->a = incat[i].a;
        tmp->c = incat[i].b;
        if(i != 0){
            tmp->prev = pre;
            pre->next = tmp;
        }
	pre = tmp;
    }
    *MAXa = incat[N-1].a;
    *MINa = incat[0].a;
    *MAXcat = cataddr[incat[N-1].c];
    *MINcat = cataddr[incat[0].c];
}

int UniqueLength(int N, TRI* incat){
    for (int i = N >> 1; i >= 0; i--){
	TRIheapify_b(N, incat, i+1, (i+1)<<1);
    }
    // O(nlogn) heap sort
    for (int i = N-1; i > 0; i--){
        swap(&(incat[0].a), &(incat[i].a)), swap(&(incat[0].b), &(incat[i].b)), swap(&(incat[0].c), &(incat[i].c));
        TRIheapify_b(i, incat, 1, 2);
//    printf("heap incat after:   (i = %d)\n", i); 
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].a);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].b);printf("\n");	
//    for(int j = 0; j < N; j++)
//	printf("%d ", incat[j].c);printf("\n");	
    }
    int L = 0, prev = -1;
    for (int i = 0; i < N; i++)
        if (incat[i].b != prev)
            prev = incat[i].b, L++;
    return L;
}

void Fillcolor(int N, TRI* incat, CAT** cataddr, COLOR* color, pair* colorpod){

    int prev = -1;
    for (int i = 0, j = 0, k = -1; i < N; i++){
        if(incat[i].b != prev){
            k++;
            prev = incat[i].b;
            colorpod[j].a = 0;
            color[k].arr = &(colorpod[j]), j++;
            color[k].c = incat[i].b;
            color[k].head = 1;
        }
        colorpod[j].a = incat[i].a;
        colorpod[j].b = incat[i].c;
        cataddr[incat[i].c]->addr_c = &(colorpod[j]), j++;
        ((color[k].arr)[0].a)++;
    }

   /* 
    int prev = 0, total = 0, k = -1;
    for (int i = 0, j = 0; i < N; i++){
        if(incat[i].b != prev){
            k++;
            prev = incat[i].b;
            colorpod[j].a = 0;
            color[k].arr = &(colorpod[j]), j++;
	    if(k > 0)
	    ((color[k-1].arr)[0]).a = total;
            color[k].c = incat[i].b;
            color[k].head = 1;
        }
        colorpod[j].a = incat[i].a;
        colorpod[j].b = incat[i].c;
        cataddr[incat[i].c]->addr_c = &(colorpod[j]), j++;
	total++;
    }
    ((color[k].arr)[0]).a = total;
  */
}

int FindColor(int c, COLOR* color, int SIZE_c){
    int low = 0, high = SIZE_c-1, index = 0;
    while (low <= high) {
        index = (low + high) >> 1;
        if (c < color[index].c)
            high = index - 1;
        else if (c > color[index].c)
            low = index + 1;
        else
            break;
    }
    if(low > high)
	return -1;
    return index;
}

void ColorBS(pair* a, int tar, int index[2]){
    int done = 0, low = 1, high = a[0].a, mid = 0, near = 0;
    if(a[low].a > tar && a[high].a < tar){
	index[0] = high, index[1] = low;
	return;	
    }
    printf("ColorBS: tar = %d, a[0].a = %d\n", tar, a[0].a);
    while(near < 2 && !done){
        mid = (low + high) >> 1;
	printf("low = %d, high = %d, mid = %d, %d,%d,%d\n", low, high, mid, a[low].a, a[high].a, a[mid].a);
        if(tar == a[mid].a){
            index[0] = index[1] = mid;
            done = 1;
        }
        if(tar == a[low].a){
            index[0] = index[1] = low;
            done = 1;
        }
        if(tar == a[high].a){
            index[0] = index[1] = high;
            done = 1;
        }
        if(a[mid].a <= a[high].a){
            if(tar > a[mid].a && tar <= a[high].a)
                low = mid;
            else
                high = mid;
        }
        else{
            if(tar >= a[low].a && tar < a[mid].a)
                high = mid;
            else
                low = mid;
        }
	if(low >= high-1) near++;
	printf("low = %d, high = %d, mid = %d\n", low, high, mid);
    }
    if(!done){
	int midup = (mid == a[0].a) ? 1 : mid+1;
	int midupup = (midup == a[0].a) ? 1 : midup+1;
	int midlo = (mid == 1) ? a[0].a : mid-1;
	int midlolo = (midlo == 1) ? a[0].a : midlo-1;

	if(a[midlolo].a < tar && a[midlo].a > tar)
	    index[0] = midlolo, index[1] = midlo;
	else if(a[midlo].a < tar && a[mid].a > tar)
	    index[0] = midlo, index[1] = mid;
	else if(a[mid].a < tar && a[midup].a > tar)
	    index[0] = mid, index[1] = midup;
	else if(a[midup].a < tar && a[midupup].a > tar)
	    index[0] = midup, index[1] = midupup;
	else
	    printf("You fucked up ad ColorBS, midlolo=%d,midlo=%d,mid=%d,midup=%d,midupup=%d\n", midlolo,midlo,mid,midup,midupup), exit(-1);
	/*
        if(a[mid].a > tar){
            if(a[midlo].a < tar || (a[midlo].a > tar && a[midlolo].a > a[midlo].a))
		index[0] = midlo, index[1] = mid;
	    else
		index[0] = midlolo, index[1] = midlo;
	}
        else{
            if(a[midup].a > tar || (a[midup].a < tar && a[midupup].a < a[midup].a))
		index[0] = mid, index[1] = midup;
	    else
		index[0] = midup, index[1] = midupup;
	}
	*/
    }
}

int main(){
    FILE* fptr = fopen("P5test.out", "r");
    int AC = 1;

    int N = 0, M = 0;
    int MAXa = 0, MINa = 0;
    CAT *MAXcat = NULL, *MINcat = NULL;
    ReadInt(&N);ReadInt(&M);
    TRI* incat = calloc(N, sizeof(*incat));
    CAT** cataddr = calloc(N, sizeof(*cataddr)); CAT* catpod = calloc(N, sizeof(*catpod));

    for (int i = 0; i < N; i++) ReadInt(&(incat[i].a));
    for (int i = 0; i < N; i++) ReadInt(&(incat[i].b));
    InitCat(N, incat, cataddr, catpod, &MAXa, &MINa, &MAXcat, &MINcat);  // and also find max and min
    int SIZE_c = UniqueLength(N, incat);

    COLOR* color = calloc(SIZE_c, sizeof(*color)); pair *colorpod = calloc(3 * N, sizeof(*colorpod));
    Fillcolor(N, incat, cataddr, color, colorpod);
    // useful:  cataddr, color
    for (int cnt = 0, type; cnt < M; cnt++){
        int c, l, r, k, s, t, index, head, tail;
        pair *a = NULL;
        ReadInt(&type);
        switch(type){
            case  1:{  // question
                ReadInt(&c); ReadInt(&l); ReadInt(&r);
                index = FindColor(c, color, SIZE_c);
                a = color[index].arr;
		printf("question c = %d, l = %d, r = %d, index = %d\n", c, l, r, index);
		if(index == 13){
		    printf("a: ");
		    for(int j = 1; j <= a[0].a; j++)
			printf("%d ", a[j].a);printf("\n");
		    printf("b: ");
		    for(int j = 1; j <= a[0].a; j++)
			printf("%d ", a[j].b);printf("\n");
		}
		if(index == -1){
		    printf("0\n");
		    int out;
		    fscanf(fptr, "%d", &out);
		    if(out != 0) AC = 0, printf("    wrong0 cnt=%d   index=-1\n", cnt), exit(-1);
		    break;
		}
                head = color[index].head;
                tail = (head == 1) ? a[0].a : head - 1;
                if(a[0].a == 0){
                    printf("0\n");
		    int out;
		    fscanf(fptr, "%d", &out);
		    if(out != 0) AC = 0, printf("    wrong0 cnt=%d   wwwwwwwwwwww\n", cnt), exit(-1);

                    break;
                }
                else if (r < a[head].a || l > a[tail].a){
                    printf("0\n");
		    int out;
		    fscanf(fptr, "%d", &out);
		    if(out != 0) AC = 0, printf("    wrong0 cnt=%d   r<%d,l>%d\n", cnt, a[head].a, a[tail].a), exit(-1);
                    break;
                }
                else {
                    int index1[2], index2[2];  // lower bound and upper bound
                    if (l <= a[head].a) index1[0] = index1[1] = head;
                    else {
			ColorBS(a, l, index1);  // arr, target, low, high, index arr
		    }
                    if (r >= a[tail].a) index2[0] = index2[1] = tail;
                    else ColorBS(a, r, index2);
//			printf("c = %d, l = %d, r = %d, index1 = %d,%d, index2 = %d,%d\n", c, l, r, index1[0], index1[1], index2[0], index2[1]);
		    int ans;
		    if(index1[1] >= index1[0])  
			ans = (index1[0]<=index2[0]) ? index2[0]-index1[1]+1 : index2[0]-index1[1]+1+a[0].a;
		    else
			ans = (index1[0]<=index2[0]) ? 0 : index2[0];
		    printf("ans: ");
		    //                a = color[0].arr;
		    printf("%d\n", ans);
		    int out;
		    fscanf(fptr, "%d", &out);
		    if(out != ans) AC = 0, printf("    wrong  cnt=%d\n", cnt),
			printf("c = %d, l = %d, r = %d, index1 = %d,%d, index2 = %d,%d\n", c, l, r, index1[0], index1[1], index2[0], index2[1]), exit(-1);
                }
                break;
            }
            case  2:{  // greedy
                ReadInt(&k);
		printf("greedy k = %d\n", k);
                CAT* cat1 = cataddr[k];
                CAT* cat2 = cataddr[k]->next;
		printf("cat1->a = %d, cat2->a = %d\n ", cat1->a, cat2->a);
                //swap address of color array
		//printf("cat1->addr_c->a = %d, b = %d; cat2->addr_c->a = %d, b = %d\n", 
		 //   cat1->addr_c->a, cat1->addr_c->b, cat2->addr_c->a, cat2->addr_c->b);
                if(cat1->c != cat2->c)
                    swap(&(cat1->addr_c->a), &(cat2->addr_c->a));
                else { 
                    swap(&(cat1->addr_c->b), &(cat2->addr_c->b));
                    pair* tmp = cat1->addr_c; cat1->addr_c = cat2->addr_c; cat2->addr_c = tmp; 
                }
//		printf("cat1->addr_c->a = %d, b = %d; cat2->addr_c->a = %d, b = %d\n", 
//			cat1->addr_c->a, cat1->addr_c->b, cat2->addr_c->a, cat2->addr_c->b);
		
                //swap a
                swap(&(cat1->a), &(cat2->a));
                //swap linked nodes
                if     (cat1->prev != NULL && cat2->next != NULL){
                    cat1->prev->next = cat2;
                    cat2->next->prev = cat1;
                    CAT *tmp1 = cat1->prev;
                    CAT *tmp2 = cat2->next;
                    cat1->prev = cat2;
                    cat2->next = cat1;
                    cat1->next = tmp2;
                    cat2->prev = tmp1;
                }
                else if(cat1->prev == NULL && cat2->next != NULL){
                    cat2->next->prev = cat1;
                    cat1->next = cat2->next;
                    cat2->next = cat1;
                    cat1->prev = cat2;
                    cat2->prev = NULL;
		    MINcat = cat2;
                }
                else if(cat1->prev != NULL && cat2->next == NULL){
                    cat1->prev->next = cat2;
                    cat2->prev = cat1->prev;
                    cat1->prev = cat2;
                    cat2->next = cat1;
                    cat1->next = NULL;
		    MAXcat = cat1;		    
                }
                else{  // which means there are only two sad cats
                    cat1->prev = cat2;
                    cat2->next = cat1;
                    cat1->next = cat2->prev = NULL;
		    MINcat = cat2;
		    MAXcat = cat1;
                }
                a = color[13].arr;
		    printf("a: ");
		    for(int j = 1; j <= a[0].a; j++)
		    printf("%d ", a[j].a);printf("\n");
		    printf("b: ");
		    for(int j = 1; j <= a[0].a; j++)
			printf("%d ", a[j].b);printf("\n");
		CAT* catflag = MINcat;int tmpcnt = 0;
		while(catflag != NULL && tmpcnt < 3000){
		//    printf("%d ", catflag->a);
		    catflag = catflag->next;
		    tmpcnt++;
		} printf("\n"); tmpcnt = 0, catflag = MINcat;
		while(catflag != NULL && tmpcnt < 3000){
		//    printf("%d ", catflag->c);
		    catflag = catflag->next;
		    tmpcnt++;
		} printf("\n");
		if(tmpcnt >= 3000) printf("cycle here\n"), exit(-1);
		else printf("no cycle\n");
                break;
            }
            case  3:{  // fairy
                ReadInt(&c); ReadInt(&s); ReadInt(&t);
                //upgrade color
                index = FindColor(c, color, SIZE_c);
                a = color[index].arr;
		printf("fairy c = %d, s = %d, t = %d, index = %d\n", c, s, t, index);
//		printf("a: ");
//		for(int i = 1; i <= a[0].a; i++)
//		    printf("%d ", a[i].a);printf("\n");
//		printf("b: ");
//		for(int i = 1; i <= a[0].a; i++)
//		    printf("%d ", a[i].b);printf("\n");
                head = color[index].head;
                tail = (head == 1) ? a[0].a : head - 1;
		printf("c = %d, head = %d, tail = %d, a[0].a = %d\n", c, head, tail, a[0].a);
                CAT *cat1 = NULL, *cat2 = NULL;
                if(t) MAXa++;
                else  MINa--;
                if (s && t){
                    a[tail].a = MAXa;
                    cat1 = cataddr[a[tail].b];
                    cat1->a = MAXa;
                    cat2 = MAXcat;
                    MAXcat = cat1;
		    if(MINcat == cat1) MINcat = cat1->next, MINa = cat1->next->a;
                }
                else if(!s && !t){
                    a[head].a = MINa;
                    cat1 = cataddr[a[head].b];
                    cat1->a = MINa;
                    cat2 = MINcat;
                    MINcat = cat1;
		    if(MAXcat == cat1) MAXcat = cat1->prev, MAXa = cat1->prev->a;;
                }
                else if(s && !t){
                    a[tail].a = MINa;
                    color[index].head = tail;
                    cat1 = cataddr[a[tail].b];
                    cat1->a = MINa;
                    cat2 = MINcat;
                    MINcat = cat1;
		    if(MAXcat == cat1) MAXcat = cat1->prev, MAXa = cat1->prev->a;
                }
                else{
                    a[head].a = MAXa;
                    color[index].head = (head == a[0].a) ? 1 : head + 1;
                    cat1 = cataddr[a[head].b];
                    cat1->a = MAXa;
                    cat2 = MAXcat;
                    MAXcat = cat1;
		    if(MINcat == cat1) MINcat = cat1->next, MINa = cat1->next->a;
                }
		printf("c = %d, head = %d, tail =   , a[0].a = %d (after fairy)\n", c, color[index].head, a[0].a);
		printf("before upgrade cataddr\n");
                //upgrade cataddr next and prev direction
                if(cat1 != cat2){
		    if(t){
			if(cat1->next != NULL) cat1->next->prev = cat1->prev;
			if(cat1->prev != NULL) cat1->prev->next = cat1->next;
			cat1->prev = cat2;
			cat2->next = cat1;
			cat1->next = NULL;
		    }
		    else{
			if(cat1->next != NULL) cat1->next->prev = cat1->prev;
			if(cat1->prev != NULL) cat1->prev->next = cat1->next;
			cat1->next = cat2;
			cat2->prev = cat1;
			cat1->prev = NULL;
		    }
		}
		printf("fairy c = %d (after)\n", c);
		if(index == 13){
		    printf("a: ");
		    for(int i = 1; i <= a[0].a; i++)
			printf("%d ", a[i].a);printf("\n");
		    printf("b: ");
		    for(int i = 1; i <= a[0].a; i++)
			printf("%d ", a[i].b);printf("\n");
		}
		CAT* catflag = MINcat;int tmpcnt = 0;
		while(catflag != NULL && tmpcnt < 3000){
		//    printf("%d ", catflag->a);
		    catflag = catflag->next;
		    tmpcnt++;
		} printf("\n"); tmpcnt = 0, catflag = MINcat;
		while(catflag != NULL && tmpcnt < 3000){
		//    printf("%d ", catflag->c);
		    catflag = catflag->next;
		    tmpcnt++;
		} printf("\n");
		if(tmpcnt >= 3000) printf("cycle here\n"), exit(-1);
		else printf("no cycle\n");
                break;
            }
            default:{
                printf("You fucked up, haiyaa.\n");
                break;
            }
        }
    }
    if(AC)
    printf("AC\n");
    else
	printf("WA\n");
    fclose(fptr);

        return 0;
}

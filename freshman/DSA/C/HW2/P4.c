# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
// N*M <= 1e6


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
    while ((c = readchar()) < '-'){
        if (c == EOF)
            return 0;
    }
    neg = (c == '-') ? -1 : 1;
    *x = (neg == 1) ? c - '0' : 0;
    while ((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c - '0';
    *x *= neg;
    return 1;
}


typedef struct node{
    int num;
    int melon;
    int price;
    struct node* parent;
} node;


void swap(int* a, int* b){
    (*a) ^= (*b) ^= (*a) ^= (*b);
}

int** Dynamic2dArr(int a, int b){
    int** arr = calloc(a, sizeof(*arr));  // 1 to M, cost[][0] means the length of the cost
    if(b > 0)
	for(int i = 0; i < a; i++)
	    arr[i] = calloc(b, sizeof(*(arr[i])));
    return arr;
}

void BuildTree(int N, node* nodeaddr[], int** subnode){
    int input;
    int* numofsubnode = calloc(N, sizeof(int));

    for(int i = 0; i < N; i++){
	nodeaddr[i] = calloc(1, sizeof(node));
	nodeaddr[i]->num = i;
    }
    for(int i = 1; i < N; i++){
	ReadInt(&input);
	numofsubnode[input-1]++;
	nodeaddr[i]->parent = nodeaddr[input-1];
    }
    for(int i = 0; i < N; i++)
	subnode[i] = calloc(numofsubnode[i]+1, sizeof(int));
	
    for(int i = 0; i < N; i++){
	node* tmp = nodeaddr[i];
	if(tmp->parent != NULL){
	    subnode[tmp->parent->num][0]++;
	    subnode[tmp->parent->num][subnode[tmp->parent->num][0]] = i;
	}
    }
}

int eachmelon(int com, node** nodeaddr, int** subnode){
    int sum = 1;
    for(int i = 1; i <= subnode[com][0]; i++)
	sum += eachmelon(subnode[com][i], nodeaddr, subnode);
    nodeaddr[com]->melon = sum;
    return sum;    
}

int findprice(int M, int cost[], int due[], int i){
    cost[0]++;
    if(i != cost[0])
	swap(cost+i, cost+cost[0]), swap(due+i, due+cost[0]);
    int isheap = 0, index = cost[0];
    while(!isheap && index != 1){
	int index_p = index >> 1;
	if(cost[index_p] > cost[index]){
	    swap(cost+index_p, cost+index), swap(due+index_p, due+index);
	    index = index_p;
	}
	else
	    isheap = 1;
    }

//    printf("cost[1] = %d, due[1] = %d, i = %d\n",cost[1], due[1], i);
//    for(int j = 1; j <= cost[0]; j++) printf("%d%c", cost[j], j == cost[0] ? '\n' : ' ');
    if(due[1] >= i) return cost[1];

    while(due[1] < i){
//	printf("overdue\n");
	swap(cost+1, cost+cost[0]), swap(due+1, due+cost[0]);
	cost[0]--;
	int done = 0, j = 1, jj = 2;
	while(!done){
	    if(jj > cost[0])
		done = 1;
	    else if(jj + 1 > cost[0]){
		if(cost[j] > cost[jj])
		    swap(cost+j, cost+jj), swap(due+j, due+jj);
		done = 1;
	    }
	    else{
		if (cost[j] > cost[jj]){
		    if (cost[jj] > cost[jj+1]) swap(cost+j, cost+jj+1), swap(due+j, due+jj+1), j = jj+1;
		    else swap(cost+j, cost+jj), swap(due+j, due+jj), j = jj;
		}
		else if (cost[j] > cost[jj+1])
		    swap(cost+j, cost+jj+1), swap(due+j, due+jj+1), j = jj+1;
		else
		    done = 1;
//		printf("j = %d\n", j);
		jj = j << 1;
//    for(int j1 = 1; j1 <= cost[0]; j1++) printf("%d%c", cost[j1], j1 == cost[0] ? '\n' : ' ');
//    if(done) printf("done\n");
	    }
	}
    }
//    printf("cost[1] = %d, due[1] = %d, i = %d\n",cost[1], due[1], i);
//    printf("after overdue\n");
//    for(int j = 1; j <= cost[0]; j++) printf("%d%c", cost[j], j == cost[0] ? '\n' : ' ');
    return cost[1];
}

int totalprice(int M, int** cost, int** due, int com, node** nodeaddr, int** subnode, int day, int MONEY){
//    printf("totalprice(com = %d)   subnode[%d][%d] = %d\n", com, com, 0, subnode[com][0]);
    int sum = findprice(M, cost[com], due[com], day);
//    printf("initial price of com %d = %d\n", com, sum);
    for(int i = 1; i <= subnode[com][0]; i++){
	int t = totalprice(M, cost, due, subnode[com][i], nodeaddr, subnode, day, MONEY);
	if(sum <= MONEY) sum += t;
    }
//    printf("final price of com %d = %d\n", com, sum);
    if(sum > MONEY) sum = MONEY+1;
    nodeaddr[com]->price = sum;
    return sum;
}


int main(){
    int N, M, MONEY;  // comp, event, money
    scanf("%d%d%d", &N, &M, &MONEY);
    int**  cost     = Dynamic2dArr(N, M+1);  // cost[i][j]    for company i and j day. cost[i][0] = length
    int**  due      = Dynamic2dArr(N, M+1);  // due[i][j]     for company i and j day. 
    int**  subnode  = Dynamic2dArr(N, 0);    // subnode[i][j] for company i and its jth subnode
    node** nodeaddr = calloc(N, sizeof(*nodeaddr));
    BuildTree(N, nodeaddr, subnode);
    eachmelon(0, nodeaddr, subnode);

    for(int i = 1; i <= M; i++)
	for(int j = 0; j < N; j++){
	    int tmp;
	    ReadInt(&cost[j][i]);ReadInt(&tmp);
	    //scanf("%d%d", &cost[j][i], &tmp);
	    due[j][i] = i + tmp;
	}

//    FILE* fptr = fopen("P4testcode_output.out","r");
    for(int i = 1; i <= M; i++){
	int max_melon = 0;
	totalprice(M, cost, due, 0, nodeaddr, subnode, i, MONEY);
	for(int j = 0; j < N; j++){
	    if(nodeaddr[j]->price <= MONEY && nodeaddr[j]->melon > max_melon)
		max_melon = nodeaddr[j]->melon;
//	    printf("nodeaddr[%d]->price = %d, ownprice = %d, ->melon = %d, melon = %d\n", j, nodeaddr[j]->price, (j == N-1) ? nodeaddr[j]->price : (nodeaddr[j]->price - nodeaddr[j+1]->price), nodeaddr[j]->melon, max_melon);
	}
//	int c;
//	fscanf(fptr, "%d", &c);
//	if(c != max_melon) printf("WA at i = %d\n", i), exit(-1);
	printf("%d\n", max_melon);
    }
//    fclose(fptr);
//    printf("AC\n");

    return 0;
}


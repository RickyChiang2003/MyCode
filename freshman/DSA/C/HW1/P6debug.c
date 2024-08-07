
# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
# define SIZE 1001  // 1001
# define SIZE2 1000001  //1000001


typedef struct bakery{
    struct bakery* next[4];  // init by up,right,down,left:0,1,2,3
    int d[4];                // distance for 0,1,2,3
    int u;
    int t;
} bakery;

    bakery* B[SIZE][SIZE] = {0};
    bakery* rankaddr[SIZE2] = {0};
    int closed[SIZE2] = {0}, close_index = 0;
    int visited[SIZE2] = {0}, visit_index = 0;  // store the visited bakery's rank
    int total[SIZE2] = {0}, total_index = 0;

void init_B (int n, int m/*, bakery* B[SIZE][SIZE], bakery* rankaddr[SIZE2]*/);
void train  (int n, int m/*, bakery* B[SIZE][SIZE], bakery* rankaddr[SIZE2]*/, int k);
void race   (bakery** first/*, bakery* rankaddr[SIZE2]*/, int k);
void adjust (int n, int m/*, bakery* B[SIZE][SIZE]*/);
void hide   (bakery* flag);
void putback(bakery* flag);
void go_forward(bakery* flag[2], int nextdir[2]);
void turn_right(int nextdir[2]);
void swap_line(bakery** first, bakery* flag[2], int nextdir[2], int l0);
void output(int n, int m/*, bakery* B[SIZE][SIZE]*/);

int main(){
    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    init_B(n, m);

    int T1 = 0, R = 0, T2 = 0;
    scanf("%d%d%d", &T1, &R, &T2);
    printf("start\n");
    train (n, m, T1);
    printf("train1 ok\n");
    race  (&(B[0][0]), R); 
    printf("race ok\n");
    adjust(n, m);
    printf("adjust ok\n");
    train (n, m, T2);
    printf("train2 ok\n");
    output(n, m);

    return 0;
}



void init_B(int n, int m/*, bakery* B[SIZE][SIZE], bakery* rankaddr[SIZE2]*/){
    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
	    int tmp_u = 0;
	    scanf("%d", &tmp_u);
	    bakery* node = (bakery*)calloc(1, sizeof(bakery));
	    for(int l = 0; l < 4; l++) node->d[l] = 1, node->next[l] = NULL;
	    if(i > 0) {node->next[0] = B[i-1][j], B[i-1][j]->next[2] = node;}
	    if(j > 0) {node->next[3] = B[i][j-1], B[i][j-1]->next[1] = node;}
	    B[i][j] = rankaddr[tmp_u] = node;
	    node->u = tmp_u;
	}
    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
	    int tmp_t = 0;
	    scanf("%d", &tmp_t);
	    B[i][j]->t = tmp_t;
	}
}


void train (int n, int m/*, bakery* B[SIZE][SIZE], bakery* rankaddr[SIZE2]*/, int k){
    int r = 0, l = 0, s = 0;

    for(int i = 0; i < k; i++){  // each day in a T
	scanf("%d%d%d", &r, &l, &s);
	bakery* flag = rankaddr[r];

	for(int cnt = 0;cnt < l; cnt++){  // each turn in a day
//	    printf("cnt = %d\n", cnt);
	    int nextdir = -1, tmp_d = SIZE, tmp_r = SIZE2+1;
	    for(int j = 0; j < 4; j++) {
		if(flag->next[j] != NULL){
//		    printf("flag->d[%d] = %d, tmp_d = %d, flag->next[%d]->u = %d, flag->next[%d]->t = %d, tmp_r = %d\n", j, flag->d[j], tmp_d, j, flag->next[j]->u, j, flag->next[j]->t, tmp_r);
		    if(flag->d[j] < tmp_d) 
			nextdir = j, tmp_d = flag->d[j], tmp_r = flag->next[j]->u;
		    else if(flag->d[j] == tmp_d && flag->next[j]->u < tmp_r) 
			nextdir = j, tmp_r = flag->next[j]->u;
		}
	    }
	    if(nextdir == -1){
		if(flag->t == 0){  // check if the last bakery is closed
		    closed[close_index] = flag->u, close_index++, total[total_index] = 0;
		    hide(flag);
		    total_index++;
		}
		break;
	    }
		     // return if no bakery
//	    printf("flag->u = %d, flag->next[%d]->u = %d\n",flag->u,nextdir,flag->next[nextdir]->u);

	    if(flag->t == 0){
		closed[close_index] = flag->u, close_index++, total[total_index] = 0;  // hide prev flag
	    }
	    else visited[visit_index] = flag->u, visit_index++, total[total_index] = 1;
	    total_index++;
	    hide(flag);
	    flag = flag->next[nextdir];  // go to next bakery
	    flag->t = (flag->t < s) ? 0 : flag->t - s;  // eat toast
	    if(flag->t == 0 && cnt == l-1){  // check if the last bakery is closed
		closed[close_index] = flag->u, close_index++, total[total_index] = 0;
		hide(flag);
		total_index++;
	    if(flag->u == 244219)
	        if(rankaddr[244219]->next[0]->u == 7358)
	    printf("train day = %d\n", i);
	    }
//	    output(n, m, B);
	}
	if(rankaddr[244219]->next[0]->u != 7358)
	    printf("u = %d, train day = %d\n",rankaddr[244219]->next[0]->u, i);
	int ct = close_index;
	for(; total_index > 0; total_index--){
	    if(total[total_index-1] == 0)
		putback(rankaddr[closed[(ct = ct-1)]]);
	    else {
		putback(rankaddr[visited[(visit_index = visit_index-1)]]);
	        if(rankaddr[244219]->next[0]->u != 7358)
		    printf("244219->next[0]->u = %d\n", rankaddr[244219]->next[0]->u);
	    }
	}
	
	for(; ct < close_index; ct++)
	    hide(rankaddr[closed[ct]]);
//	if(rankaddr[244219]->next[0]->u != 7358)
//	    printf("nextu = %d, 7358->next[2]->u = %d, train day = %d\n",rankaddr[7358]->next[2]->u, rankaddr[7358]->next[2]->u, i), exit(-1);
//	for(; visit_index > 0; visit_index--)  // put visited back
//	    putback(rankaddr[visited[visit_index-1]]);  // visit_index is (real-index)+1
    }
    for(; close_index > 0; close_index--)  // put closed back
        putback(rankaddr[closed[close_index-1]]);  // close_index is (real-index)+1
}


void race(bakery ** first/*, bakery* rankaddr[SIZE2]*/, int k){
    int r, l;
    for(int i = 0; i < k; i++){
	scanf("%d%d", &r, &l);
	bakery* flag[2] = {rankaddr[r], rankaddr[r]};
	int nextdir[2] = {0, 2}, l0 = l >> 1;  // dir pointing the flags' next, useful element
	for(int j = 0; j < l0; j++)
	    go_forward(flag, nextdir);
	    if(flag[0]->next[nextdir[0]] != NULL) printf("race day = %d, flag[0]->u = %d, flag[1]->u = %d, l0 = %d, flag[0]->next[%d]->u = %d\n", i, flag[0]->u, flag[1]->u, l0, nextdir[0], flag[0]->next[nextdir[0]]->u);
	if(l0 > 0){
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0);
	    printf("race day = %d, flag[0]->u = %d, flag[0]->t = %d, flag[1]->u = %d, flag[1]->t = %d, l0 = %d\n", i, flag[0]->u, flag[0]->t, flag[1]->u, flag[1]->t, l0);
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0 << 1);
	    printf("race day = %d, flag[0]->u = %d, flag[0]->t = %d, flag[1]->u = %d, flag[1]->t = %d, l0 = %d\n", i, flag[0]->u, flag[0]->t, flag[1]->u, flag[1]->t, l0);
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0-1);
	    printf("race day = %d, flag[0]->u = %d, flag[0]->t = %d, flag[1]->u = %d, flag[1]->t = %d, l0 = %d\n", i, flag[0]->u, flag[0]->t, flag[1]->u, flag[1]->t, l0);
	}
    }
}


void adjust (int n, int m/*, bakery* B[SIZE][SIZE]*/){
    // by the "first" in race, B[0][0] should not been changed
    int dirpair[3][4] = {{0,0,0,0},{2,3,0,1},{1,2,3,0}};
    bakery* f = B[0][0];
    if(f->next[0] != NULL || f->next[3] != NULL){
	if(f->next[1] == NULL && f->next[2] == NULL)
	    f->next[1] = f->next[3], f->next[2] = f->next[0];
	else if(f->next[2] == NULL && f->next[3] == NULL)
	    f->next[2] = f->next[1], f->next[1] = f->next[0];
	else if(f->next[0] == NULL && f->next[1] == NULL)
	    f->next[1] = f->next[2], f->next[2] = f->next[3];
	f->next[0] = f->next[3] = NULL;
    }
    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++){
	    int nextdir = 1;
	    bakery* prev = NULL;
//	    printf("i = %d, j = %d\n",i,j);
	    if(i == 0 && j == 0) continue;
	    if(i > 0 && j == 0) f = B[i-1][j]->next[2], nextdir = 2, prev = B[i-1][j];
	    else f = B[i][j-1]->next[1], nextdir = 1, prev = B[i][j-1];
//	    printf("prev->t = %d, f->t = %d\n", prev->t, f->t);
	    int dir = -1;
	    for(int m = 0; dir == -1; m++){
//		if(f->next[m] != NULL) printf("f->next[m]->u = %d\n", f->next[m]->u);
		if(f->next[m] == prev) dir = dirpair[nextdir][m];
//		if(m > 5) exit(-1);
	    }
//		printf("nextdir = %d, dir=%d", nextdir, dir);
	    dir--;
	    if(dir != 0){
		dir = (dir < 0) ? dir+4 : dir;
		bakery* tmp = NULL;		
		if(dir == 1)
		    tmp = f->next[1], f->next[1] = f->next[2], f->next[2] = f->next[3],
		    f->next[3] = f->next[0], f->next[0] = tmp;
		else if(dir == 2)
		    tmp = f->next[1], f->next[1] = f->next[3], f->next[3] = tmp,
		    tmp = f->next[0], f->next[0] = f->next[2], f->next[2] = tmp;
		else if(dir == 3)
		    tmp = f->next[1], f->next[1] = f->next[0], f->next[0] = f->next[3], 
		    f->next[3] = f->next[2], f->next[2] = tmp;
	    }
//	    printf("i = %d, j = %d\n",i,j);
	    B[i][j] = f;
	}

    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	    if(B[i][j]->t == 0) hide(B[i][j]);
}


void hide(bakery* flag){
    int tmp[4] = {2,3,0,1};
    for(int i = 0; i < 4; i++){
        if(flag->next[i] != NULL){ 
	    if(flag->u == 7358)
    	        printf("hide:7358   flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d\n", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u);
	    if(flag->u == 122725)
    	        printf("hide:122725 flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d, t = %d\n", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u, flag->t);
	    if(flag->u == 202294)
    	        printf("hide:202294 flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d\n", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u);
	    flag->next[i]->next[tmp[i]] = flag->next[tmp[i]];
	    flag->next[i]->d[tmp[i]] = flag->d[i] + flag->d[tmp[i]];
	    
	    if(flag->u == 7358)
    	        printf("hide:7358   flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	    if(flag->u == 122725)
    	        printf("hide:122725 flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	    if(flag->u == 202294)
    	        printf("hide:202294 flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	}
    }
    if(flag->u == 7358 && rankaddr[244219]->next[0]->u != 7358)
        printf("7358 is hided\n");
}


void putback(bakery* flag){
    int tmp[4] = {2,3,0,1};
    for(int i = 0; i < 4; i++){
	if(flag->next[i] != NULL){
	    if(flag->u == 7358)
    	        printf("putback7358: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d\n", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u);
	    if(flag->u == 122725)
    	        printf("putback122725: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d\n", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u);
	    if(flag->u == 202294)
    	        printf("putback202294: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d\n ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u);
	    if(flag->next[i]->next[tmp[i]] != flag){
	        if(flag->next[i]->next[tmp[i]] != NULL){
		    flag->next[i]->next[tmp[i]]->next[i] = flag;
	            flag->next[i]->next[tmp[i]]->d[i] = flag->d[tmp[i]];
		}
	        flag->next[i]->next[tmp[i]] = flag;
	        flag->next[i]->d[tmp[i]] = flag->d[i];
	    }
	    if(flag->u == 7358)
    	        printf("putback7358: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	    if(flag->u == 122725)
    	        printf("putback122725: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	    if(flag->u == 202294)
    	        printf("putback202294: flag->next[%d]->next[%d], flag->next[%d]->u = %d, 244219->next[0]->u = %d ", i, tmp[i], i, flag->next[i]->u, rankaddr[244219]->next[0]->u),printf("after\n");
	}
    }
}


void go_forward(bakery* flag[2], int nextdir[2]){
    int dirpair[4] = {2, 3, 0, 1};
    for(int i = 0; i < 2; i++){
	bakery* prev = flag[i];
	flag[i] = flag[i]->next[nextdir[i]];
	for(int j = 0; j < 4; j++)
	    if(flag[i]->next[j] == prev){ 
		nextdir[i] = dirpair[j];
		break;
	    }
    }
}


void turn_right(int nextdir[2]){
    int dirpair[4] = {1, 2, 3, 0};
    nextdir[0] = dirpair[nextdir[0]];
    nextdir[1] = dirpair[nextdir[1]];
}


void swap_line(bakery** first, bakery* flag[2], int nextdir[2], int l0){
    int leftdir[4] = {3, 0, 1, 2};  // the left one
    if(*first == flag[0]) *first = flag[1];
    else if (*first == flag[1])	*first = flag[0];

    int dirbound[2] = {-1,-1};
    for(int m = 0; m < 2; m++)
	if(flag[m]->next[leftdir[nextdir[m]]] != NULL)
	    for(int j = 0; dirbound[m] == -1; j++)
		if(flag[m]->next[leftdir[nextdir[m]]]->next[j] == flag[m]){
		    dirbound[m] = j;
		    flag[m]->next[leftdir[nextdir[m]]]->next[j] = flag[(!m)];
		    flag[m]->next[leftdir[nextdir[m]]]->d[j] = flag[(!m)]->d[leftdir[nextdir[(!m)]]];
		}
    bakery* tmp = flag[0]->next[leftdir[nextdir[0]]];
    flag[0]->next[leftdir[nextdir[0]]] = flag[1]->next[leftdir[nextdir[1]]];
    flag[1]->next[leftdir[nextdir[1]]] = tmp;
    int tmp_d = flag[0]->d[leftdir[nextdir[0]]];
    flag[0]->d[leftdir[nextdir[0]]] = flag[1]->d[leftdir[nextdir[1]]];
    flag[1]->d[leftdir[nextdir[1]]] = tmp_d;

//        printf("swaping: flag[0]->u = %d, flag[1]->u = %d, flag[1]->next[leftdir[nextdir[1]]]->u = %d\n", flag[0]->u, flag[1]->u,flag[1]->next[leftdir[nextdir[1]]]->u);
    for(int i = 0; i < l0; i++){
	go_forward(flag, nextdir);
	printf("swaping: i = %d\n", i);
	dirbound[0] = dirbound[1] = -1;
	for(int m = 0; m < 2; m++)
	    if(flag[m]->next[leftdir[nextdir[m]]] != NULL)
		for(int j = 0; dirbound[m] == -1; j++){
		    if(j == 4) printf("flag[%d]->u = %d, flag[%d]->next[%d]->u = %d, t = %d\nbreak\n", m, flag[m]->u, m, leftdir[nextdir[m]], flag[m]->next[leftdir[nextdir[m]]]->u, flag[m]->next[leftdir[nextdir[m]]]->t), exit(-1);
		    if(flag[m]->next[leftdir[nextdir[m]]]->next[j] == flag[m]){
			dirbound[m] = j;
			flag[m]->next[leftdir[nextdir[m]]]->next[j] = flag[(!m)];
		        flag[m]->next[leftdir[nextdir[m]]]->d[j] = flag[(!m)]->d[leftdir[nextdir[(!m)]]];
		    }
		}
		    
	tmp = flag[0]->next[leftdir[nextdir[0]]];
	flag[0]->next[leftdir[nextdir[0]]] = flag[1]->next[leftdir[nextdir[1]]];
	flag[1]->next[leftdir[nextdir[1]]] = tmp;
        int tmp_d = flag[0]->d[leftdir[nextdir[0]]];
        flag[0]->d[leftdir[nextdir[0]]] = flag[1]->d[leftdir[nextdir[1]]];
        flag[1]->d[leftdir[nextdir[1]]] = tmp_d;
	printf("swaping: i = %d done\n", i);
    }
}


void output(int n, int m/*, bakery* B[SIZE][SIZE]*/){
    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	    printf("%d%c", B[i][j]->t, (j == m-1) ? '\n':' ');
}

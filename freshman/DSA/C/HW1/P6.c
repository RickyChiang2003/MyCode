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

void init_B (int n, int m);
void train  (int n, int m, int k);
void race   (bakery** first, int k);
void adjust (int n, int m);
void hide   (bakery* flag);
void putback(bakery* flag);
void go_forward(bakery* flag[2], int nextdir[2]);
void turn_right(int nextdir[2]);
void swap_line(bakery** first, bakery* flag[2], int nextdir[2], int l0);
void output(int n, int m);

int main(){
    int n = 0, m = 0;
    scanf("%d%d", &n, &m);
    init_B(n, m);

    int T1 = 0, R = 0, T2 = 0;
    scanf("%d%d%d", &T1, &R, &T2);
    train (n, m, T1);
    race  (&(B[0][0]), R); 
    adjust(n, m);
    train (n, m, T2);
    output(n, m);

    return 0;
}



void init_B(int n, int m){
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


void train (int n, int m, int k){
    int r = 0, l = 0, s = 0;

    for(int i = 0; i < k; i++){  // each day in a T
	scanf("%d%d%d", &r, &l, &s);
	bakery* flag = rankaddr[r];

	for(int cnt = 0;cnt < l; cnt++){  // each turn in a day
	    int nextdir = -1, tmp_d = SIZE, tmp_r = SIZE2+1;
	    for(int j = 0; j < 4; j++) {
		if(flag->next[j] != NULL){
		    if(flag->d[j] < tmp_d) 
			nextdir = j, tmp_d = flag->d[j], tmp_r = flag->next[j]->u;
		    else if(flag->d[j] == tmp_d && flag->next[j]->u < tmp_r) 
			nextdir = j, tmp_r = flag->next[j]->u;
		}
	    }
	    if(nextdir == -1){
		if(flag->t == 0){  // check if the last bakery is closed
		    closed[close_index] = flag->u, close_index++, total[total_index] = 0;
		    total_index++;
		    hide(flag);
		}
		break;
	    }// return if no bakery
	    if(flag->t == 0) closed[close_index] = flag->u, close_index++, total[total_index] = 0;
	    else visited[visit_index] = flag->u, visit_index++, total[total_index] = 1;
	    total_index++;
	    hide(flag);
	    flag = flag->next[nextdir];  // go to next bakery
	    flag->t = (flag->t < s) ? 0 : flag->t - s;  // eat toast
	    if(flag->t == 0 && cnt == l-1){  // check if the last bakery is closed
		closed[close_index] = flag->u, close_index++, total[total_index] = 0;
		total_index++;
		hide(flag);
	    }
	}
	int ct = close_index;
	for(; total_index > 0; total_index--){
	    if(total[total_index-1] == 0)
		putback(rankaddr[closed[(ct = ct-1)]]);
	    else 
		putback(rankaddr[visited[(visit_index = visit_index-1)]]);
	}

	for(; ct < close_index; ct++)
	    hide(rankaddr[closed[ct]]);
    }
    for(; close_index > 0; close_index--)  // put closed back
	putback(rankaddr[closed[close_index-1]]);  // close_index is (real-index)+1
}


void race(bakery ** first, int k){
    int r, l;
    for(int i = 0; i < k; i++){
	scanf("%d%d", &r, &l);
	bakery* flag[2] = {rankaddr[r], rankaddr[r]};
	int nextdir[2] = {0, 2}, l0 = l >> 1;  // dir pointing the flags' next, useful element
	for(int j = 0; j < l0; j++)
	    go_forward(flag, nextdir);
	if(l0 > 0){
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0);
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0 << 1);
	    turn_right(nextdir); swap_line(first, flag, nextdir, l0-1);
	}
    }
}


void adjust (int n, int m){
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
	    if(i == 0 && j == 0) continue;
	    if(i > 0 && j == 0) f = B[i-1][j]->next[2], nextdir = 2, prev = B[i-1][j];
	    else f = B[i][j-1]->next[1], nextdir = 1, prev = B[i][j-1];

	    int dir = -1;
	    for(int m = 0; dir == -1; m++){
		if(f->next[m] == prev) dir = dirpair[nextdir][m];
	    }
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
	    B[i][j] = f;
	}

    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	    if(B[i][j]->t == 0) hide(B[i][j]);
}


void hide(bakery* flag){
    int tmp[4] = {2,3,0,1};
    for(int i = 0; i < 4; i++)
	if(flag->next[i] != NULL){ 
	    flag->next[i]->next[tmp[i]] = flag->next[tmp[i]];
	    flag->next[i]->d[tmp[i]] = flag->d[i] + flag->d[tmp[i]];
	}
}


void putback(bakery* flag){
    int tmp[4] = {2,3,0,1};
    for(int i = 0; i < 4; i++)
	if(flag->next[i] != NULL)
	    if(flag->next[i]->next[tmp[i]] != flag){
		if(flag->next[i]->next[tmp[i]] != NULL){
		    flag->next[i]->next[tmp[i]]->next[i] = flag;
		    flag->next[i]->next[tmp[i]]->d[i] = flag->d[tmp[i]];
		}
		flag->next[i]->next[tmp[i]] = flag;
		flag->next[i]->d[tmp[i]] = flag->d[i];
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

    for(int i = 0; i < l0; i++){
	go_forward(flag, nextdir);
	dirbound[0] = dirbound[1] = -1;
	for(int m = 0; m < 2; m++)
	    if(flag[m]->next[leftdir[nextdir[m]]] != NULL)
		for(int j = 0; dirbound[m] == -1; j++){
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
    }
}


void output(int n, int m/*, bakery* B[SIZE][SIZE]*/){
    for(int i = 0; i < n; i++)
	for(int j = 0; j < m; j++)
	    printf("%d%c", B[i][j]->t, (j == m-1) ? '\n':' ');
}


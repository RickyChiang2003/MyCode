#include <stdio.h>
# define SIZE 26


void print(int R, int C, int B[][SIZE]){
    for(int i = 0; i < R; i++)
	for(int j = 0; j < C; j++){
	    printf("%c", (B[i][j] == 1) ? 'o' : '_');
	    if(j == C-1)
		printf("\n");
	}
}


void input(int* R, int* C, int* flag, int B[][SIZE], int cond[][SIZE][SIZE]){
    scanf("%d%d", R, C);
    *flag = *R * *C;
    int L[2] = {*R, *C}, bars = 0;
    for(int l = 0; l < 2; l++)
	for(int i = 0; i < L[l]; i++){
	    scanf("%d", &bars);
	    for(int j = 0; j < bars; j++)
		scanf("%d", &(cond[l][i][j]));
	}
}


void init(int R, int C, int cond[][SIZE][SIZE], int maxbars[][SIZE]){
    int L[2] = {R, C};
    for(int i = 0; i < 2; i++)
	for(int j = 0; j < L[i]; j++)
	    for(int flag = 0; cond[i][j][flag] != 0; flag++)
		maxbars[i][j] += cond[i][j][flag];
	
    for(int i = 0; i < 2; i++)
	for(int j = 0; j < L[i]; j++){
	    int tmpi = (i) ? 0 : 1;
	    maxbars[i][j] += (L[tmpi] - maxbars[i][j]) * 100;
	}
}


int pruning(int maxbars[][SIZE], int prun[][SIZE], int r, int c){
    if(maxbars[0][r] % 100 < prun[0][r] % 100 || maxbars[0][r] / 100 < prun[0][r] / 100)
	return 0;
    if(maxbars[1][c] % 100 < prun[1][c] % 100 || maxbars[1][c] / 100 < prun[1][c] / 100)
	return 0;
    return 1;
}


int bars_correct(int R, int C, int B[][SIZE], int cond[][SIZE][SIZE], int l, int n){
    int L[2] = {R, C};
    int barlen = 0;
    int black = 0;
    int flag = 0;
    int Il = (l == 0) ? 1 : 0;
    
    for(int i = 0; i < L[Il]; i++){
	int bar = (Il == 0) ? B[i][n] : B[n][i];
	if(bar == 1){
	    black = 1;
	    barlen++;
	}
	if(bar == -1 && black){
	    if(cond[l][n][flag] != barlen || !(cond[l][n][flag])){
		return 0;
	    }
	    barlen = 0;
	    black = 0;
	    flag++;
	}
    }
    if(black){
	if(cond[l][n][flag] != barlen || !(cond[l][n][flag]))
	    return 0;
	flag++;
    }
    if(cond[l][n][flag] != 0)
	return 0;

    return 1;
}


int valid(int R, int C, int B[][SIZE], int cond[][SIZE][SIZE]){
    int L[2] = {R, C};
    for(int l = 0; l < 2; l++)
	for(int i = 0; cond[l][i][0] != 0; i++)
	    if(!bars_correct(R, C, B, cond, l, i))
		    return 0;
    return 1;
}


void findcell(int R, int C, int B[][SIZE], int *r, int *c){
    for(int i = 0; i < R; i++)
	for(int j = 0; j < C; j++)
	    if(B[i][j] == 0){
		*r = i, *c = j;
		return;
	    }
}


int paint(int R, int C, int flag, int B[][SIZE], int cond[][SIZE][SIZE], int maxbars[][SIZE], int prun[][SIZE]){
    if(flag == 0){
	if(valid(R, C, B, cond)){
	    print(R, C, B);
	    return 1;
	}
	
	flag++;
	return 0;
    }
    else{
	//1 for black, -1 for white
	flag--;
	int r, c;
       	findcell(R, C, B, &r, &c);
	B[r][c] = 1, prun[0][r]++, prun[1][c]++;
	if(pruning(maxbars, prun, r, c))
	    if(paint(R, C, flag, B, cond, maxbars, prun))
		return 1;

	B[r][c] = -1, prun[0][r] += 99, prun[1][c] += 99;
	if(pruning(maxbars, prun, r, c))
	    if(paint(R, C, flag, B, cond, maxbars, prun))
		return 1;

	flag++;
	B[r][c] = 0, prun[0][r] -= 100, prun[1][c] -= 100;
	return 0;
    }
}


int main(){
    int R, C, flag = 0;
    int cond[2][SIZE][SIZE] = {0};
    int B[SIZE][SIZE] = {0};
    int maxbars[2][SIZE] = {0};
    int prun[2][SIZE] = {0};
    
    input(&R, &C, &flag, B, cond);
    init(R, C, cond, maxbars);
    paint(R, C, flag, B, cond, maxbars, prun);


    return 0;
}


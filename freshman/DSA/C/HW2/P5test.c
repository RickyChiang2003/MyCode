# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
# define N 1000
# define M 9000


int main(){
    
    srand( time(NULL) );
    FILE *fptr;
    fptr = fopen("P5test.in","w");

    int min = 1;

    int a[N] = {0}, b[N] = {0}, maxa = -1, mina = 1000000000;
    int commend[M][3] = {0};
    fprintf(fptr, "%d %d\n", N, M);
    for(int i = 0; i < N; i++){
	int x = rand() % (N+N/3 - 0 + 1) + 0;
	int same = 0;
	for(int j = 0; j < i; j++)
	    if(x == a[j]) same = 1;
	if(same) 
	    i--;
	else{
	    a[i] = x;
	    fprintf(fptr, "%d%c", x, (i == N-1) ? '\n' : ' ');
	}
    }


    for(int i = 0; i < N; i++){
	//int y = rand() % ((N/3) - 1 + 1) + 1;
	int y = 1;
	fprintf(fptr, "%d%c", y, (i == N-1) ? '\n' : ' ');
	b[i] = y;
    }

    for(int i = 0; i < N; i++){
	if(maxa < a[i]) maxa = a[i];
	if(mina > a[i]) mina = a[i];
    }

    for(int i = 0; i < M; i++){
	int id = rand() % (3 - 1 + 1) + 1;
	int low = rand() % (maxa - (mina-mina/2) + 1) + mina-mina/2;
	int high = rand() % (maxa+maxa/2 - low + 1) + low;
	
	fprintf(fptr, "1 %d %d %d\n", id, low, high);

	commend[i][0] = id;
	commend[i][1] = low;
	commend[i][2] = high;
    }

    fclose(fptr);


    fptr = fopen("P5test.out","w");
    for(int i = 0; i < M; i++){
	int sum = 0;
	for(int j = 0; j < N; j++){
	    if(b[j] == commend[i][0] && a[j] <= commend[i][2] && a[j] >= commend[i][1])
		sum++;
	}
	fprintf(fptr, "%d\n", sum);
    }

    fclose(fptr);
    return 0;

}

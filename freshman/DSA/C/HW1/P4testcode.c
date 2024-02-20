# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand( time(NULL) );

    /* 指定亂數範圍 */
    int min = 1;
    int max = 100;

    int N = rand() % (max - min + 1) + min;
    int M = rand() % (N - min + 1) + min;
    printf("%d %d\n", N, M);

    /* 產生 [min , max] 的整數亂數 */
    for (int i = 0; i < N; i++){
	int x = rand() % (max - min + 1) + min;
	printf("%d%c", x, (i == N - 1) ? '\n' : ' ');
    }

    return 0;
}

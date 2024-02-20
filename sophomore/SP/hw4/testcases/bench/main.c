#include "my_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef long long LL;


void *Collatz(void *args) {
    LL x = *(LL *)args;
    LL cnt = 0;
    while (x != 1) {
	if (x & 1)
	    x = x * 3 + 1;
	else
	    x /= 2;
	cnt++;
	// try uncomment printf
	// printf("%lld\n", x);
    }
    // try uncomment printf
    // printf("%lld\n", cnt);
    return NULL;
}
#define M 0x800000
// N={1,2,5,10,30,50,75,100,125,150}
int main(int argc, char *argv[]) {
    int N;
    N = atoi(argv[1]);
    tpool *pool = tpool_init(N);
    LL *arg = malloc(M * sizeof(LL));
    for (int i = 0; i < M; i++) {
	arg[i] = 0x10000000ll + i;
	tpool_add(pool, Collatz, (void *)&arg[i]);
    }
    tpool_wait(pool);
    tpool_destroy(pool);
    free(arg);
}

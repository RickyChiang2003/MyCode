#include "my_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 2
#define M 3

int output[M][2];

void *func0(void *args) {
    int i = *((int*)args);
    output[i][0] = 0;
    output[i][1] = i;
    return NULL;
    printf("func 0: %d\n", *((int *)args));
}
void *func1(void *args) {
    int i = *((int*)args);
    output[i][0] = 1;
    output[i][1] = i;
    return NULL;
}
int main() {
    tpool *pool = tpool_init(N);
    void *(*funcs[M])(void *) = {&func0, &func1, &func0};
    int *arg = malloc(M * sizeof(int));
    for (int i = 0; i < M; i++) {
	arg[i] = i;
	tpool_add(pool, funcs[i], (void *)&arg[i]);
    }
    tpool_wait(pool);
    for(int i = 0; i < M; i++)
	printf("func %d: %d\n",output[i][0], output[i][1]);
    tpool_destroy(pool);
    free(arg);
}

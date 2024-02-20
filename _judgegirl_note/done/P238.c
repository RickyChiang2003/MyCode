# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //atoi()
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

void process(int n, int p, int sum, int arr[], int total[]) {
    if (p == n) {
        total[sum]++;
    }
    else {
        process(n, p + 1, sum + arr[p], arr, total);
        process(n, p + 1, sum, arr, total);
    }
}

int main() {
    int n, k;
    int arr[15];
    int total[121] = { 0 };

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    process(n, 0, 0, arr, total);

    //for(int i=0;i<121;i++) printf("%d: %d\n", i, total[i]);
    while (scanf("%d", &k) != EOF) {
        if (k > 120) printf("0\n");
        else printf("%d\n", total[k]);
    }
    return 0;
}

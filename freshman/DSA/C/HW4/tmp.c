#include <stdio.h>
#include <time.h>
# define n 1001
# define P 1000000007
# define ll long long

int main() {
    clock_t start, finish;
    double total = 0;
    start = clock();

    //code
    ll a = P, b = 1;
    for(int i = 0; i < 1000; i++){
	//a %= n;
	while(a > n) a -= n;
	a = P;
    }

    finish = clock();
    total = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("time: %fs\n", total);
}



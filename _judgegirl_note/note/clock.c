#include <stdio.h>
#include <time.h>

int main() {
	clock_t start, finish;
	double total = 0;
	start = clock();
	
	//code
	
	finish = clock();
	total = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("time: %fs\n", total);
}


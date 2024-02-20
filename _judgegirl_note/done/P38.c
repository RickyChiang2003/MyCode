# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //atoi()
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


void swap(long long* a, long long* b) {
	long long t = *a;
	*a = *b;
	*b = t;
}

long long sort(long long* a, long long* b, long long* c, long long* A, long long* B, long long* C) {
	if (*C > *B)
		swap(b, c), swap(B, C);
	if (*B > *A)
		swap(b, a), swap(B, A);
	if (*C > *B)
		swap(b, c), swap(B, C);
}

long long find(long long D, long long a, long long b, long long c, long long A, long long B, long long C, long long ta, long long tb, long long tc) {
	if (D == (ta * A + tb * B + tc * C))
		return 1;
	if ((ta == a || A > D) && (tb == b || B > D) && (tc == c || C > D))
		return 0;

	if (ta == a) {
		ta = 0;
		if (tb == b) {
			tb = 0;
			tc++;
		}
		else
			tb++;
	}
	else
		ta++;

	return find(D, a, b, c, A, B, C, ta, tb, tc);
	
}

int main() {
	long long cnt;
	long long D, a, b, c, A, B, C, ta, tb, tc;

	for (scanf("%lld", &cnt); cnt > 0; cnt--) {
		scanf("%lld%lld%lld%lld%lld%lld%lld", &D, &a, &b, &c, &A, &B, &C);

		ta = tb = tc = 0;
		sort(&a, &b, &c, &A, &B, &C);

		if (find(D, a, b, c, A, B, C, ta, tb, tc))
			printf("yes\n");
		else
			printf("no\n");
	}

	return 0;
}
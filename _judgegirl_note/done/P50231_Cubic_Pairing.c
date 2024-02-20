# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
# define SIZE 105
# define hashSIZE 11025


int Cubic[SIZE][SIZE][SIZE] = { 0 };
int height[SIZE][SIZE] = { 0 };
int hash[hashSIZE][10][3] = { 0 };  // i,j,value


int gethash(int k, int S) {
	return (77 * k + 2222) % S;
}

int main() {
	//init
	int N, S, C;
	scanf("%d%d%d", &N, &S, &C);
	for (int h = 0; h < N; h++)
		for (int i = 0; i <= h; i++)
			for (int j = 0; j <= h; j++)
				scanf("%d", &(Cubic[h][i][j]));
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			height[i][j] = i > j ? i : j;

	//find
	int flag1[2], flag2[2], found;  // i,j
	//first find
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			int value = Cubic[height[i][j]][i][j];
			int hashvalue = gethash(value, S);

			found = 0;
			for (int k = 0; k < 10; k++)
				if (hash[hashvalue][k][2] == value) {
					flag1[0] = i;
					flag1[1] = j;
					flag2[0] = hash[hashvalue][k][0];
					flag2[1] = hash[hashvalue][k][1];
					hash[hashvalue][k][2] = 0;
					found = 1;
					printf("%d", value);
					height[flag1[0]][flag1[1]]++;
					height[flag2[0]][flag2[1]]++;
					value = Cubic[height[i][j]][i][j];
					break;
				}
			if (!found) {
				int k = 0;
				while (hash[hashvalue][k][2] != 0) k++;
				assert(k < 10);
				hash[hashvalue][k][0] = i;
				hash[hashvalue][k][1] = j;
				hash[hashvalue][k][2] = value;
			}
		}

	found = 1;
	//all other find
	while (found) {
		//printf("start\n");
		//printf("flag1[0] = %d, flag1[1] = %d, flag2[0] = %d, flag2[1] = %d\n", flag1[0], flag1[1], flag2[0], flag2[1]);
		found = 0;
		assert(height[flag1[0]][flag1[1]] <= N && height[flag2[0]][flag2[1]] <= N);

		int k, tmp[2] = { 0 }, found2 = 0;
		if (height[flag1[0]][flag1[1]] < N) {
			int val1 = Cubic[height[flag1[0]][flag1[1]]][flag1[0]][flag1[1]];
			int hash1 = gethash(val1, S);
			for (k = 0; k < 10; k++)
				if (hash[hash1][k][2] == val1) {
					tmp[0] = hash[hash1][k][0];
					tmp[1] = hash[hash1][k][1];
					hash[hash1][k][2] = 0;
					found = 1;
					printf(" %d", val1);
					height[flag1[0]][flag1[1]]++;
					height[tmp[0]][tmp[1]]++;
					break;
				}
			if (!found) {
				k = 0;
				while (hash[hash1][k][2] != 0) k++;
				assert(k < 10);
				hash[hash1][k][0] = flag1[0];
				hash[hash1][k][1] = flag1[1];
				hash[hash1][k][2] = val1;
			}
		}
		if (height[flag2[0]][flag2[1]] < N) {
			int val2 = Cubic[height[flag2[0]][flag2[1]]][flag2[0]][flag2[1]];
			int hash2 = gethash(val2, S);
			for (k = 0; k < 10; k++)
				if (hash[hash2][k][2] == val2) {
					flag1[0] = hash[hash2][k][0];
					flag1[1] = hash[hash2][k][1];
					hash[hash2][k][2] = 0;
					found = 1;
					found2 = 1;
					printf(" %d", val2);
					height[flag1[0]][flag1[1]]++;
					height[flag2[0]][flag2[1]]++;
					break;
				}
			if (!found2) {
				k = 0;
				while (hash[hash2][k][2] != 0) k++;
				assert(k < 10);
				hash[hash2][k][0] = flag2[0];
				hash[hash2][k][1] = flag2[1];
				hash[hash2][k][2] = val2;
			}
		}
		if (found && !found2)
			flag2[0] = tmp[0], flag2[1] = tmp[1];
		//printf("\nend\n");
		//printf("flag1[0] = %d, flag1[1] = %d, flag2[0] = %d, flag2[1] = %d\n",flag1[0],flag1[1],flag2[0],flag2[1]);
	}
	printf("\n");
#ifdef DEBUG
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			printf("%8d%c", Cubic[height[i][j]][i][j], j == N - 1 ? '\n' : ' ');
#endif

	return 0;
}

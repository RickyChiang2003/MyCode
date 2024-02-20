# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int adjust(int i, int j, int F[][9], int ans[][9], int tmp[][9], int d){
	int notOF = 1;
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++){
			int x = i + dx;
			int y = j + dy;
			if (x >= 0 && x < 9 && y >= 0 && y < 9)
				if(F[x][y] < (tmp[x][y] += d))
					notOF = 0; 
		}
	ans[i][j] = (d == 1 && notOF) ? 1 : 0;
	return notOF;
}

void process(int i, int j, int F[][9], int ans[][9], int tmp[][9]){
	if(i == 9){
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (F[i][j] != tmp[i][j])
					return;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				printf("%d%c", ans[i][j], (j == 8) ? '\n' : ' ');
		exit(0);
	}

	for (int i1 = 0; i1 < i - 1; i1++)
		for (int j1 = 0; j1 < 9; j1++)
			if(F[i1][j1] != tmp[i1][j1])
				return;

	int I = (j == 8) ? i + 1 : i;
	int J = (j == 8) ? 0 : j + 1;
	if (adjust(i, j, F, ans, tmp, 1))
		process(I, J, F, ans, tmp);
	adjust(i, j, F, ans, tmp, -1);
	process(I, J, F, ans, tmp);	
}

int main() {
	int F[9][9] = { 0 };
	int ans[9][9] = { 0 };
	int tmp[9][9] = { 0 };
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			scanf("%d", &F[i][j]);
	
	process(0, 0, F, ans, tmp);

	printf("no solution\n");

	return 0;
}
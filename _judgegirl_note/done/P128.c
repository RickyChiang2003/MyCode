# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //atoi()
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//intersection; T-junction; turn; dead
void intersection(int map[100][100], int result[4]) {

	result[0] = result[1] = result[2] = result[3] = 0;
	int di[] = { -1, 0, 1, 0 };
	int dj[] = { 0, 1, 0, -1 };
	int form[2]; //verticle and horizontal

	for(int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			if (map[i][j]) {

				form[0] = form[1] = 0;
				for (int dir = 0; dir < 4; dir++) {
					int it = i + di[dir];
					int jt = j + dj[dir];
					if (it < 100 && jt < 100 && it >= 0 && jt >= 0)
						if (map[it][jt]) {
							form[0] = (dir % 2) ? form[0] : form[0] + 1;
							form[1] = !(dir % 2) ? form[1] : form[1] + 1;
						}					
				}

				if (form[0] == form[1] && form[0] == 2)
					result[0]++;
				if ((form[0] == 2 && form[1] == 1) || (form[0] == 1 && form[1] == 2))
					result[1]++;
				if (form[0] == form[1] && form[0] == 1)
					result[2]++;
				if ((form[0] == 1 && form[1] == 0) || (form[0] == 0 && form[1] == 1))
					result[3]++;
			}
		

	return;
}

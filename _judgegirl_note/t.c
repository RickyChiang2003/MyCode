# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define SIZE 1000
//10000
# define nameSIZE 32

struct person {
	unsigned int id;
	char name[32];
};
struct friends {
	unsigned id1;
	unsigned id2;
};

char people[SIZE][32] = { 0 };
int relation[SIZE][SIZE] = { 0 };

int main(){
	FILE* fin = fopen("1.friends", "rb");

	int P, F;
	struct person p = { 0 };
	struct friends f = { 0 };
	fread(P, sizeof(P), 1, fin);
	for (int i = 0; i < P; i++) {
		fread(p, sizeof(p), 1, fin);
		strcpy(people[p.id], p.name);
	}
	fread(F, sizeof(F), 1, fin);
	for (int i = 0; i < F; i++) {
		fread(f, sizeof(f), 1, fin);
		relation[f.id1][f.id2] = 1;
		relation[f.id2][f.id1] = 1;
	}

	while()

	fclose(fin);
	return 0;
}





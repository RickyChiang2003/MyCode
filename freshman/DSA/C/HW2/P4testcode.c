# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// N*M <= 1e6
# define cSIZE 1000   //company
# define dSIZE 1000 //day
# define MONEY 1000000000 

int main(){
    srand( time(NULL) );
    FILE *fptr;
    fptr = fopen("P4testcode_input.in","w");

    int min = 1;
    int arr[cSIZE][dSIZE] = {0};
    for(int i = 0; i < cSIZE; i++)
	for(int j = 0; j < dSIZE; j++)
	    arr[i][j] = 1000000002;

    fprintf(fptr, "%d %d %d\n", cSIZE, dSIZE, MONEY);
    //printf("%d %d %d\n", cSIZE, dSIZE, MONEY);  // 5 company, 9 days, MONEY=120
    for(int i = 1; i < cSIZE; i++)
	fprintf(fptr, "%d%c", i, (i == cSIZE-1) ? '\n' : ' ');
	//printf("%d%c", i, (i == cSIZE-1) ? '\n' : ' ');
    for (int i = 0; i < dSIZE; i++){
	for(int j = 0; j < cSIZE; j++){
	    int x = rand() % (MONEY - min + 1) + min;
	    int y = rand() % (dSIZE/100 - 0 + 1) + 0;
	    fprintf(fptr, "%d %d\n", x, y);
	    //printf("%d %d\n", x, y);
	    for(int k = i; k <= i+y && k < dSIZE; k++){
		if(arr[j][k] > x) arr[j][k] = x;
	    }
	}
    }
    fclose(fptr);
    
    
    fptr = fopen("P4testcode_init_arr","w");
    for(int i = 0; i < cSIZE; i++)
	for(int j = 0; j < dSIZE; j++)
	    fprintf(fptr, "%3d%c", arr[i][j], (j == dSIZE-1) ? '\n' : ' ');
	    //printf("%3d%c", arr[i][j], (j == dSIZE-1) ? '\n' : ' ');
    fclose(fptr);

    fptr = fopen("P4testcode_final_arr","w");
    for(int i = 0; i < dSIZE; i++){
	int sum = 0;
	for(int j = cSIZE-1; j >= 0; j--){
	    sum += arr[j][i];
	    if(sum > MONEY) sum = MONEY+1;
	    arr[j][i] = sum;
	}
    }
    for(int i = 0; i < cSIZE; i++)
	for(int j = 0; j < dSIZE; j++)
	    fprintf(fptr, "%3d%c", arr[i][j], (j == dSIZE-1) ? '\n' : ' ');
	    //printf("%3d%c", arr[i][j], (j == dSIZE-1) ? '\n' : ' ');

    fptr = fopen("P4testcode_output.out","w");
    for(int i = 0; i < dSIZE; i++){
	for(int j = 0; j < cSIZE; j++){
	    if(arr[j][i] <= MONEY){
		fprintf(fptr, "%d\n", cSIZE-j);
		//printf("%d\n", cSIZE-j);
		break;
	    }
	    if(j == cSIZE-1)
		fprintf(fptr, "0\n");
	}
    }


    return 0;
}




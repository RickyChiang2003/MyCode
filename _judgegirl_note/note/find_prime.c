#include <stdio.h>
#include <stdlib.h>
#include <math.h>
# define min 10000000
# define max 10001000


int main(){
    FILE *f = fopen("prime.out", "w");
    int i,k,leap = 1;
    int m;
    for(m = min; m <= max; m++){
	k = (int)sqrt(m);
	for(i = 2; i <= k; i++){
	    if(m%i == 0){
		leap = 0;
		break;
	    }
	}
	if(leap)
	    fprintf(f, "%d\n", m);
	leap = 1;
    }
    fclose(f);
    return 0;
}

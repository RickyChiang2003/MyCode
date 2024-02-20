# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct Players{
    struct Players* prev;
    struct Players* next;
    int ID; //same as in-round
    int ATK;
    int in_num;
    int front_r;
    int score1;  //score[ID - 1]
} player;


void init(player* a){
    a->next = malloc(sizeof(player));
    player* tmp = a->next;
    tmp->prev = a;
    tmp->next = NULL;
}


int count_score(int M, int front_r, int in_num, int d, int score1, int score2){
//    printf("M = %d, front_r = %d, in_num = %d, d = %d, score1 = %d, score2 = %d\n", M, front_r, in_num, d, score1, score2);
    return ((M - in_num + 1) * d + score2 - score1 - (front_r * d));
}


int main(){
    int N, M;
    int num = 0, r_num = 0, tATK;
    scanf("%d%d", &N, &M);
    player* flag = malloc(sizeof(player));
    init(flag);
    player* head = flag;

    int score = 0, prev_score = 0; //score from revolution

    for(int i = 1; i <= N; i++){
	int out[1001][2] = {0}, out_i = 0;
	scanf("%d", &tATK);
	printf("Round %d:", i);

	while(num > 0){
	    if(flag->ATK >= tATK)
		break;
	    out[out_i][0] = flag->ID;
//	    printf("\nkill ID = %d\n", flag->ID);	
	    int S = count_score(M, flag->front_r, flag->in_num, i-(flag->ID), flag->score1, prev_score);
	    out[out_i][1] = S;
	    out_i++;
	    if(num > 1)
		flag = flag->prev;
	    num--;
	}
	num++;
	
	if(num > M){
//	    printf("\nrevolution ID = %d\n", head->ID);
	    int S = count_score(M, head->front_r, head->in_num, i-(head->ID), head->score1, prev_score);
	    printf(" %d,%d", head->ID, S);
	    player* tmp = head;
	    head = head->next;
	    if(head->prev == flag)
		flag = head;
	    if(head->next == NULL)
		init(head);
	    head->prev = NULL;
	    free(tmp);
	    r_num ++;
	    num--;
	}

	if(num > 1){
	    flag = flag->next;
	    if(flag->next == NULL)
		init(flag);
	}
	flag->ID = i;
	flag->ATK = tATK;
	flag->in_num = num;
	flag->front_r = r_num;
	flag->score1 = score;
	score += r_num;
	prev_score = score;

//	printf("\nscore[%d] = %d\n", i, score);
	
	for(int j = 0; j < out_i; j++)
	    printf(" %d,%d", out[j][0], out[j][1]);
	printf("\n");

    }

    printf("Final:");
    while(flag != NULL){
//	printf("\nfinal ID = %d\n", flag->ID);
        int S = count_score(M, flag->front_r, flag->in_num, N+1-(flag->ID), flag->score1, score);
	printf(" %d,%d", flag->ID, S);
	flag = flag->prev;
    }
    printf("\n");


    return 0;
}

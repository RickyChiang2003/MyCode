# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct Players{
    struct Players* prev;
    struct Players* next;
    int ID;
    int ATK;
} player;


void init(player* a){
    a->next = malloc(sizeof(player));
    player* tmp = a->next;
    tmp->prev = a;
    tmp->next = NULL;
}


int main(){

    int N, M;
    int num = 0, tATK;
    scanf("%d%d", &N, &M);
    player* flag = malloc(sizeof(player));
    init(flag);
    player* head = flag;

    for(int i = 1; i <= N; i++){
	int out[1001] = {0}, out_i = 0;
	scanf("%d", &tATK);
	printf("Round %d:", i);

	while(num > 0){
	    if(flag->ATK >= tATK)
		break;
	    out[out_i] = flag->ID;
	    out_i++;
	    if(num > 1)
		flag = flag->prev;
	    num--;
	}
	num++;
	
	if(num > M){
	    printf(" %d", head->ID);
	    num--;
	    player* tmp = head;
	    head = head->next;
	    if(head->prev == flag)
		flag = head;
	    if(head->next == NULL)
		init(head);
	    head->prev = NULL;
	    free(tmp);
	}

	if(num > 1){
	    flag = flag->next;
	    if(flag->next == NULL)
		init(flag);
	}
	flag->ID = i;
	flag->ATK = tATK;
	
	for(int j = 0; j < out_i; j++)
	    printf(" %d", out[j]);
	printf("\n");
    }

    printf("Final:");
    while(flag != NULL){
	printf(" %d", flag->ID);
	flag = flag->prev;
    }
    printf("\n");

    return 0;
}

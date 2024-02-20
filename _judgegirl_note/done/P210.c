#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct info{
    char data[5][81];   //lname fname ID sal age
};


int findword(char c){
    switch(c){
        case 'l':
            return 0;
        case 'f':
            return 1;
        case 'I':
            return 2;
        case 's':
            return 3;
        case 'a':
            return 4;
        default:
            printf("you fucked up\n");
            return 0;
        }
}


int isdigit_str(char s[81]){
    int l = strlen(s);
    for (int i = 0; i < l; i++)
        if(!isdigit(s[i]))
            return 0;
    return 1;
}


void digitprocess(int cnt_m, struct info man[], int tgt[], int n_tgt, char cond[][81]){
    int c = atoi(cond[2]);
    char o = cond[1][0];    
    for (int i = 0; i < cnt_m; i++){
        int v = (cond[0][0] == 'a') ? atoi(man[i].data[4]) : atoi(man[i].data[3]);

        if ((v == c && o == '=') || (v > c && o == '>') || (v < c && o == '<'))
            for (int j = 0; j < n_tgt; j++)
                printf("%s%c", man[i].data[tgt[j]], (j == n_tgt - 1) ? '\n' : ' ');
    }

}


void strprocess(int cnt_m, struct info man[], int tgt[], int n_tgt, char cond[][81]){
    for (int i = 0; i < cnt_m; i++){
        int iseq = (strcmp(man[i].data[findword(cond[0][0])], cond[2]) == 0) ? 1 : 0;

        if((iseq && cond[1][0] == '=') || (!iseq && cond[1][0] == '!'))
            for (int j = 0; j < n_tgt; j++)
                printf("%s%c", man[i].data[tgt[j]], (j == n_tgt - 1) ? '\n' : ' ');       
    }

}


int main(){
    int cnt_m;
    struct info man[51];
    scanf("%d", &cnt_m);
    for (int i = 0; i < cnt_m; i++)
        for (int j = 0; j < 5; j++)
            scanf("%s", man[i].data[j]);

    int cnt;
    scanf("%d", &cnt);
    for (int i = 0; i < cnt; i++){
        char select[7] = {0};
        scanf("%s", select);

        int tgt[8] = {0};
        int n_tgt = 0;      //have to be 1~8
        for (int j = 0; j <= 8; j++){
            char tmp[81] = {0};
            scanf("%s", tmp);
            if(tmp[0] == 'w')
                break;
            tgt[j] = findword(tmp[0]);
            n_tgt++;
        }

        char cond[3][81] = {0}; // field operator const
        scanf("%s%s%s", cond[0], cond[1], cond[2]);
        if (isdigit_str(cond[2]))
            digitprocess(cnt_m, man, tgt, n_tgt, cond);
        else
            strprocess(cnt_m, man, tgt, n_tgt, cond);
    }

    
    return 0;
}

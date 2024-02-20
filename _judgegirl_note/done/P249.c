#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> //atoi(), itoa(); abs() 絕對值;
#include <ctype.h>
#include <string.h>

typedef struct employee{
    int id;
    char f_name[32];
    char l_name[32];
    int boss_id;
    
} EMP;


void findsub(int n, EMP mem[32], int relation[32][2][34], int origin, int host, int cnt){
    for (int i = 0; i < n; i++)
        if(mem[i].id == mem[host].boss_id && i != host){
            relation[origin][0][cnt] = mem[i].id;
            if(mem[i].id != mem[i].boss_id)
                findsub(n, mem, relation, origin, i, cnt + 1);
            return;
        }           
}


void findsup(int n, EMP mem[32], int relation[32][2][34], int origin, int host, int* cnt){
    for (int i = 0; i < n; i++)
        if(mem[i].boss_id == mem[host].id && i != host){
            relation[origin][1][*cnt] = mem[i].id;
            *cnt += 1;
            //if(strcmp(mem[origin].l_name, "E") == 0)
            //   printf("relation[origin][1][%d] = mem[%d].id = %d\n", *cnt, i, mem[i].id);
            findsup(n, mem, relation, origin, i, cnt); 
        }           
    return;
}


void findID(int n, EMP mem[32], char P1[2][32], char P2[2][32], int ID[3], int BOSS_ID[3]){
    
    for (int i = 0; i < n; i++){
        //printf("mem[i].l_name = %s, P1[f1] = %s\n", mem[i].l_name, P1[1]);
        if(strcmp(mem[i].f_name, P1[0]) == 0)
            if(strcmp(mem[i].l_name, P1[1]) == 0){
                BOSS_ID[1] = mem[i].boss_id;
                ID[1] = mem[i].id;
                break;
            }
    }
    for (int i = 0; i < n; i++){
        //printf("mem[i].l_name = %s, P2[f1] = %s\n", mem[i].l_name, P2[1]);
        if(strcmp(mem[i].f_name, P2[0]) == 0)
            if(strcmp(mem[i].l_name, P2[1]) == 0){
                BOSS_ID[2] = mem[i].boss_id;
                ID[2] = mem[i].id;
                break;
            }
    }

}

int iscol(int a1[], int a2[]){
    for (int i = 1; a1[i] != 0; i++)
        for (int j = 1; a2[j] != 0; j++)
            if(a1[i] == a2[j])
                return 1;
    return 0;
}

int issub(int subrelation[2][34], int ID2){
    for (int i = 1; subrelation[0][i] != 0; i++)
        if(subrelation[0][i] == ID2)
            return 1;
    return 0;
}


int issup(int subrelation[2][34], int ID2){
    for (int i = 1; subrelation[1][i] != 0; i++){
        //printf("subrelation[1][%d] = %d, ID2 = %d\n", i, subrelation[1][i], ID2);
        if(subrelation[1][i] == ID2)
            return 1;        
    }
    return 0;
}


int main(){
    int n;
    EMP mem[32];
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d%s%s%d", &mem[i].id, mem[i].f_name, mem[i].l_name, &mem[i].boss_id);

    int relation[32][2][34] = {0};   //(member), (have to output sub, sup), (exist people's ID)
    for (int i = 0; i < n; i++){
        relation[i][0][0] = mem[i].id;
        findsub(n, mem, relation, i, i, 1);   // 0 for himself
        int c_1 = 1;
        findsup(n, mem, relation, i, i, &c_1);        
    }
    
    int m;
    scanf("%d", &m);
    for (int i = 0; i < m; i++){
        char P1[2][32] = {0};         //f_name and l_name
        char P2[2][32] = {0};
        int ID[3] = {0}, BOSS_ID[3] = {0};
        scanf("%s%s%s%s", P1[0], P1[1], P2[0], P2[1]);
        findID(n, mem, P1, P2, ID, BOSS_ID);
        //printf("BOSS_ID[1] = %d, BOSS_ID[2] = %d, ID[1] = %d, ID[2] = %d\n", BOSS_ID[1], BOSS_ID[2], ID[1], ID[2]);
        int cnt1 = 0, cnt2 = 0;
        while (relation[cnt1][0][0] != ID[1])
            cnt1++;
        while (relation[cnt2][0][0] != ID[2])
            cnt2++;

        if(issub(relation[cnt1], ID[2]))
            printf("subordinate\n");
        else if(issup(relation[cnt1], ID[2]))
            printf("supervisor\n");
        else if(iscol(relation[cnt1][0], relation[cnt2][0]) && !(ID[1] == BOSS_ID[1] || ID[2] == BOSS_ID[2]))
            printf("colleague\n");
        else
            printf("unrelated\n");
        
    }
}
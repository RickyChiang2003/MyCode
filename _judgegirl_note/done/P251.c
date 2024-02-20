# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct employee {
  char first_name[32];
  char last_name[32];
  struct employee *boss;
} Employee;

typedef struct me {
    int id;
    char first_name[32];
    char last_name[32];
    int boss_id;
} employee;

int relation(Employee *e1, Employee *e2) {
    Employee *t1 = e1;
    while(t1 != t1->boss){
        if(t1->boss == e2)
            return 1;
        t1 = t1->boss;
    }
    Employee *t2 = e2;
    while(t2 != t2->boss){
        if(t2->boss == e1)
            return 2;
        t2 = t2->boss;
    }
    if(t1 == t2)
        return 3;
    return 4;
}
 
void readName(employee *e) {
    scanf("%s %s", e->first_name, e->last_name);
}
 
int nameToIndex(employee *e, employee A[], int n) {
    for (int i = 0; i < n; i++) {
        if (!strcmp(A[i].first_name, e->first_name) &&
            !strcmp(A[i].last_name, e->last_name))
            return i;
    }
    return -1;
}
int main() {
    int n, m;
    employee A[32];
    Employee B[32];
    const char out[4][32] = {"subordinate", "supervisor", "colleague", "unrelated"};
    while (scanf("%d", &n) == 1) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &A[i].id);
            readName(&A[i]);
            scanf("%d", &A[i].boss_id);
        }
 
        for (int i = 0; i < n; i++) {
            strcpy(B[i].first_name, A[i].first_name);
            strcpy(B[i].last_name, A[i].last_name);
            B[i].boss = NULL;
        }
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) 
                if (A[i].boss_id == A[j].id)
                    B[i].boss = &B[j];

        scanf("%d", &m);
        employee x, y;
        for (int i = 0; i < m; i++) {
            readName(&x);
            readName(&y);
            int ix = nameToIndex(&x, A, n);
            int iy = nameToIndex(&y, A, n);
            printf("%d\n", relation(&B[ix], &B[iy]));
        }
    }
    return 0;
}
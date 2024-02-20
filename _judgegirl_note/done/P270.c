#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VAR 100   // max of var
#define NAME 80   // max length of var name
#define LINE 1001 // max of lines of code

enum Operator{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    EQL,
    NOTEQL,
    SMALLER,
    LARGER,
    SMALLEREQL,
    LARGEREQL
};

enum StatementType{
    STOP,
    IF,
    GOTO,
    PRINT,
    ASSIGN
};

int find(char *str, char *table[]);
int findOpr(char *opr);
void processIF(int linenum, char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[]);
void processASSIGN(int linenum, char *first, char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int ans[]);

int compare(int opd1, int opd2, int opr);
int compute(int opd1, int opd2, int opr);

void readCode(char varName[][NAME], int varValue[], char *varNameTable[]);
void processCode(char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[], int ans[]);
void runCode(char varName[][NAME], int varValue[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[], int ans[]);

/* main */
int main(){

    char varName[VAR][NAME] = {0};
    int varValue[VAR] = {0};
    char *varNameTable[VAR] = {0};
    readCode(varName, varValue, varNameTable);

    int code[LINE] = {0};
    int gotoLine[LINE] = {0};
    int opd1[LINE] = {0}, compopr[LINE] = {0}, opd2[LINE] = {0};
    int ans[LINE] = {0};
    processCode(varNameTable, code, opd1, opd2, compopr, gotoLine, ans);

    runCode(varName, varValue, code, opd1, opd2, compopr, gotoLine, ans);

    return 0;
}

int find(char *str, char *table[]){
    for (int i = 0; table[i] != NULL; i++)
        if (strcmp(str, table[i]) == 0)
            return i;
    return -1;
}

int findOpr(char *opr){
    char *Opr[] = {"+", "-", "*", "/", "%", "==", "!=", "<", ">", "<=", ">=", NULL};
    return (find(opr, Opr));
}

void processIF(int linenum, char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[]){
    char gotoStr[NAME] = {0}, opda[NAME] = {0}, opdb[NAME] = {0}, opr[NAME] = {0};

    code[linenum] = IF;
    scanf("%s", opda);
    opd1[linenum] = find(opda, varNameTable);
    scanf("%s", opr);
    compopr[linenum] = findOpr(opr);
    scanf("%s", opdb);
    opd2[linenum] = find(opdb, varNameTable);
    scanf("%s", gotoStr);
    scanf("%d", &(gotoLine[linenum]));
}

void processASSIGN(int linenum, char *first, char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int ans[]){
    char opda[NAME] = {0}, opdb[NAME] = {0}, opr[NAME] = {0};

    code[linenum] = ASSIGN;
    ans[linenum] = find(first, varNameTable);
    char trash[NAME] = {0}; // it must be '='
    scanf("%s", trash);
    scanf("%s", opda);
    opd1[linenum] = find(opda, varNameTable);
    scanf("%s", opr);
    compopr[linenum] = findOpr(opr);
    scanf("%s", opdb);
    opd2[linenum] = find(opdb, varNameTable);
}

void processCode(char *varNameTable[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[], int ans[]){
    char first[NAME] = {0};
    char opd[NAME] = {0};
    int linenum = 1;
    char *firstTable[] = {"STOP", "IF", "GOTO", "PRINT", NULL};

    while (scanf("%s", first) != EOF){
        int firstType = find(first, firstTable);
        switch (firstType){
        case GOTO:
            code[linenum] = GOTO;
            scanf("%d", &(gotoLine[linenum]));
            break;
        case STOP:
            code[linenum] = STOP;
            break;
        case IF:
            code[linenum] = IF;
            processIF(linenum, varNameTable, code, opd1, opd2, compopr, gotoLine);
            break;
        case PRINT:
            code[linenum] = PRINT;
            scanf("%s", opd);
            ans[linenum] = find(opd, varNameTable);
            break;
        default:
            code[linenum] = ASSIGN;
            processASSIGN(linenum, first, varNameTable, code, opd1, opd2, compopr, ans);
            break;
        }
        linenum++;
    }
}

int compare(int opd1, int opd2, int opr){
    switch (opr){
    case EQL:
        return (opd1 == opd2);
    case NOTEQL:
        return (opd1 != opd2);
    case SMALLER:
        return (opd1 < opd2);
    case LARGER:
        return (opd1 > opd2);
    case SMALLEREQL:
        return (opd1 <= opd2);
    case LARGEREQL:
        return (opd1 >= opd2);
    default:
        printf("You fucked up, Haiyaa\n");
        exit(-1);
    }
}

int compute(int opd1, int opd2, int opr){
    switch (opr){
    case ADD:
        return (opd1 + opd2);
    case SUB:
        return (opd1 - opd2);
    case MUL:
        return (opd1 * opd2);
    case DIV:
        return (opd1 / opd2);
    case MOD:
        return (opd1 % opd2);
    default:
        printf("You fucked up, Haiyaa\n");
        exit(-1);
    }
}

void runCode(char varName[][NAME], int varValue[], int code[], int opd1[], int opd2[], int compopr[], int gotoLine[], int ans[]){
    int line = 1; // starts at line 2

    while (code[line] != STOP){
        int v1 = varValue[opd1[line]], v2 = varValue[opd2[line]];
        int opr = compopr[line];

        switch (code[line]){
        case IF:
            if (compare(v1, v2, opr))
                line = gotoLine[line];
            else
                line++;
            break;
        case GOTO:
            line = gotoLine[line];
            break;
        case ASSIGN:
            varValue[ans[line]] = compute(v1, v2, opr);
            line++;
            break;
        case PRINT:
            printf("%d\n", varValue[ans[line]]);
            line++;
            break;
        default:
            printf("You fucked up, Haiyaa\n");
            exit(-1);
        }
    }
}

void readCode(char varName[][NAME], int varValue[], char *varNameTable[]){
    int cnt = 0;
    char name[NAME] = {0};
    scanf("%s", name);
    while (strcmp(name, "END") != 0){
        char trash[NAME] = {0};
        strcpy(varName[cnt], name);
        scanf("%s", trash);
        scanf("%d", &(varValue[cnt]));
        varNameTable[cnt] = varName[cnt];
        cnt++;
        scanf("%s", name);
    }
    varNameTable[cnt] = NULL;
}
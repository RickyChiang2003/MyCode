#include <stdio.h>
int hasEOF = 0;
int readchar()
{
    static int N = 1 << 20;
    static char buf[1 << 20];
    static char *p = buf, *end = buf;
    if (p == end)
    {
        if ((end = buf + fread(buf, 1, N, stdin)) == buf)
        {
            hasEOF = 1;
            return EOF;
        }
        p = buf;
    }
    return *p++;
}
int ReadInt(int *x)
{
    char c, neg;
    while ((c = readchar()) < '-')
    {
        if (c == EOF)
            return 0;
    }
    neg = (c == '-') ? -1 : 1;
    *x = (neg == 1) ? c - '0' : 0;
    while ((c = readchar()) >= '0')
        *x = (*x << 3) + (*x << 1) + c - '0';
    *x *= neg;
    return 1;
}

// Or, 
//     printf  -->  fputc(c, stdout), putchar(c)
//     scanf   -->  c = fgetc(stdin), c = getchar()
// 
// in file
//     use fprintf, fscanf originally
//                       fputc(c, fptr)
//     get string   -->  fgets(s, SIZE, fptr)      (return string or NULL; SIZE means the size of s[SIZE], include '\0')
//     print string -->  fputs(s, fptr)
//     fread(buf, sizeof(int), arr_SIZE, fptr)  // or char or something
//     fwrite(buf, sizeof(int), arr_SIZE, fptr)
// file tool
//     rewind(fptr)
//     feof(fptr)
//     fseek(fptr, n*sizeof(int), SEEK_CUR)  // SEEK_SET, SEEK_CUR, SEEK_END
// 
// in string
//     use sprintf, sscanf originally

int main(){
    int x;
    while (ReadInt(&x)){
        // add your code
    }
    fputc()
    // output your answer
    return 0;
}
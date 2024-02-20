#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
# define SIZE 100
// _exit(0), wait(&status) or waitpid(-1,&status,0), 

int main(){

    int fd1;
    FILE *fdo = fopen("file", "r");
    fscanf(fdo, "%d", &fd1);
    fclose(fdo);
    
    char buf[SIZE] = {0};
    strcpy("This is prog-1 output.\n", buf);
    printf("prog-1 stdout.\n");
    write(fd1, buf, SIZE);

    return 0;
}

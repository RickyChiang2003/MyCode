#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]){    
    int i, pid;
    // Section A
    char output[argc][100];
    int fd[2];
    pipe(fd);
    // Section A END
    
    for (i=1; i<argc; i++){
        // Section B
        // Section B END

        pid = fork();
        // child process
        if ( pid == 0 ){
            // Section C 
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
            // Section C END
            
            execlp( argv[i], argv[i], (char *) 0 ); 
        }
        // Section D
        // parent process
        else if( pid > 0){
            waitpid(pid, &status, 0);
        }
        // Section D END

    }
    // Section E
            close(fd[1]);
    for(i=1; i<argc; i++){
            read(fd[0], buf[i], sizeof(buf[i]));
        printf("%s", buf[i]);
    }
    // Section E END
}

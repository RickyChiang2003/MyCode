#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]){    
    int i, pid;
    // Section A
    int fd[argc][2];
    char prog_output[argc][SIZE];
    // Section A END
    
    for (i=1; i<argc; i++){
        // Section B
        pipe(fd[i])
        // Section B END

        pid = fork();
        // child process
        if ( pid == 0 ){
            // Section C 
            close(fd[i][0]);
            dup2(fd[i][1], 1);
            close(fd[i][1]);
            // Section C END
            
            execlp( argv[i], argv[i], (char *) 0 ); 
        }
        // Section D
        // parent process
        else if( pid > 0){
            close(fd[i][1]);
            waitpid(pid, &status, 0);
        }
        // Section D END

    }
    // Section E
    for(i=1; i<argc; i++){
        int len = read(fd[i][0], prog_output[i], SIZE);
        buf[len] = '\0';
    }
    //process prog_output[][]

    // Section E END
}

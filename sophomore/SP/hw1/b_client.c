#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 512

typedef struct {
    char* ip; // server's ip
    unsigned short port; // server's port
    int conn_fd; // fd to talk with server
    char buf[BUFFER_SIZE]; // data sent by/to server
    size_t buf_len; // bytes used by buf
} client;

client cli;
static void init_client(char** argv);
static void display_welcome_message();
static void user_input_message();
static void display_pull_message();
// figure out a way to check whether the message is complete

int main(int argc, char** argv){
    
    // Parse args.
    if(argc!=3){
        ERR_EXIT("usage: [ip] [port]");
    }

    // Handling connection
    init_client(argv);
    fprintf(stderr, "connect to %s %d\n", cli.ip, cli.port);
    int state = 0;
    char command[10]= "pull";
    // -1 is done reading
    while(1){
        if(state != 0) { scanf("%s", command); }
        if(strcmp(command, "post") == 0) {
            ssize_t bytesWritten = write(cli.conn_fd, command, strlen(command));
            char signal = ' ';
            while(signal != '@' && signal != '$') { read(cli.conn_fd, &signal, 1); }
            if(signal == '@') {
                record cur;
                printf("FROM: ");
                scanf("%s", cur.From);
                printf("CONTENT:\n");
                scanf("%s", cur.Content);
                fflush(stdout);
                write(cli.conn_fd, &cur, sizeof(record));
            }
            else { 
                printf("[Error] Maximum posting limit exceeded\n"); 
                fflush(stdout);
            }
            user_input_message();
        }
        else if(strcmp(command, "pull") == 0) {
            size_t bytesWritten = write(cli.conn_fd, command, strlen(command));
            size_t curbytes;
            cli.buf_len = 0;
            while(true) {
                curbytes = read(cli.conn_fd, cli.buf + cli.buf_len, BUFFER_SIZE);
                cli.buf_len += curbytes;
                if(cli.buf[cli.buf_len - 1] == '$') {
                    break;
                }
            }
            cli.buf_len -- ;
            if(state == 0) display_welcome_message();
            display_pull_message();
            user_input_message();
        }
        else if(strcmp(command, "exit") == 0) {
            ssize_t bytesWritten = write(cli.conn_fd, command, strlen(command));
            exit(0);

        }
        else { // handle exception
        }
        state = 1;
    }
}
void display_pull_message() {
    printf("==============================\n");
    record cur;
    for(int i = 0; i * sizeof(record) < cli.buf_len; i ++) {
        memcpy(&cur, cli.buf + i * sizeof(record), sizeof(record));
        printf("FROM: %s\n", cur.From);
        printf("CONTENT:\n%s\n", cur.Content);
    }
    printf("==============================\n");
    fflush(stdout);
    return;
}
void user_input_message() {
    const char *message = "Please enter your command (post/pull/exit): ";
    fprintf(stdout, "%s", message);
    fflush(stdout);
}
void display_welcome_message() { 
    // what is the exact format ??
    const char *message = 
        "==============================\n"
        "Welcome to CSIE Bulletin board\n";
    fprintf(stdout, "%s", message);
    fflush(stdout);
}


static void init_client(char** argv){
    
    cli.ip = argv[1];
    cli.buf_len = 0;

    if(atoi(argv[2])==0 || atoi(argv[2])>65536){
        ERR_EXIT("Invalid port");
    }
    cli.port=(unsigned short)atoi(argv[2]);

    struct sockaddr_in servaddr;
    cli.conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(cli.conn_fd<0){
        ERR_EXIT("socket");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(cli.port);

    if(inet_pton(AF_INET, cli.ip, &servaddr.sin_addr)<=0){
        ERR_EXIT("Invalid IP");
    }

    if(connect(cli.conn_fd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
        ERR_EXIT("connect");
    }

    return;
}

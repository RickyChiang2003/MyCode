#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 4096
#define RECORD_SIZE sizeof(record)

typedef struct {
    char* ip; // server's ip
    unsigned short port; // server's port
    int conn_fd; // fd to talk with server
    char buf[BUFFER_SIZE]; // data sent by/to server
    size_t buf_len; // bytes used by buf
} client;

client cli;
static void init_client(char** argv);

struct timeval timeout;

void client_pull(){
    lseek(cli.conn_fd, 0, SEEK_SET);
    write(cli.conn_fd, "[pull]", sizeof("[pull]"));

    printf("==============================\n");
    fflush(stdout);

    memset(cli.buf, 0, sizeof(cli.buf));
    // wait for server response
    while(strcmp(cli.buf, "") == 0){
        select(0, NULL, NULL, NULL, &timeout);
        lseek(cli.conn_fd, 0, SEEK_SET);
        read(cli.conn_fd, cli.buf, RECORD_SIZE);
    };

    while(1){  
        if(strcmp(cli.buf, "[next]") == 0){
            continue;
        }
        if(strcmp(cli.buf, "[end]") == 0){
            memset(cli.buf, 0, BUFFER_SIZE);
            break;
        }

        printf("FROM: %s\n", cli.buf);
        fflush(stdout);
        printf("CONTENT:\n%s\n", cli.buf+FROM_LEN);
        fflush(stdout);

        memset(cli.buf, 0, RECORD_SIZE);
        lseek(cli.conn_fd, 0, SEEK_SET);
        write(cli.conn_fd, "[next]", sizeof("[next]"));

        lseek(cli.conn_fd, 0, SEEK_SET);
        read(cli.conn_fd, cli.buf, RECORD_SIZE); 
    }

    printf("==============================\n");
    fflush(stdout);
}

void client_post(){
    lseek(cli.conn_fd, 0, SEEK_SET);
    write(cli.conn_fd, "[post]", sizeof("[post]"));

    memset(cli.buf, 0, sizeof(cli.buf));

    // wait for response
    while (strcmp(cli.buf, "[fail]") != 0 && strcmp(cli.buf, "[okay]") != 0){
        select(0, NULL, NULL, NULL, &timeout);
        lseek(cli.conn_fd, 0, SEEK_SET);
        read(cli.conn_fd, cli.buf, sizeof("[mode]"));
    }
    
    // cannot post
    if(strcmp(cli.buf, "[fail]") == 0){
        printf("[Error] Maximum posting limit exceeded\n");
        fflush(stdout);
        return;
    }
    else if(strcmp(cli.buf, "[okay]") == 0){
        record post_ = {0};

        printf("FROM: ");
        fflush(stdout);
        scanf("%s", post_.From);
        fflush(stdin);
        printf("CONTENT:\n");
        fflush(stdout);
        scanf("%s", post_.Content);
        fflush(stdin);

        lseek(cli.conn_fd, 0, SEEK_SET);
        write(cli.conn_fd, "[from]", sizeof("[from]"));

        lseek(cli.conn_fd, 0, SEEK_SET);
        write(cli.conn_fd, &post_, RECORD_SIZE);
    }
    
}

void client_exit(){
    lseek(cli.conn_fd, 0, SEEK_SET);
    write(cli.conn_fd, "[exit]", sizeof("[exit]"));
    exit(0);
}

int main(int argc, char** argv){

    timeout.tv_sec = 0;
    timeout.tv_usec = 200;
    
    // Parse args.
    if(argc!=3){
        ERR_EXIT("usage: [ip] [port]");
    }

    // Handling connection
    init_client(argv);
    fprintf(stderr, "connect to %s %d\n", cli.ip, cli.port);

    printf("==============================\n");
    fflush(stdout);
    printf("Welcome to CSIE Bulletin board\n");
    fflush(stdout);

    client_pull();

    while(1){
        // TODO: handle user's input
        char user_input[5];
        fprintf(stdout, "%s", "Please enter your command (post/pull/exit): ");
        fflush(stdout);

        scanf("%s", user_input);
        fflush(stdin);

        if(strcmp(user_input, "post") == 0){
            client_post();
        }else if(strcmp(user_input, "pull") == 0){
            client_pull();
        }else if(strcmp(user_input, "exit") == 0){
            client_exit();
        }
    }
}

static void init_client(char** argv){
    
    cli.ip = argv[1];

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


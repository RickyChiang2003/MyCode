#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>


#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 50000
#define COMMAND_SIZE 10
#define read_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define kill_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))


typedef struct {
    char* ip; // server's ip
    unsigned short port; // server's port
    int conn_fd; // fd to talk with server
    char buf[BUFFER_SIZE]; // data sent by/to server
    size_t buf_len; // bytes used by buf
} client;

client cli = {0};
struct timeval time_for_sleep; 
void init_buf(int len);
static void init_client(char** argv);
int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len);
int check_lock(int fd);
void print_divider();
void print_announcement();
int post_request();
void post_Content(record input);
void exit_server();


int main(int argc, char** argv){
    
    // Parse args.
    if(argc!=3){
        ERR_EXIT("usage: [ip] [port]");
    }

    // Handling connection
    init_client(argv);
    fprintf(stderr, "connect to %s %d\n", cli.ip, cli.port);
    
    //init lock, init time_for_sleep, init cli.conn_fd
    int ch = -1;
    ch = write_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when write lock at first");
    time_for_sleep.tv_sec = 0;
    time_for_sleep.tv_usec = 800;
    
    print_divider();
    puts("Welcome to CSIE Bulletin board");
    print_announcement();
    while(1){
        // TODO
	char command[COMMAND_SIZE] = {0};
	printf("Please enter your command (post/pull/exit): ");
	fflush(stdout);
	scanf("%s", command);
	fflush(stdin);
	
	switch(command[1]){
	    case 'o':  // post
		record input = {0};
		if(!post_request()){
		    printf("[Error] Maximum posting limit exceeded\n");
		    fflush(stdout);
		    break;
		}
		printf("FROM: ");
		fflush(stdout);
		scanf("%s", input.From);
		fflush(stdin);
		printf("CONTENT:\n");
		fflush(stdout);
		scanf("%s", input.Content);
		fflush(stdin);
		post_Content(input);
		break;
	    case 'u':  // pull
		print_announcement();
		break;
	    case 'x':  // exit
		exit_server();
		exit(1);
		break;
	    default:
		printf("You fucked up.\n");
		break;
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

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len){
    // same as course ppt
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;
    return (fcntl(fd,cmd,&lock));
}

int check_lock(int fd){
    struct flock chlk = {F_WRLCK,SEEK_SET,0,0,0};
    if(fcntl(fd,F_GETLK,&chlk) == -1)
	ERR_EXIT("check lock error");
    if(chlk.l_type == F_UNLCK){
	printf("no lock here yeahh????????????\n");
	return 1;
    }
    else
	return 0;
}

void init_buf(int len){
    for(int i = 0; i < len; i++)
	cli.buf[i] = 0;
}
void print_divider(){
    printf("==============================\n");
    fflush(stdout);
}
void print_announcement(){
    int ch = 0;
    print_divider();
    init_buf(COMMAND_SIZE);     
    cli.buf[0] = 'p';  // print request
    ch = write(cli.conn_fd, cli.buf, 1);
    ch = kill_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when killing lock that write 'p'");

    char c = 'p';
    do{
	select(0,NULL,NULL,NULL,&time_for_sleep);
	lseek(cli.conn_fd,0,SEEK_SET);
	read(cli.conn_fd,&c,1);
    } while(c != '!' && c != '@');
    write_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when setting write_lock");

    int type = 1;  // 0 for FROM and 1 for CONTENT
    int head = 1;
    int more_endl = 0;
    while(c != '@'){
	more_endl = 1;
	switch(c){
	    case '!':   // print type
		if(head == 1)
		    printf("FROM: "), head = 0,
		    fflush(stdout);
		else if(type == 0)
		    printf("\nFROM: "), type = (type == 1) ? 0 : 1,
		    fflush(stdout);
		else
		    printf("\nCONTENT:\n"), type = (type == 1) ? 0 : 1,
		    fflush(stdout);
		break;
	    case '\0':
		break;
	    default:    // print content
		putchar(c);
		break;
	}
	read(cli.conn_fd,&c,1);
    }

    if(more_endl)
	putchar('\n');
    print_divider();
}

int post_request(){
    init_buf(COMMAND_SIZE);
    cli.buf[0] = 'c';  // check request
    write(cli.conn_fd, cli.buf, 1);
    int ch = 0;
    ch = kill_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when kill lock in check request");

    char c = 'c';
    do{
	select(0,NULL,NULL,NULL,&time_for_sleep);
	lseek(cli.conn_fd,0,SEEK_SET);
	read(cli.conn_fd,&c,1);
    } while(c == 'c');
    ch = write_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when write lock after check");

    return ((int)c);
}

void post_Content(record input){
    cli.buf_len = FROM_LEN+CONTENT_LEN+5;
    init_buf(cli.buf_len);
    
    int ch = 0;
    int idx = 1;
    cli.buf[0] = 'i';  // input request
    for(int i = 0; i < FROM_LEN; i++, idx++)
	cli.buf[idx] = input.From[i];
    for(int i = 0; i < CONTENT_LEN; i++, idx++)
	cli.buf[idx] = input.Content[i];
    lseek(cli.conn_fd,0,SEEK_SET);
    write(cli.conn_fd, cli.buf, cli.buf_len);
    ch = kill_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when kill lock in Content");
    

    char c = 'c';
    do{
	select(0,NULL,NULL,NULL,&time_for_sleep);
	lseek(cli.conn_fd,0,SEEK_SET);
	read(cli.conn_fd,&c,1);
    } while(c != '#');
    ch = write_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when writting '#'");
}

void exit_server(){
    cli.buf[0] = 'e';  // exit request
    write(cli.conn_fd, cli.buf, 1);
    int ch = kill_lock(cli.conn_fd,0,SEEK_SET,0);
    if(ch != 0) ERR_EXIT("ch != 0 when writting exit");
}


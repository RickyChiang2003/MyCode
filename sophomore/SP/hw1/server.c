#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

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
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[BUFFER_SIZE];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    int id;
} request;

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

// initailize a server, exit for error
static void init_server(unsigned short port);

// initailize a request instance
static void init_request(request* reqP);

// free resources used by a request instance
static void free_request(request* reqP);

void init_BulletinBoard();
int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len);
int check_lock(int fd);

struct timeval time_for_sleep; 


int main(int argc, char** argv) {

    // Parse args.
    if (argc != 2) {
        ERR_EXIT("usage: [port]");
        exit(1);
    }

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd = -1;  // fd for a new connection with client
    int file_fd;  // fd for file that we open for reading
    char buf[BUFFER_SIZE];
    int buf_len;
    int last = 0;
    int mem = 0;

    //Init 
    int fd_table[MAX_CLIENTS];
    for(int i = 0; i < MAX_CLIENTS; i++)
	fd_table[i] = -1;
    
    fd_set readset;
    fd_set workset;
    FD_ZERO(&readset);
    
    file_fd = open(RECORD_PATH,O_RDWR);
    if(file_fd < 0) ERR_EXIT("file_fd open error");
    int flen = FROM_LEN + CONTENT_LEN;

    int Boardlock[RECORD_NUM] = {0};
    int writepoint[MAX_CLIENTS] = {0};
    int locallock = 0;

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Loop for handling connections
    //fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);

    while (1) {
	// TODO: Add IO multiplexing

	struct pollfd new_pfd = { svr.listen_fd, POLLIN, 0 };  //or POLLHUP?
	int ifnew = poll(&new_pfd,1,1);
	if(ifnew < 0) 
	    ERR_EXIT("poll of new_pfd return a negative number");
	if(ifnew == 1){	
	    // Check new connection
	    clilen = sizeof(cliaddr);
	    conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
	    if (conn_fd < 0) {
		if (errno == EINTR || errno == EAGAIN) continue;  // try again
		if (errno == ENFILE) {
		    (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
		    continue;
		}
		ERR_EXIT("accept");
	    }
	    requestP[conn_fd].conn_fd = conn_fd;
	    strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
	    //fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);
	    int i;
	    for(i = 0; i < MAX_CLIENTS; i++)
		if(fd_table[i] == -1){
		    fd_table[i] = conn_fd;
		    FD_SET(conn_fd, &readset);
		    mem++;
		    break;
		}
	    if(i == MAX_CLIENTS) ERR_EXIT("fd_table overflow");
	}

	time_for_sleep.tv_sec = 0;
	time_for_sleep.tv_usec = 300;
	memcpy(&workset, &readset, sizeof(readset));
	int sel = select(3+maxfd, &workset, NULL, NULL, &time_for_sleep);
	if(mem > 0 && sel == -1) ERR_EXIT("sel == -1, error");

	if(mem > 0 && sel > 0){
	// TODO: handle requests from clients

	    for(int cnt = 0; cnt < MAX_CLIENTS; cnt++){
		if(fd_table[cnt] == -1)
		    continue;
		if(!(FD_ISSET(fd_table[cnt], &workset)))
		    continue;
		conn_fd = fd_table[cnt];

		char c = 0;
		lseek(conn_fd,0,SEEK_SET);
		read(conn_fd,&c,1);
		if(!(c == 'p' || c == 'c' || c == 'i' || c == 'e')){
		    kill_lock(conn_fd, 0, SEEK_SET, 0);
		    continue;
		}


		switch(c){
		    case 'p':  // pull
			int num_of_lock = 0;
			int idx = 0;           
			/////////////////////////////////////////////////////////////////////
			for(int i = 0; i < RECORD_NUM; i++){
			    if(Boardlock[i] == 0){
				if(read_lock(file_fd, i*flen, SEEK_SET, flen) == 0){
				    lseek(file_fd, i*flen, SEEK_SET);
				    read(file_fd, buf, flen);
				    int empty = 1;
				    for(int j = 0; j < flen; j++)
					if(buf[j] > 0){
					    empty = 0;
					    break;
					}
				    if(!empty){
					requestP[conn_fd].buf[idx] = '!';idx++;
					for(int j = 0; j < FROM_LEN; j++, idx++)
					    requestP[conn_fd].buf[idx] = buf[j];
					requestP[conn_fd].buf[idx] = '!';idx++;
					for(int j = FROM_LEN; j < flen; j++, idx++)
					    requestP[conn_fd].buf[idx] = buf[j];
				    }
				    kill_lock(file_fd,i*flen,SEEK_SET, flen);
				}
				else
				    num_of_lock ++;
			    }
			    else
				num_of_lock ++;
			}
			requestP[conn_fd].buf[idx] = '@'; idx++;

			if(num_of_lock > 0){
			    printf("[Warning] Try to access locked post - %d\n",num_of_lock);
			    fflush(stdout);  /////////////////////////////////////////////////////////
			}
			for(int i = 0; i < idx; i++){
			    if(requestP[conn_fd].buf[i] < '!' && requestP[conn_fd].buf[i] != ' ')
				requestP[conn_fd].buf[i] = 0;
			}
			lseek(conn_fd,0,SEEK_SET);
			write(conn_fd, requestP[conn_fd].buf, idx);

			break;

		    case 'c':  // check for post  ////////////////////////////////////////////////////
			char tmp_c = 0;
			for(int i = 0; i < RECORD_NUM; i++){
			    last = (last+1)%RECORD_NUM;
			    if(Boardlock[(last+RECORD_NUM-1)%RECORD_NUM] == 0)
				if(write_lock(file_fd, ((last+RECORD_NUM-1)%RECORD_NUM)*flen, SEEK_SET, flen) == 0){
				    Boardlock[(last+RECORD_NUM-1)%RECORD_NUM] = 1;
				    writepoint[conn_fd] = (last+RECORD_NUM-1)%RECORD_NUM;
				    locallock += 1;
				    tmp_c = 1;
				    break;
				}
			}
			lseek(conn_fd,0,SEEK_SET);
			write(conn_fd, &tmp_c, 1);
			kill_lock(conn_fd,0,SEEK_SET,0);		
			break;

		    case 'i':  // input from post
			write_lock(conn_fd,0,SEEK_SET,0);		
			lseek(conn_fd,1,SEEK_SET);
			read(conn_fd, buf, flen);

			lseek(file_fd,(writepoint[conn_fd])*flen,SEEK_SET);
			write(file_fd, buf, flen);
			for(int i = 0; i < flen; i++)
			    if(buf[i] < '!' && buf[i] != ' ')
				buf[i] = 0;
			char zero = '#';
			lseek(conn_fd,0,SEEK_SET);
			write(conn_fd, &zero, 1);
			kill_lock(conn_fd,0,SEEK_SET,0);		
			kill_lock(file_fd,(writepoint[conn_fd])*flen,SEEK_SET,flen);		
			Boardlock[writepoint[conn_fd]] = 0;
			locallock -= 1;
			printf("[Log] Receive post from %s\n", buf);
			fflush(stdout);
			break;

		    case 'e':  // exit
			FD_CLR(conn_fd,&readset);
			close(requestP[conn_fd].conn_fd);
			free_request(&requestP[conn_fd]);
			int i;
			for(i = 0; i < MAX_CLIENTS; i++)
			    if(fd_table[i] == conn_fd){
				fd_table[i] = -1;
				mem--;
				break;
			    }
			//puts("Bye~");
			break;

		    default:
			ERR_EXIT("WTF did you read from conn_id ???");
			break;	    
		}
		kill_lock(conn_fd,0,SEEK_SET,0);		
	    }
	}
    }
    close(file_fd);
    free(requestP);
    return 0;
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
	fflush(stdout);
	return 1;
    }
    else
	return 0;
}

void init_buf(char* buf, int len){
    for(int i = 0; i < len; i++)
	buf[i] = 0;
}

void init_BulletinBoard(){
    int fd_init = open(RECORD_PATH, O_RDWR);
    int len = (FROM_LEN + CONTENT_LEN)*RECORD_NUM;
    char* buf_init = calloc(len, sizeof(char));
    write(fd_init,buf_init,len);
    close(fd_init);
    free(buf_init);

}

// ======================================================================================================
// You don't need to know how the following codes are working

static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->id = 0;
}

static void free_request(request* reqP) {
    init_request(reqP);
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }

    // Get file descripter table size and initialize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (int i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    return;
}

#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <stdbool.h>
#include <poll.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 4096
#define RECORD_SIZE sizeof(record)

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

fd_set clientfd_set, working_set;
struct timeval timeout;

// initailize a server, exit for error
static void init_server(unsigned short port);

// initailize a request instance
static void init_request(request* reqP);

// free resources used by a request instance
static void free_request(request* reqP);

// type: read F_RDLCK    write F_WRLCK     unlock F_UNLCK
int lock(int fd, int pos, short type){
    struct flock lock = {
        .l_len = RECORD_SIZE,
        .l_start = pos * RECORD_SIZE,
        .l_type = type,
        .l_whence = SEEK_SET,
    };

    return fcntl(fd, F_SETLK, &lock);
}

#define write_lock(fd, pos) lock(fd, pos, F_WRLCK)
#define read_lock(fd, pos) lock(fd, pos, F_RDLCK)
#define unlock(fd, pos) lock(fd, pos, F_UNLCK)

bool can_post(int fd, int pos){
    struct flock lock = {
        .l_len = RECORD_SIZE,
        .l_start = pos * RECORD_SIZE,
        .l_whence = SEEK_SET,
    };

    // check write lock
    lock.l_type = F_WRLCK;
    if(fcntl(fd, F_GETLK, &lock) < 0){
        perror("get write lock error");
        return false;
    }
    // this pos is write locked
    if(lock.l_type != F_UNLCK) return false;

    // check read lock
    lock.l_type = F_RDLCK;
    if(fcntl(fd, F_GETLK, &lock) < 0){
        perror("get read lock error");
        return false;
    }
    // this pos is read locked
    if(lock.l_type != F_UNLCK) return false;

    // can post here
    return true;
}

bool have_post(int fd, int pos){
    lseek(fd, pos * RECORD_SIZE, SEEK_SET);
    record tmp = {0};
    read(fd, &tmp, RECORD_SIZE);
    // a record is considered empty and not a post if both of its attributes, 
    // From and Content, are empty string ""
    if(strcmp(tmp.From, "") == 0 && strcmp(tmp.Content, "") == 0) return false;
    return true;
}

void init_BulletinBoard(){
    int fd_init = open(RECORD_PATH, O_RDWR);
    int len = (FROM_LEN + CONTENT_LEN)*RECORD_NUM;
    char* buf_init = calloc(len, sizeof(char));
    write(fd_init,buf_init,len);
    close(fd_init);
    free(buf_init);

}

int main(int argc, char** argv) {

    // init_BulletinBoard();


    // Parse args.
    if (argc != 2) {
        ERR_EXIT("usage: [port]");
        exit(1);
    }

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd = open(RECORD_PATH, O_RDWR);  // fd for file that we open for reading
    char buf[BUFFER_SIZE];
    int buf_len;

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);
    
    FD_ZERO(&clientfd_set);
    //FD_SET(svr.listen_fd, &clientfd_set);             /////////////////////changed

    int last = -1;
    int conn_amount = 0;
    int client_sockfd[MAX_CLIENTS] = {0};
    int ret;

    while (1) {
        // TODO: Add IO multiplexing
	for(int i = 0; i < MAX_CLIENTS; ++i) {
	    if(client_sockfd[i] != 0) {
		FD_SET(client_sockfd[i], &clientfd_set);
	    }
	}

        struct pollfd p_fd = {svr.listen_fd, POLLIN, 0};
        ret = poll(&p_fd, 1, 0);
        
        if(ret == 1){
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
            fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);

            for(int i=0;i<MAX_CLIENTS;i++){
                if(client_sockfd[i] == 0){
                    conn_amount++;
                    client_sockfd[i] = conn_fd;
                    FD_SET(conn_fd, &clientfd_set);
                    break;
                }
            }
            FD_SET(conn_fd, &clientfd_set);
        }
        // copy clientfd_set to working_set
        memcpy(&working_set, &clientfd_set, sizeof(clientfd_set));
	timeout.tv_sec = 0;                                                //////////changed
	timeout.tv_usec = 200;                                             //////////changed
        ret = select(MAX_CLIENTS + 5, &working_set, NULL, NULL, &timeout); //////////changed
        if(conn_amount > 0 && ret > 0){
            // TODO: handle requests from clients
            for(int i=0;i<MAX_CLIENTS;i++){
                if(FD_ISSET(client_sockfd[i], &working_set)){

                    int cur_fd = client_sockfd[i];
                    lseek(cur_fd, 0, SEEK_SET);
                    read(cur_fd, requestP[cur_fd].buf, sizeof("[mode]"));

                    // client input for post request
                    if(strcmp(requestP[cur_fd].buf, "[from]") == 0 && requestP[cur_fd].id >= 0){
                        while(1){
                            lseek(cur_fd, 0, SEEK_SET);
                            read(cur_fd, requestP[cur_fd].buf, RECORD_SIZE);
                            if(strcmp(requestP[cur_fd].buf, "[from]") != 0){
                                break;
                            }
                        }
                        
                        record post_ = {0};
                        memcpy(&post_, requestP[cur_fd].buf, RECORD_SIZE);
                        // move to id_th record position
                        lseek(file_fd, requestP[cur_fd].id * RECORD_SIZE, SEEK_SET);
                        // write client's post to bulletin board
                        write(file_fd, &post_, RECORD_SIZE);
                        // unlock write lock when client post request done
                        unlock(file_fd, requestP[cur_fd].id);
                        requestP[cur_fd].id = -1;
                        printf("[Log] Receive post from %s\n", post_.From);
                        fflush(stdout);
                        break;
                    }
                    // post request
                    else if(strcmp(requestP[cur_fd].buf, "[post]") == 0){
                        int cnt = 0;
                        int cur_pos = last+1;
                        while(cnt < RECORD_NUM){
                            // first unlocked pos
                            if(can_post(file_fd, cur_pos)){
                                break;
                            }
                            cur_pos = (cur_pos + 1) % RECORD_NUM;
                            cnt++;
                        }
                        // all pos are locked
                        if(cnt == RECORD_NUM){
                            // tell client cannot post
                            lseek(cur_fd, 0, SEEK_SET);
                            write(cur_fd, "[fail]", sizeof("[fail]"));
                        }
                        // can post at cur_pos
                        else{
                            // write lock the pos until client post request done
                            write_lock(file_fd, cur_pos);
                            // tell client can post
                            lseek(cur_fd, 0, SEEK_SET);
                            write(cur_fd, "[okay]", sizeof("[okay]"));
                            requestP[cur_fd].id = cur_pos;
                            last = cur_pos;
                        }
                        break;
                    }
                    // pull request
                    else if(strcmp(requestP[cur_fd].buf, "[pull]") == 0){
                        int locked_post = 0;
                        for(int i=0;i<RECORD_NUM;i++){
                            if(!have_post(file_fd, i)){
                                continue;
                            }

                            if(read_lock(file_fd, i) == 0){
                                memset(buf, 0, sizeof(buf));
                                // sent current record to client
                                lseek(file_fd, i * RECORD_SIZE, SEEK_SET);
                                read(file_fd, buf, RECORD_SIZE);
                                lseek(cur_fd, 0, SEEK_SET);
                                write(cur_fd, buf, RECORD_SIZE);

                                // unlock record position
                                unlock(file_fd, i);

                                // wait for client to signal next send
                                char signal[7] = {0};
                                while(1){
                                    lseek(cur_fd, 0, SEEK_SET);
                                    read(cur_fd, signal, sizeof("[mode]")); 
                                    if(strcmp(signal, "[next]") == 0){
                                        break;
                                    }
                                }
                            }
                            else{
                                locked_post++;
                            }
        
                        }
                        if(locked_post){
                            printf("[Warning] Try to access locked post - %d\n", locked_post);
                            fflush(stdout);
                        }
                        // signal all pull complete
                        lseek(cur_fd, 0, SEEK_SET);
                        write(cur_fd, "[end]", sizeof("[end]"));
                        break;
                    }
                    // exit request
                    else if(strcmp(requestP[cur_fd].buf, "[exit]") == 0){
                        conn_amount--;
                        FD_CLR(cur_fd, &clientfd_set);
                        close(requestP[cur_fd].conn_fd);
                        free_request(&requestP[cur_fd]);
                        break;
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
    close(file_fd);
    free(requestP);
    return 0;
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


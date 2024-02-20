#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <stdbool.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 512
#define START_FD 0

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
int accessible[RECORD_NUM] = {0};
struct timeval timeout;

const char ok = '@', bad = '$';
fd_set masterset, childset;
// initailize a server, exit for error
static void init_server(unsigned short port);

// initailize a request instance
static void init_request(request* reqP);

// free resources used by a request instance
static void free_request(request* reqP);

// ======================================================================================================

void close_connection(int fd) {

    FD_CLR(fd, &masterset);
    close(requestP[fd].conn_fd);
    free_request(&requestP[fd]);
}
void apply_write_lock(int fd, off_t start, off_t len) {
    struct flock lock;

    lock.l_type = F_WRLCK;  // Read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = start;  // Start of the file
    lock.l_len = len;  // Lock the entire file (0 means to the end of the file)

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to apply write lock");
        exit(1);
    }
}
void apply_read_lock(int fd, off_t start, off_t len) {
    struct flock lock;

    lock.l_type = F_RDLCK;  // Read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = start;  // Start of the file
    lock.l_len = len;  // Lock the entire file (0 means to the end of the file)

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to apply read lock");
        exit(1);
    }
}
int is_unlocked(int fd, off_t start, off_t len) {
    struct flock lock;
    lock.l_type = F_WRLCK;  // Check for write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;

    if (fcntl(fd, F_GETLK, &lock) == -1) {
        perror("Failed to check lock status");
        return -1;
    }

    return (lock.l_type == F_UNLCK);
}
void release_lock(int fd, int start, int len) {
    struct flock lock;
    lock.l_type = F_UNLCK;  // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = start;  // Start of the file
    lock.l_len = len;    // Unlock the entire file (0 means to the end of the file)

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Failed to release the lock");
        exit(1);
    }
}
size_t get_size(int file_fd) {
    struct stat file_info;
    fstat(file_fd, &file_info);
    off_t size = file_info.st_size;
    return size;
}
int is_writable(int fd, off_t start, off_t len) {
    struct flock lock;

    lock.l_type = F_WRLCK;  // We're initially checking for write locks
    lock.l_whence = SEEK_SET;
    lock.l_start = start;
    lock.l_len = len;

    // Check for write lock
    if (fcntl(fd, F_GETLK, &lock) == -1) {
        perror("Failed to check lock status for write lock");
        return -1;  // Error
    }
    if (lock.l_type != F_UNLCK) {
        return 0;  // The region is locked for writing
    }

    lock.l_type = F_RDLCK;
    if (fcntl(fd, F_GETLK, &lock) == -1) {
        perror("Failed to check lock status for read lock");
        return -1;  // Error
    }
    if (lock.l_type != F_UNLCK) {
        return 0;  // The region is locked for reading
    }
    return 1;  // The region is unlocked for both reading and writing
}
bool isempty(int file_fd, int id) {
    char empty[] = "";
    record cur;
    lseek(file_fd, id * sizeof(record), SEEK_SET);
    read(file_fd, &cur, sizeof(record));
    return strcmp(empty, cur.From) == 0 && strcmp(empty, cur.Content) == 0;


}
// ======================================================================================================
int main(int argc, char** argv) {

    // Parse args.
    if (argc != 2) {
        ERR_EXIT("usage: [port]");
        exit(1);
    }

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd;  // fd for file that we open for reading
    char buf[BUFFER_SIZE];
    int buf_len;
    int last_idx = 0;
    FD_ZERO(&masterset);
    FD_ZERO(&childset);
    // Initialize server
    init_server((unsigned short) atoi(argv[1]));
    file_fd = open(RECORD_PATH, O_RDWR);
    FD_SET(svr.listen_fd, &masterset);
    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);


    while (1) {
        // IO multiplexing
        for(int i = START_FD; i < START_FD + MAX_CLIENTS + 5; i ++) {
            if(FD_ISSET(i, &masterset)) { FD_SET(i, &childset); }
            else { FD_CLR(i, &childset); }
        }
        int numofevents = select(FROM_LEN + MAX_CLIENTS + 5, &childset, NULL,  NULL, NULL);

        // Handle Requests 
        if(numofevents > 0) {
            int curfd = -1;
            for(int i = START_FD; i < START_FD + MAX_CLIENTS + 5; i ++) {
                if(FD_ISSET(i, &childset)) {
                    curfd = i;
                    break;
                }
            }
            assert(curfd != -1);
            if(curfd == svr.listen_fd) {
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
                requestP[conn_fd].id = 0;
                FD_SET(conn_fd, &masterset);
                strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
                fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);
            }
            else if(requestP[curfd].id > 0) {
                int curid = requestP[curfd].id - 1;
                size_t curbytes = 0;
                curbytes = read(curfd, requestP[curfd].buf, sizeof(record));
                record cur;
                memcpy(&cur, requestP[curfd].buf, sizeof(record));
                lseek(file_fd, curid * sizeof(record), SEEK_SET);
                write(file_fd, requestP[curfd].buf, sizeof(record));
                fprintf(stdout, "[Log] Receive post from %s\n", cur.From);
                fflush(stdout);
                release_lock(file_fd, curid * sizeof(record), sizeof(record));
                accessible[curid] = 0;
                requestP[curfd].id = 0;
            }
            else {

                requestP[curfd].buf_len = read(curfd, requestP[curfd].buf, 4);
                if(strcmp(requestP[curfd].buf, "post") == 0) {
                    // check the behaviour of posts
                    size_t  cursize = get_size(file_fd);
                    int num = 0, nxt_empty = -1, nxt_unlock = -1;
                    record cur;
                    char empty[] = "";
                    for(int i = last_idx; i < last_idx + RECORD_NUM && (nxt_empty == -1); i ++) {
                        int id = i % RECORD_NUM;
                        if(is_writable(file_fd, (i % RECORD_NUM) * sizeof(record), sizeof(record)) && accessible[id] == 0) {
                            num = 1;
                            if(nxt_empty == -1 && isempty(file_fd, id)) { nxt_empty = id; }
                            if(nxt_unlock == -1) nxt_unlock = id;
                        }
                    }
                    if(num) {
                        last_idx = (nxt_empty == -1 ? nxt_unlock : nxt_empty);
                        apply_write_lock(file_fd, last_idx * sizeof(record), sizeof(record));
                        accessible[last_idx] = -1;
                        write(curfd, &ok, 1);
                        requestP[curfd].id = last_idx + 1;
                        last_idx = (last_idx + 1) % RECORD_NUM;
                    }
                    else { write(curfd, &bad, 1); }
                }
                else if(strcmp(requestP[curfd].buf, "pull") == 0) {
                    int bad = 0, num = 0;
                    // deal with the cases
                    for(int i = 0; i  < RECORD_NUM; i ++ ) {
                        if(is_unlocked(file_fd, i * sizeof(record), sizeof(record)) && accessible[i] != -1) {
                            if(isempty(file_fd, i)) continue;
                            apply_read_lock(file_fd, i * sizeof(record), sizeof(record));
                            lseek(file_fd, i * sizeof(record), SEEK_SET);
                            read(file_fd, buf + num * sizeof(record), sizeof(record));
                            num ++;
                            release_lock(file_fd, i * sizeof(record), sizeof(record));
                        }
                        else bad += 1;
                    }
                    if(bad) { fprintf(stdout, "[Warning] Try to access locked post - %d\n", bad); fflush(stdout);}
                    buf[num * sizeof(record)] = '$';
                    write(curfd, buf, num * sizeof(record) + 1);
                }
                else if(strcmp(requestP[curfd].buf, "exit") == 0) {
                    close_connection(curfd);
                } 
            }
        }
    }
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

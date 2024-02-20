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
#define BUFFER_SIZE 512
#define COMMAND_SIZE 10
#define read_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define write_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define kill_lock(fd, offset, whence, len) \
    lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))


void init_BulletinBoard();
int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len);
int check_lock(int fd);

int main() {
    sleep(3);
    int file_fd = open(RECORD_PATH,O_RDWR);
    if(file_fd < 0) ERR_EXIT("file_fd open error");
    int flen = FROM_LEN + CONTENT_LEN;
    sleep(3);
    if(1 == check_lock(file_fd))
	printf("no lock\n");
    int c = write_lock(file_fd,0,SEEK_SET,0);
    if(c != 0) printf("write_lock() fail\n");
    printf("before sleep\n");
    sleep(20);
    printf("end sleep\n");
    check_lock(file_fd);
     c = write_lock(file_fd,0,SEEK_SET,0);
    if(c != 0) printf("write_lock() fail\n");
    else printf("write_lock ok\n");
    check_lock(file_fd);
    sleep(20);
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
	return 1;
    }
    else
	return 0;
}

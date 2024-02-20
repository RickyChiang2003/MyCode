#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "util.h"

#define ERR_EXIT(s) perror(s), exit(errno);

static unsigned long secret;
static char service_name[MAX_SERVICE_NAME_LEN];

static inline bool is_manager() {
    return strcmp(service_name, "Manager") == 0;
}

void print_not_exist(char *service_name) {
    printf("%s doesn't exist\n", service_name);
}

void print_receive_command(char* service_name, char* cmd) {
    printf("%s has received %s\n", service_name, cmd);
}

void print_spawn(char *parent_name, char *child_name) {
    printf("%s has spawned a new service %s\n", parent_name, child_name);
}

void print_kill(char *target_name, int decendents_num) {
    printf("%s and %d child services are killed\n", target_name, decendents_num);
}

void print_acquire_secret(char *service_a, char *service_b, unsigned long secret) {
    printf("%s has acquired a new secret from %s, value: %lu\n", service_a, service_b, secret);
}

void print_exchange(char *service_a, char *service_b) {
    printf("%s and %s have exchanged their secrets\n", service_a, service_b);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./service [service_name]\n");
        return 0;
    }

    pid_t pid = getpid();
    srand(pid);
    secret = rand();
    setvbuf(stdout, NULL, _IONBF, 0);
    strncpy(service_name, argv[1], MAX_SERVICE_NAME_LEN);
    printf("%s has been spawned, pid: %d, secret: %lu\n", service_name, pid, secret);

    int p_RDfd, p_WRfd;
    if(is_manager()){
	p_RDfd = 0;
	p_WRfd = 1;
    }
    else{
	p_RDfd = PARENT_READ_FD;
	p_WRfd = PARENT_WRITE_FD;
	char create = '1';
	write(p_WRfd, &create, 1);
    }


    char cmd[MAX_CMD_LEN] = {0};
    service child[MAX_CHILDREN] = {0};
    int c_num = 0;

    while(read(p_RDfd, cmd, sizeof(cmd)) > 0){
	char subcmd[3][MAX_SERVICE_NAME_LEN] = {0};
	int sub_i = 0, sub_j = 0;
	for(int i = 0, jump = 1; i < MAX_CMD_LEN; i++){
	    if(cmd[i] >= '!'){
		if(sub_i == 3)
		    ERR_EXIT("jump too much time, Haiyaa~~");
		subcmd[sub_i][sub_j] = cmd[i];
		sub_j++;
		jump = 1;
	    }
	    else if(jump){
		sub_i++;
		sub_j = 0;
		jump = 0;
	    }
	}
	char printcmdbuf[128] = {0};
	for(int i = 0, idx = 0, pidx = 0; i < sub_i; idx = 0, i++){
	    while(subcmd[i][idx] >= '!'){
		printcmdbuf[pidx] = subcmd[i][idx];
		idx++;
		pidx++;
	    }
	    if(i < sub_i-1){
		printcmdbuf[pidx] = ' ';
		pidx++;
	    }
	}


	if(strcmp(subcmd[0], "spawn") == 0){
	    print_receive_command(service_name, printcmdbuf);

	    char p_ser[MAX_SERVICE_NAME_LEN] = {0};
	    char c_ser[MAX_SERVICE_NAME_LEN] = {0};
	    strncpy(p_ser,subcmd[1],sizeof(p_ser));
	    strncpy(c_ser,subcmd[2],sizeof(c_ser));
	    if(strcmp(service_name, p_ser) == 0){  // this is target
		if(c_num == MAX_CHILDREN)
		    ERR_EXIT("exist too many children");

		int RDfd[2], WRfd[2];
		//pipe2(RDfd,0);  // or O_CLOEXEC ???
		//pipe2(WRfd,0);  // or O_CLOEXEC ???
		pipe2(RDfd,O_CLOEXEC);
		pipe2(WRfd,O_CLOEXEC);

		child[c_num].pid = fork();
		if(child[c_num].pid < 0){
		    ERR_EXIT("fork failed");
		}
		else if(child[c_num].pid == 0){  // child
		    close(RDfd[1]);
		    close(WRfd[0]);
		    dup2(RDfd[0],PARENT_READ_FD);
		    dup2(WRfd[1],PARENT_WRITE_FD);
		    //for(int i = 5; i < 100; i++)
		    //	close(i);
		    int flags1 = fcntl(PARENT_READ_FD, F_GETFD, 0);
		    int flags2 = fcntl(PARENT_WRITE_FD, F_GETFD, 0);
		    flags1 &= ~FD_CLOEXEC;
		    flags2 &= ~FD_CLOEXEC;
		    fcntl(PARENT_READ_FD, F_SETFD, flags1);
		    fcntl(PARENT_WRITE_FD, F_SETFD, flags2);
		    execlp("./service", "./service", c_ser, NULL);
		    _exit(0);
		}
		else{  // parent
		    child[c_num].read_fd = WRfd[0];  // parent  read from child
		    child[c_num].write_fd = RDfd[1]; // parent write form child
		    close(RDfd[0]);
		    close(WRfd[1]);
		    strcpy(child[c_num].name,c_ser);

		    char create = '0';
		    read(child[c_num].read_fd, &create, 1);
		    if(create != '1') {printf("%s: create return = %c\n",service_name, create); exit(0);}
		    c_num++;
		    if(is_manager()){
			print_spawn(p_ser, c_ser);
		    }
		    else{
			char found = '1';
			write(p_WRfd, &found, 1);
			write(p_WRfd, &create, 1);
		    }

		}
	    }
	    else{
		char found = '0';
		int s_i = 0;
		for(int i = 0; i < c_num && found == '0'; i++){
		    char outbuf[MAX_CMD_LEN] = {0};
		    sprintf(outbuf, "spawn %s %s\0", p_ser, c_ser);
		    write(child[i].write_fd, outbuf, MAX_CMD_LEN);
		    read(child[i].read_fd, &found, 1);
		    if(found == '1')
			s_i = i;
		}

		if(is_manager()){
		    if(found == '0')
			print_not_exist(p_ser);
		    if(found == '1'){
			char create = '0';
			read(child[s_i].read_fd, &create, 1);
			if(create != '1') {printf("%s: create return = %c\n",service_name, create); exit(0);}
			print_spawn(p_ser, c_ser);
		    }
		}
		else if(found == '0'){
		    write(p_WRfd, &found, 1);
		}
		else if(found == '1'){
		    write(p_WRfd, &found, 1);
		    char create = '0';
		    read(child[s_i].read_fd, &create, 1);
		    if(create != '1') {printf("%s: create return = %c\n",service_name, create); exit(0);}
		    write(p_WRfd, &create, 1);
		}
		else
		    ERR_EXIT("found == (not '0' and not '1')");
	    }
	    
	}
	else if(strcmp(subcmd[0], "kill") == 0){
	    char k_ser[MAX_SERVICE_NAME_LEN] = {0};
	    strncpy(k_ser,subcmd[1],sizeof(k_ser));

	    if(strcmp(service_name, k_ser) == 0){  // this is target
		if(!is_manager()){
		    char checkistar = '2';
		    read(p_RDfd, &checkistar, 1);
		    if(checkistar == '0')
			print_receive_command(service_name, printcmdbuf);
		    else if(checkistar != '1'){printf("%s: checkistar = %c\n",service_name, checkistar);exit(0);}
		}
		else{
		    print_receive_command(service_name, printcmdbuf);
		}

		int sum_k_num = 0;
		char istar = '1';
		for(int i = 0; i < c_num; i++){
		    char k_num[3] = {0};
		    char check = '1';
		    char outbuf[MAX_CMD_LEN] = {0};
		    sprintf(outbuf, "kill %s\0", child[i].name);
		    write(child[i].write_fd, outbuf, sizeof(outbuf));
		    write(child[i].write_fd, &istar, 1);
		    read(child[i].read_fd, k_num, 3);
		    sum_k_num += (int)(k_num[1]-'0');
		    close(child[i].read_fd);
		    write(child[i].write_fd, &check, 1);
		}
		for(int i = 0; i < c_num; i++){
		    waitpid(child[i].pid,NULL,0);
		    close(child[i].write_fd);
		}
		if(is_manager()){
		    print_kill("Manager", sum_k_num);
		}
		else{
		    char k_num[3] = {'1','0'+sum_k_num+1,0}, check = '0';
		    write(p_WRfd, k_num, 3);
		    read(p_RDfd, &check, 1);
		    if(check != '1'){printf("%s: check = %c\n",service_name, check); exit(0);}
		}
		exit(0);
	    }
	    else{  // not target
		print_receive_command(service_name, printcmdbuf);
		if(!is_manager()){
		    char checkistar = '1';
		    read(p_RDfd, &checkistar, 1);
		    if(checkistar != '0'){printf("%s: checkistar = %c\n",service_name, checkistar);exit(0);}
		}
		int k_i = -1;
		char k_return[3] = {'0','0','0'};
		char istar = '0';
		char outbuf[MAX_CMD_LEN] = {0};
		sprintf(outbuf, "kill %s\0", k_ser);
		for(int i = 0; i < c_num; i++){
		    write(child[i].write_fd, outbuf, sizeof(outbuf));
		    write(child[i].write_fd, &istar, 1);
		    read(child[i].read_fd, k_return, 3);
		    if(k_return[0] == '1'){
			k_i = i;
			break;
		    }
		}
		if(k_return[0] != '1' && k_return[0] != '0'){
		    ERR_EXIT("check k_return from child fucked up");
		}
		if(is_manager()){
		    if(k_return[0] == '0')
			print_not_exist(k_ser);
		    if(k_return[0] == '1'){
			char check = '0';
			check = '1';
			write(child[k_i].write_fd, &check, 1);
			print_kill(k_ser, k_return[1]-'0'-1);
		    }
		}
		else{  // not Manager
		    write(p_WRfd, k_return, 3);
		    if(k_return[0] == '1'){
			char check = '0';
			check = '1';
			read(p_RDfd, &check, 1);
			if(check != '1')
			    ERR_EXIT("check kill in nonkiller return 2 fucked up");
			if(k_i == -1)
			    ERR_EXIT("k_i should not = -1");
			write(child[k_i].write_fd, &check, 1);
		    }
		}
		if(strcmp(child[k_i].name, k_ser) == 0){  // if have to kill a child
		    close(child[k_i].read_fd);
		    close(child[k_i].write_fd);
		    memset(&(child[k_i]), 0, sizeof(child[k_i]));
		    for(int i = k_i; i < c_num-1; i++){
			child[i].pid = child[i+1].pid;
			child[i].read_fd = child[i+1].read_fd;
			child[i].write_fd = child[i+1].write_fd;
			strcpy(child[i].name,child[i+1].name);
		    }  
		    c_num--;
		}
	    }
	}
	else if(strcmp(subcmd[0], "exchange") == 0){
	    print_receive_command(service_name, printcmdbuf);

	    char a_ser[MAX_SERVICE_NAME_LEN] = {0};
	    char b_ser[MAX_SERVICE_NAME_LEN] = {0};
	    strncpy(a_ser,subcmd[1],sizeof(a_ser));
	    strncpy(b_ser,subcmd[2],sizeof(b_ser));
	    char fifoab[MAX_FIFO_NAME_LEN] = {0};
	    char fifoba[MAX_FIFO_NAME_LEN] = {0};
	    sprintf(fifoab, "%s_to_%s.fifo\0",a_ser,b_ser);
	    sprintf(fifoba, "%s_to_%s.fifo\0",b_ser,a_ser);

	    if(is_manager()){
		unlink(fifoab);
		unlink(fifoba);
		mkfifo(fifoab, 0666);
		mkfifo(fifoba, 0666);

		char mode = '0';
		if(strcmp(service_name, a_ser) == 0)
		    mode = 'a';
		if(strcmp(service_name, b_ser) == 0)
		    mode = 'b';

		int i_e[2] = {-1,-1}, i_ei = 0;
		char e_num[3] = {0};
		char outbuf[MAX_CMD_LEN] = {0}, outbuf_num = '0';
		sprintf(outbuf, "exchange %s %s\0",a_ser, b_ser);
		int found_num = (mode == '0') ? 0 : 1;
		for(int i = 0; i < c_num && found_num < 2; i++){
		    outbuf_num = '0'+found_num;
		    write(child[i].write_fd, outbuf, sizeof(outbuf));
		    write(child[i].write_fd, &outbuf_num, 1);
		    read(child[i].read_fd, e_num, 3);
		    int d = (int)(e_num[0]-'0');
		    if(d > found_num){
			i_e[i_ei] = i;
			i_ei++;
			found_num = d;
		    }
		    if(found_num > 2) {printf("error: found_num in manager = %d\n", found_num);exit(0);}
		}
		char e_req = '1';
		for(int i = 0; i < 2; i++)
		    if(i_e[i] != -1)
			write(child[i_e[i]].write_fd, &e_req, 1);  // tell target service to output and input
		//fifo

		char secretin[MAX_FIFO_NAME_LEN] = {0};
		char secretout[MAX_FIFO_NAME_LEN] = {0};
		for(int i = 0, tmp = secret; tmp > 0; tmp /= 10, i += 1)
		    secretout[i] = '0'+(tmp%10);
		int fab, fba, newsecret;
		switch(mode){
		    case 'a':
			fab = open(fifoab, O_WRONLY);
			fba = open(fifoba, O_RDONLY);
			read(fba, secretin, sizeof(secretin));
			close(fba);
			newsecret = 0;
			for(int i = 0, p10 = 1; secretin[i] > '\0'; i++){
			    newsecret += p10*(secretin[i]-'0');
			    p10 *= 10;
			}
			secret = newsecret;
			print_acquire_secret(a_ser, b_ser, secret);
			write(fab, secretout, sizeof(secretout));
			close(fab);
			break;
		    case 'b':
			fab = open(fifoab, O_RDONLY);
			fba = open(fifoba, O_WRONLY);
			write(fba, secretout, sizeof(secretout));
			close(fba);
			read(fab, secretin, sizeof(secretin));
			close(fab);
			newsecret = 0;
			for(int i = 0, p10 = 1; secretin[i] > '\0'; i++){
			    newsecret += p10*(secretin[i]-'0');
			    p10 *= 10;
			}
			secret = newsecret;
			print_acquire_secret(b_ser, a_ser, secret);
			break;
		    case '0':
			break;
		    default:
			printf("Excuse me, wtf is going on here why the f'ck the mode will be '%c' ??????????????\n",mode);
			exit(0);
			break;
		}

		for(int i = 0; i < 2; i++)
		    if(i_e[i] != -1){
			char check = '0';
			read(child[i_e[i]].read_fd, &check, 1);  // tell target service to output and input
			if(check != '1') {printf("error: check in manager = '%c'\n",check);exit(0);}
		    }
		unlink(fifoba);
		unlink(fifoab);
		print_exchange(a_ser, b_ser);
	    }
	    else{  // not Manager
		char mode = '0';
		if(strcmp(service_name, a_ser) == 0)
		    mode = 'a';
		if(strcmp(service_name, b_ser) == 0)
		    mode = 'b';
		
		int i_e[2] = {-1,-1}, i_ei = 0;
		char e_num[3] = {0};
		char outbuf[MAX_CMD_LEN] = {0}, outbuf_num = '0', inbuf_num = 0;
		sprintf(outbuf, "exchange %s %s\0",a_ser, b_ser);
		read(p_RDfd, &inbuf_num, 1);
		int found_num = inbuf_num-'0'+((mode == '0')?0:1);
		for(int i = 0; i < c_num && found_num < 2; i++){
		    outbuf_num = '0'+found_num;
		    write(child[i].write_fd, outbuf, sizeof(outbuf));
		    write(child[i].write_fd, &outbuf_num, 1);
		    read(child[i].read_fd, e_num, 3);
		    int d = (int)(e_num[0]-'0');
		    if(d > found_num){
			i_e[i_ei] = i;
			i_ei++;
			found_num = d;
		    }
		    if(found_num > 2) {printf("error: found_num in manager = %d\n", found_num);exit(0);}
		}
		e_num[0] = '0'+found_num;
		write(p_WRfd, e_num, 3);

		if(i_ei == 0 && mode == '0'){
		    memset(cmd, 0, sizeof(cmd));
		    continue;
		}
		char e_req = '0';
		read(p_RDfd, &e_req, 1);
		if(e_req != '1') {printf("%s error: e_req in manager = %c\n",service_name, e_req);exit(0);}
		for(int i = 0; i < 2; i++)
		    if(i_e[i] != -1)
			write(child[i_e[i]].write_fd, &e_req, 1);  // tell target service to output and input
		
		//fifo
		char secretin[MAX_FIFO_NAME_LEN] = {0};
		char secretout[MAX_FIFO_NAME_LEN] = {0};
		for(int i = 0, tmp = secret; tmp > 0; tmp /= 10, i += 1)
		    secretout[i] = '0'+(tmp%10);
		int fab, fba, newsecret;
		switch(mode){
		    case 'a':
			fab = open(fifoab, O_WRONLY);
			fba = open(fifoba, O_RDONLY);
			read(fba, secretin, sizeof(secretin));
			close(fba);
			newsecret = 0;
			for(int i = 0, p10 = 1; secretin[i] > '\0'; i++){
			    newsecret += p10*(secretin[i]-'0');
			    p10 *= 10;
			}
			secret = newsecret;
			print_acquire_secret(a_ser, b_ser, secret);
			write(fab, secretout, sizeof(secretout));
			close(fab);
			break;
		    case 'b':
			fab = open(fifoab, O_RDONLY);
			fba = open(fifoba, O_WRONLY);
			write(fba, secretout, sizeof(secretout));
			close(fba);
			read(fab, secretin, sizeof(secretin));
			close(fab);
			newsecret = 0;
			for(int i = 0, p10 = 1; secretin[i] > '\0'; i++){
			    newsecret += p10*(secretin[i]-'0');
			    p10 *= 10;
			}
			secret = newsecret;
			print_acquire_secret(b_ser, a_ser, secret);
			break;
		    case '0':
			break;
		    default:
			printf("Excuse me, wtf is going on here why the f'ck the mode will be '%c' ??????????????\n",mode);
			exit(0);
			break;
		}
		char check;
		for(int i = 0; i < 2; i++)
		    if(i_e[i] != -1){
			check = '0';
			read(child[i_e[i]].read_fd, &check, 1);
			if(check != '1') {printf("error: check in manager = '%c'\n",check);exit(0);}
		    }
		check = '1';
		write(p_WRfd, &check, 1);
	    }
	}
	else{
	    ERR_EXIT("~~~~~~~~~~~~~~~~~~~~~~You fucked up, Haiyaa~~~~~~~~~~~~~~~~~~~~~~");
	}
	

	memset(cmd, 0, sizeof(cmd));
    }


    return 0;
}

#ifndef THREADTOOL
#define THREADTOOL
#include <setjmp.h>
#include <sys/signal.h>
#include "bank.h"


#define THREAD_MAX 16  // maximum number of threads created
#define BUF_SIZE 512
struct tcb {
    int id;  // the thread id
    jmp_buf environment;  // where the scheduler should jump to
    int arg;  // argument to the function
    int i, x, y;  // declare the variables you wish to keep between switches
};



extern int timeslice;
extern jmp_buf sched_buf;
extern struct tcb *ready_queue[THREAD_MAX], *waiting_queue[THREAD_MAX];
extern struct Bank bank;
/*
 * rq_size: size of the ready queue
 * rq_current: current thread in the ready queue
 * wq_size: size of the waiting queue
 */
extern int rq_size, rq_current, wq_size;
/*
* base_mask: blocks both SIGTSTP and SIGALRM
* tstp_mask: blocks only SIGTSTP
* alrm_mask: blocks only SIGALRM
*/
extern sigset_t base_mask, tstp_mask, alrm_mask;
/*
 * Use this to access the running thread.
 */
#define RUNNING (ready_queue[rq_current])

void sighandler(int signo);
void scheduler();

// TODO
#define thread_create(func, id, arg) {\
    rq_size++;\
    rq_current = rq_size-1;\
    func(id, arg);\
}

#define thread_setup(id, arg) {\
    printf("%d %s\n", id, __func__);\
    RUNNING = calloc(1, sizeof(*RUNNING));\
    RUNNING->id = id;\
    RUNNING->arg = arg;\
    RUNNING->i = RUNNING->x = RUNNING->y = 1;\
    int jmp = sigsetjmp(RUNNING->environment, 1);\
    if(jmp == 0)\
	return;\
    else if(jmp != 4){\
	printf("You fucked up in thread_setup, haiyaa~~~~\n");\
	exit(0);\
    }\
}

#define thread_exit() {\
    free(RUNNING);\
    siglongjmp(sched_buf, 3);\
}

#define thread_yield() {\
    int jmp = sigsetjmp(RUNNING->environment, 1);\
    if(jmp == 0){\
	sigprocmask(SIG_SETMASK, &alrm_mask, NULL);\
	sigprocmask(SIG_SETMASK, &tstp_mask, NULL);\
	sigprocmask(SIG_SETMASK, &base_mask, NULL);\
    }\
    else if(jmp != 4){\
	printf("You fucked up in thread_yield, haiyaa~~~~\n");\
	exit(0);\
    }\
}

#define lock(){\
    if(bank.lock_owner == -1)\
	bank.lock_owner = RUNNING->id;\
    else if(bank.lock_owner != RUNNING->id)\
	siglongjmp(sched_buf, 2);\
}

// should I leave this parentheses?
#define unlock() ({\
    if(RUNNING->id == bank.lock_owner)\
	bank.lock_owner = -1;\
    else{\
	printf("You fucked up in unlock, haiyaa~~~~\n");\
	exit(0);\
    }\
})

#endif // THREADTOOL

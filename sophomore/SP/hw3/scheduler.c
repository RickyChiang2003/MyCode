#include "threadtools.h"
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Print out the signal you received.
 * If SIGALRM is received, reset the alarm here.
 * This function should not return. Instead, call siglongjmp(sched_buf, 1).
 */
void sighandler(int signo) {
    // TODO
    switch(signo){
	case SIGTSTP:
	    printf("caught SIGTSTP\n");
	    break;
	case SIGALRM:
	    printf("caught SIGALRM\n");
	    alarm(0);  // Do I need this?
	    alarm(timeslice);
	    break;
	default:
	    printf("You fucked up in sighandler, haiyaa~~~~\n");
	    exit(0);
    }
    siglongjmp(sched_buf, 1);
}



/*
 * Prior to calling this function, both SIGTSTP and SIGALRM should be blocked.
 */
void scheduler() {
    // TODO
    rq_current = 0;
    while (rq_size > 0){  // in spec, it is (rq_size > 0 || wq_size > 0)
	int jmp = sigsetjmp(sched_buf, 1);
	//printf("jmp=%d, rq_current=%d, rq_size=%d\n",jmp,rq_current,rq_size);
	switch(jmp){
	    case 0:  
		siglongjmp(RUNNING->environment, 4);
		break;
	    case 1:  // sighandler() from thread_yield()
		if (bank.lock_owner == -1 && wq_size > 0){
		    ready_queue[rq_size] = waiting_queue[0];
		    rq_size++;
		    for(int i = 1; i < wq_size; i++)
			waiting_queue[i-1] = waiting_queue[i];
		    wq_size--;
		    bank.lock_owner = ready_queue[rq_size-1]->id;
		}
		rq_current = (rq_current+1) % rq_size;
		break;
	    case 2:  // lock()
		if (bank.lock_owner == -1 && wq_size > 0){
		    ready_queue[rq_size] = waiting_queue[0];
		    bank.lock_owner = ready_queue[rq_size]->id;
		    rq_size++;
		    for(int i = 1; i < wq_size; i++)
			waiting_queue[i-1] = waiting_queue[i];
		    wq_size--;
		}
		waiting_queue[wq_size] = RUNNING;
		wq_size++;
		rq_size--;
		RUNNING = ready_queue[rq_size];
		rq_current = (rq_current == rq_size) ? 0 : rq_current;
		break;
	    case 3:  // thread_exit()
		if (bank.lock_owner == -1 && wq_size > 0){
		    ready_queue[rq_size] = waiting_queue[0];
		    bank.lock_owner = ready_queue[rq_size]->id;
		    rq_size++;
		    for(int i = 1; i < wq_size; i++)
			waiting_queue[i-1] = waiting_queue[i];
		    wq_size--;
		}
		rq_size--;
		RUNNING = ready_queue[rq_size];
		rq_current = (rq_current == rq_size) ? 0 : rq_current;
		break;
	    default:
		printf("You fucked up in scheduler, haiyaa~~~~\n");
		exit(0);
	}

    }
}

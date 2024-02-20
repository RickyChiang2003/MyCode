#include "threadtools.h"
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void fibonacci(int id, int arg) {
    thread_setup(id, arg);

    for (RUNNING->i = 1; ; RUNNING->i++) {
        if (RUNNING->i <= 2)
            RUNNING->x = RUNNING->y = 1;
        else {
            /* We don't need to save tmp, so it's safe to declare it here. */
            int tmp = RUNNING->y;
            RUNNING->y = RUNNING->x + RUNNING->y;
            RUNNING->x = tmp;
        }
        printf("%d %d\n", RUNNING->id, RUNNING->y);
        sleep(1);

        if (RUNNING->i == RUNNING->arg) {
            thread_exit();
        }
        else {
            thread_yield();
        }
    }
}

void factorial(int id, int arg){
    // TODO
    thread_setup(id, arg);

    for (RUNNING->i = RUNNING->x = 1; ; RUNNING->i++) {
	RUNNING->x *= RUNNING->i;
	printf("%d %d\n", RUNNING->id, RUNNING->x);
	sleep(1);

	if(RUNNING->i == RUNNING->arg) {
	    thread_exit();
	}
	else {
	    thread_yield();
	}
    }
}


void bank_operation(int id, int arg) {
    // TODO
    thread_setup(id, arg);
    
    // Step 1
    lock();
    printf("%d acquired the lock\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // Step 2
    int tmp = bank.balance;
    if(RUNNING->arg > 0)
	bank.balance += RUNNING->arg;  // deposit
    else if (RUNNING->arg < 0 && bank.balance+RUNNING->arg >= 0)
	bank.balance += RUNNING->arg;  // withdraw
    printf("%d %d %d\n", RUNNING->id, tmp, bank.balance);
    sleep(1);
    thread_yield();

    // Step 3
    unlock();
    printf("%d released the lock\n", RUNNING->id);
    sleep(1);
    thread_exit();
}

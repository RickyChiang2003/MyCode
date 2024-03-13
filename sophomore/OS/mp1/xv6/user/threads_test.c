#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0
// no next thread: current_thread->next == current_thread
// if only one thread, curr == prev == next

static struct thread* current_thread = NULL;
static int id = 1;
static jmp_buf env_st;
// static jmp_buf env_tmp;

struct thread *thread_create(void (*f)(void *), void *arg){
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    t->fp = f;
    t->arg = arg;
    t->ID  = id;
    t->buf_set = 0;
    t->stack = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;
    return t;
}
void thread_add_runqueue(struct thread *t){
    if(current_thread == NULL){
        // TODO
	t->next = t->previous = t;
	current_thread = t;
    }
    else{
        // TODO
	t->previous = current_thread->previous;
	t->next = current_thread;
	current_thread->previous->next = t;
	current_thread->previous = t;
    }
}
void thread_yield(void){
    // TODO
    // return value == 2
    int jmp = setjmp(current_thread->env);
    if(jmp == 0){
	schedule();
	dispatch();
    }
    else if(jmp != 2){
	printf("You fucked up in thread_yield, haiyaa~~~~\n");
	exit(0);
    }
}
void dispatch(void){
    // TODO
    if(current_thread->buf_set == 0){
	current_thread->env->sp = (unsigned long)(current_thread->stack_p);
	current_thread->buf_set = 1;
	(current_thread->fp)(current_thread->arg);
    }
    else{
	longjmp(current_thread->env, 2); // jump to thread_yield
    }
    thread_exit(); // if the function return here
}
void schedule(void){
    // TODO
    printf("schedule from thread %d\n", current_thread->ID);
    current_thread = current_thread->next;
    printf("schedule to thread %d\n", current_thread->ID);
}
void thread_exit(void){
    if(current_thread->next != current_thread){
        // TODO
	current_thread->previous->next = current_thread->next;
	current_thread->next->previous = current_thread->previous;
	struct thread* free_thread = current_thread;
	current_thread = current_thread->next;
	free(free_thread->stack);
	free(free_thread);
	dispatch();
    }
    else{
        // TODO
        // Hint: No more thread to execute
	free(current_thread->stack);
	free(current_thread);
	longjmp(env_st, 1);
    }
}
void thread_start_threading(void){
    // TODO
    int jmp = setjmp(env_st);
    if(jmp == 0){
	current_thread->buf_set = 1;
	current_thread->env->sp = (unsigned long)(current_thread->stack_p);
	(current_thread->fp)(current_thread->arg);
    }
    else if(jmp != 1){
	printf("You fucked up in thread_start_threading, haiyaa~~~~\n");
    }
    exit(0);
}


void ft(void *arg)
{
    int i = 10000;
    while (1) {
        printf("thread 2: %d\n", i++);
        if(i == 10005){
            thread_exit();
        }
        thread_yield();
    }
}
void f1(void *arg)
{
    int i = 100;
    struct thread *tt = thread_create(ft, NULL);
    thread_add_runqueue(tt);
    while(1) {
        printf("thread 1: %d\n", i++);
        if (i == 110) {
            thread_exit();
        }
        thread_yield();
    }
}
int main(){
    printf("start test\n");
    struct thread *t1 = thread_create(f1, NULL);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\ntest done\n");
    exit(0);
}


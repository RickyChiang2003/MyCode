#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0
// no next thread: current_thread->next == current_thread
// no next task: ->next == NULL or ->previous == NULL
// if only one thread, curr == prev == next
// longjmp(,5) === set malloc memory

static struct thread* current_thread = NULL;
static struct thread* current_task = NULL;
static jmp_buf env_st;
static int task_on = 0;
static int id = 1;

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
    
    t->task_set = 0;
    t->is_task = 0;
    t->task_p = NULL;
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
    // return value == 2, 3(task)
    //printf("line 55, in thread_yield, task_on = %d\n",task_on);
    if(task_on == 0){
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
    else{  // task on
	int jmp = setjmp(current_task->env);
	if(jmp == 0){
	    task_on = 0;
	    schedule();
	    dispatch();
	}
	else if(jmp != 3){
	    printf("You fucked up in thread_yield, haiyaa~~~~\n");
	    exit(0);
	}
	else
	    task_on = 1;
    }
}

void dispatch(void){
    // TODO
    task_on = (current_thread->task_set == 0) ? 0 : 1;
    //printf("line 87, in dispatch, task_on = %d\n", task_on);
    
    if(task_on == 0){
	if(current_thread->buf_set == 0){
	    int jmp = setjmp(current_thread->env);
	    if(jmp == 0){
		current_thread->env->sp = (unsigned long)(current_thread->stack_p);
		longjmp(current_thread->env, 5);
	    }
	    else if(jmp == 5){
		current_thread->buf_set = 1;
		(current_thread->fp)(current_thread->arg);
	    }
	    else
		printf("You fucked up in dispatch, haiyaa~~~~\n");
	}
	else
	    longjmp(current_thread->env, 2); // jump to thread_yield
	thread_exit(); // if the function return here
    }	
    else{ // task on
	current_task = current_thread->task_p;
	while(current_task->next != NULL)
	    current_task = current_task->next;
	if(current_task->buf_set == 0){
	    int jmp = setjmp(current_task->env);
	    if(jmp == 0){
		current_task->env->sp = (unsigned long)(current_task->stack_p);
		longjmp(current_task->env, 5);
	    }
	    else if(jmp == 5){
		current_task->buf_set = 1;
		(current_task->fp)(current_task->arg);
	    }
	    else
		printf("You fucked up in dispatch, haiyaa~~~~\n");
	}
	else
	    longjmp(current_task->env, 3);
	task_exit();		
    }
}

void schedule(void){
    // TODO
    current_thread = current_thread->next;
}
void thread_exit(void){
    if(current_thread->next != current_thread){
        // TODO
	current_thread->previous->next = current_thread->next;
	current_thread->next->previous = current_thread->previous;
	struct thread* free_thread = current_thread;
	schedule();
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
void task_exit(void){
    if(current_task->next != NULL)
	current_task->next->previous = current_task->previous;
    if(current_task->previous != NULL)
	current_task->previous->next = current_task->next;
    struct thread* free_task = current_task;
    if(current_task->next == NULL && current_task->previous == NULL){
	current_thread->task_p = NULL;
	task_on = 0;
	current_thread->task_set = 0;
    }
    else{
	if(current_task->next != NULL)
	    while(current_task->next != NULL)
		current_task = current_task->next;
	else
	    current_task = current_task->previous;
    }
    free(free_task->stack);
    free(free_task);
    dispatch();
}
void thread_start_threading(void){
    // TODO
    int jmp_st = setjmp(env_st);
    if(jmp_st == 0){
	int jmp = setjmp(current_thread->env);
	if(jmp == 0){
	    current_thread->env->sp = (unsigned long)(current_thread->stack_p);
	    longjmp(current_thread->env, 5);
	}
	else if(jmp == 5){
	    current_thread->buf_set = 1;
	    (current_thread->fp)(current_thread->arg);
	}
	else
	    printf("You fucked up in thread_start_threading, haiyaa~~~~\n");
	thread_exit();
    }
    else if(jmp_st != 1){
	printf("You fucked up in thread_start_threading, haiyaa~~~~\n");
    }
}
// part 2
void thread_assign_task(struct thread *t, void (*f)(void *), void *arg){
    // TODO
    struct thread *prev = NULL;
    struct thread *new = (t->is_task == 0) ? t->task_p : t;
    while(new != NULL){
	prev = new;
	new = new->next;
    }
    new = (struct thread*)malloc(sizeof(struct thread));
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    new->fp = f;
    new->arg = arg;
    new->stack = (void*) new_stack;
    new->stack_p = (void*) new_stack_p;
    new->ID = id;
    id++;

    new->buf_set = 0;
    new->is_task = 1;
    new->task_p = NULL;
    new->next = NULL;
    new->previous = prev;
    if(prev!=NULL)
	new->previous->next = new;
    if(t->task_p == NULL){
	t->task_p = new;
	t->task_set = 1;
    }
}

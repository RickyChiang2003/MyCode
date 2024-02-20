#include "my_pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thd_func(void* new){
    tpool* pool = new;
    pthread_mutex_lock(&(pool->m));
    while(!(pool->q_head == NULL && pool->done == 1)){
	if(pool->q_head == NULL)
	    pthread_cond_wait(&(pool->c), &(pool->m));
	if(pool->q_head == NULL && pool->done == 1) 
	    break;

	// take the job and wake other thread
	if(pool->q_head == NULL)
	    continue;
	else if(pool->q_head->next != NULL)  // ensure thd_func only run if job exist
	    pthread_cond_signal(&(pool->c));
	
	void* (*func)(void*) = pool->q_head->func;
	void* arg = pool->q_head->arg;
	pool->q_head = pool->q_head->next;
	pthread_mutex_unlock(&(pool->m));

	(*func)(arg);
	
	pthread_mutex_lock(&(pool->m));
    }
    pthread_mutex_unlock(&(pool->m));
    pthread_cond_signal(&(pool->c));
    return NULL;
}

void tpool_add(tpool *pool, void *(*func)(void *), void *arg) {
  // TODO
    pthread_mutex_lock(&(pool->m));
    pool->q_tail->next = calloc(1, sizeof(llist));
    pool->q_tail = pool->q_tail->next;
    pool->q_tail->next = NULL;
    pool->q_tail->func = func;
    pool->q_tail->arg = arg;
    if(pool->q_head == NULL)  // ensure thd_func only run if job exist
	pool->q_head = pool->q_tail;
    
    pthread_mutex_unlock(&(pool->m));
    pthread_cond_signal(&(pool->c));
}

void tpool_wait(tpool *pool) {
  // TODO
    pthread_mutex_lock(&(pool->m));
    pool->done = 1;
    pthread_mutex_unlock(&(pool->m));
    pthread_cond_signal(&(pool->c));
    for(int i = 0; i < pool->n_threads; i++){
	pthread_join(pool->arr[i], NULL);
    }
}

void tpool_destroy(tpool *pool) {
  // TODO
    pthread_mutex_destroy(&(pool->m));
    pthread_cond_destroy(&(pool->c));
    free(pool->arr);
    llist* tmp = pool->q_free;
    while(tmp != NULL){
	llist* tmp2 = tmp->next;
	free(tmp);
	tmp = tmp2;
    }
    free(pool);
}

tpool *tpool_init(int n_threads) {
  // TODO
    tpool* new = calloc(1, sizeof(*new));
    pthread_mutex_init(&(new->m),NULL);
    pthread_cond_init(&(new->c),NULL);
    pthread_attr_init(&(new->attr));  // do I need this?
    new->arr = calloc(n_threads, sizeof(*(new->arr)));
    new->n_threads = n_threads;
    new->q_head = NULL;
    new->q_tail = calloc(1, sizeof(llist));
    new->q_free = new->q_tail;
    new->done = 0;

    for(int i = 0; i < n_threads; i++)
	pthread_create(&(new->arr[i]), &(new->attr), thd_func, (void*)new);

    return new;
}

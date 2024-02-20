#include <pthread.h>
#ifndef __MY_THREAD_POOL_H
#define __MY_THREAD_POOL_H

typedef struct llist {
    struct llist* next;
    void* (*func)(void *);
    void* arg;
} llist;

typedef struct tpool {
  // TODO: define your structure
    pthread_mutex_t m;
    pthread_cond_t c;
    pthread_attr_t attr;
    pthread_t* arr;
    int n_threads;
    llist* q_head;
    llist* q_tail;
    llist* q_free;
    int done;
} tpool;

tpool *tpool_init(int n_threads);
void tpool_add(tpool *, void *(*func)(void *), void *);
void tpool_wait(tpool *);
void tpool_destroy(tpool *);

#endif

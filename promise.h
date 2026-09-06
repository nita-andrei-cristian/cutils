#ifndef NITA_PROMISE_UTIL
#define NITA_PROMISE_UTIL

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    _Bool ready;
    void *result;
} Promise;

void promise_init(Promise *p);

// Called by the CALLER thread — blocks until the worker fulfills it
void *promise_wait(Promise *p);

// Called by the WORKER thread — fulfills the promise and wakes the waiter
void promise_fulfill(Promise *p, void *result);

void promise_destroy(Promise *p);

#endif

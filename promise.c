#include "promise.h"

// Warning! This is an AI-generated promise util

void promise_init(Promise *p) {
    pthread_mutex_init(&p->lock, NULL);
    pthread_cond_init(&p->cond, NULL);
    p->ready = 0;
    p->result = NULL;
    p->error = 0;
}

// Called by the CALLER thread — blocks until the worker fulfills it
void *promise_wait(Promise *p) {
    pthread_mutex_lock(&p->lock);
    while (!p->ready) {
        pthread_cond_wait(&p->cond, &p->lock);
    }
    void *result = p->result;
    pthread_mutex_unlock(&p->lock);
    return result;
}

// Called by the WORKER thread — fulfills the promise and wakes the waiter
void promise_fulfill(Promise *p, void *result) {
    pthread_mutex_lock(&p->lock);
    p->result = result;
    p->ready = 1;
    pthread_cond_signal(&p->cond);   // or broadcast if multiple waiters
    pthread_mutex_unlock(&p->lock);
}

void promise_destroy(Promise *p) {
    pthread_mutex_destroy(&p->lock);
    pthread_cond_destroy(&p->cond);
}

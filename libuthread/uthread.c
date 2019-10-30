#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

/*declared a queue for threads in ready and zombie states
declared a tcb for the running thread
enumerated the different states
tid_create starts at 0 and keeps track of the assignment of tids
*/
static struct queue * ready_q, * zombie_q;
static struct tcb * running_tcb;
enum states {
    ready,
    running,
    blocked,
    zombie
};
static uthread_t tid_create;

/*in the tcb struct, we store the tid, context, a pointer to the top of the stack,
the function, the argument, the return value, and the enumerated state*/
struct tcb {
    uthread_t tid;
    uthread_ctx_t tcb_context;
    void * ptr;
    uthread_func_t tcb_func;
    void * tcb_arg;
    int tcb_retval;
    enum states state;

};

/*dequeues the first element of the ready_q and sets it as the running thread
enqueues the previous running thread into the ready_q with the state ready
then it context switches with the context of these 2 tcbs as arguments
*/
void uthread_yield(void) {
    struct tcb * current_tcb = calloc(1, sizeof(struct tcb));

    queue_dequeue(ready_q, (void ** ) & current_tcb);
    running_tcb->state = ready;
    queue_enqueue(ready_q, running_tcb);


    struct tcb * temp = calloc(1, sizeof(struct tcb));
    temp = running_tcb;

    current_tcb->state = running;
    running_tcb = current_tcb;
    running_tcb->state = running;


    uthread_ctx_switch( & temp->tcb_context, & running_tcb->tcb_context);

}

//returns the tid of the running tcb
uthread_t uthread_self(void) {
    return running_tcb->tid;
}

/*when uthread_create is called for the first time, it must initialize the 2 queues
and also the running tcb
the main thread is created with tid 0 and set to the running tcb*/
void main_thread(uthread_func_t func, void * arg) {

    ready_q = queue_create();
    zombie_q = queue_create();

    struct tcb * new_tcb = calloc(1, sizeof(struct tcb));

    new_tcb->tid = tid_create;
    tid_create++;

    new_tcb->state = running;

    running_tcb = calloc(1, sizeof(struct tcb));
    running_tcb = new_tcb;

}

/*allocate space for a new tcb
initializes its context to the given function and arguments
and then enqueues it in the ready queue*/
int uthread_create(uthread_func_t func, void * arg) {
    struct tcb * new_tcb = calloc(1, sizeof(struct tcb));

    if (tid_create == 0) {
        main_thread(func, arg);
    }

    new_tcb->tid = tid_create;
    tid_create++;

    new_tcb->tcb_func = func;
    new_tcb->tcb_arg = arg;
    new_tcb->ptr = uthread_ctx_alloc_stack();
    uthread_ctx_init( & new_tcb->tcb_context, new_tcb->ptr, func, arg);
    new_tcb->state = ready;

    queue_enqueue(ready_q, new_tcb);

    return new_tcb->tid;
}

/*dequeues the first element of the ready queue and sets it to the running tcb
The previous running tcb is enqueued into the zombie queue
Then we call context switch with the contexts of these 2 as arguments*/
void uthread_exit(int retval) {

    struct tcb * new_run = calloc(1, sizeof(struct tcb));
    struct tcb * old_run = calloc(1, sizeof(struct tcb));

    queue_dequeue(ready_q, (void ** ) & new_run);

    old_run = running_tcb;
    old_run->state = zombie;
    queue_enqueue(zombie_q, old_run);

    running_tcb = new_run;
    new_run->state = running;

    uthread_ctx_switch( & old_run->tcb_context, & new_run->tcb_context);

}

/*an infinite loop that yields until the ready queue is empty
when the ready queue is empty it breaks and returns 0*/
int uthread_join(uthread_t tid, int * retval) {
    while (1) {
        if (queue_length(ready_q) == 0) {
            return 0;
        }
        uthread_yield();
    }
    return 0;
}

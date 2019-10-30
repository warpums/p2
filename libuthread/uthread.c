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

static struct queue *ready_q, *zombie_q, *running_q;
static struct tcb *running_tcb;
enum states{ready, running, blocked, zombie};
static uthread_t tid_create;

struct tcb{
        uthread_t tid;
        uthread_ctx_t tcb_context;
        void *ptr;
        uthread_func_t tcb_func;
        void *tcb_arg;
        int tcb_retval;
        enum states state;

};

void uthread_yield(void)
{
        //disable preempt
        struct tcb *current_tcb = calloc(1, sizeof(struct tcb));

        queue_dequeue(ready_q, (void**)&current_tcb);
        running_tcb->state = ready;
        queue_enqueue(ready_q, running_tcb);

        printf("ready dq tid = %d\n", (int)current_tcb->tid);

        struct tcb *temp = calloc(1, sizeof(struct tcb));
        temp = running_tcb;

        current_tcb->state = running;
        running_tcb = current_tcb;
        running_tcb->state = running;

        printf("new running tid = %d\n", (int)running_tcb->tid);

        uthread_ctx_switch(&temp->tcb_context, &running_tcb->tcb_context);

        printf("ready queue length = %d\n", queue_length(ready_q));

        //enable preempt
}

uthread_t uthread_self(void)
{
        return running_tcb->tid;
}

void main_thread(uthread_func_t func, void *arg){

        running_q = queue_create();
        ready_q = queue_create();
        zombie_q = queue_create();

        struct tcb *new_tcb = calloc(1, sizeof(struct tcb));

        new_tcb->tid = tid_create;
        tid_create++;

        //new_tcb->ptr = uthread_ctx_alloc_stack();
        //uthread_ctx_init(&new_tcb->tcb_context, new_tcb->ptr, func, arg);
        new_tcb->state = running;

        running_tcb = calloc(1, sizeof(struct tcb));
        running_tcb = new_tcb;

        printf("main tid = %d\n", (int)running_tcb->tid);
}


int uthread_create(uthread_func_t func, void *arg)
{
        struct tcb *new_tcb = calloc(1, sizeof(struct tcb));

        if(tid_create == 0){
                main_thread(func, arg);
        }

                //struct tcb *new_tcb = calloc(1, sizeof(struct tcb));

        new_tcb->tid = tid_create;
        tid_create++;

        new_tcb->tcb_func = func;
        new_tcb->tcb_arg = arg;
        new_tcb->ptr = uthread_ctx_alloc_stack();
        uthread_ctx_init(&new_tcb->tcb_context, new_tcb->ptr, func, arg);
        new_tcb->state = ready;

        queue_enqueue(ready_q, new_tcb);

        printf("new ready tid = %d\n", (int)new_tcb->tid);

        printf("running tid = %d\n", (int)running_tcb->tid);

        return new_tcb->tid;
}

void uthread_exit(int retval)
{

        /*if(queue_length(running_q) == 0){
                exit(0);
        }

        running_tcb->tcb_retval = retval;

        struct tcb *current_tcb = calloc(1, sizeof(struct tcb));

        queue_dequeue(running_q, (void**)&current_tcb);
        current_tcb->state = zombie;
        queue_enqueue(zombie_q, current_tcb);*/

        printf("ENTERED EXIT\n");

        struct tcb *new_run = calloc(1, sizeof(struct tcb));
        struct tcb *old_run = calloc(1, sizeof(struct tcb));

        queue_dequeue(ready_q, (void**)&new_run);

        old_run = running_tcb;
        old_run->state = zombie;
        queue_enqueue(zombie_q, old_run);

        running_tcb = new_run;
        new_run->state = running;

        uthread_ctx_switch(&old_run->tcb_context, &new_run->tcb_context);

}

/*int find_tid(void *data, uthread_t arg)
{
        struct tcb *current_tcb = calloc(1, sizeof(struct tcb));

        current_tcb = data;

        if(current_tcb->tid == arg)
                return 1;
        else
                return 0;
}*/

int uthread_join(uthread_t tid, int *retval)
{
        /*if(tid == 0 || ready_q->first->tid == tid){
                return -1;
        }

        struct tcb *found_tcb = calloc(1, sizeof(tcb));
        found_tcb = NULL;

        queue_iterate(zombie_q, find_tid, tid, &found_tcb);

        if(found_tcb != NULL){
                //collect
        }else{
                return -1;
        }*/

        while(1){
                if(queue_length(ready_q) == 0){
                        printf("exiting join\n");
                        return 0;
                }
                printf("join yielding\n");
                uthread_yield();
        }
        return 0;
}

/*int main(){

        return 0;

}*/

## Phase 1:

### Design:
We initially thought of using a linked-list implementation, then decided to
use a doubly linked-list to improve efficiency of queue functionality.
Choosing a doubly linked-list helped in our implementation of queue functions
because we could easily retrieve the previous node even in the middle of the
queue. As a refresher to doubly linked-list, we looked online at GeeksforGeeks
and spoke with TAs at office hours.

### Testing:
We first tested each function independently to ensure that each behaved
properly by itself. As was recommended, we changed the data of each node from
void pointers to ints. We then combined multiple functions in a complex test,
making extensive use of printf statements to compare output with expected
values. When we were satisfied that all queue functions performed correctly we
changed the data of nodes back to void pointers and ran the tests again. We
then used the provided test functions, and expanded on them.

## Phase 2:

### Design:
We first brainstormed the appropriate data structure for the thread control
blocks. We decided to create a struct that holds the thread id, its context,
a pointer to the top of the allocated stack, the specified function, the
specified argument, the return val, and its enumerated state. We initially
thought we'd need four queues, one for each of the states. We eventually
narrowed this down to just two queues: one for ready threads and one for
zombie threads. We also decided to store the running thread as a tcb struct.
To determine if it was the first call of the uthread_create function, we
checked to see if the static int variable tid_create was equal to 0. In that
case, we would call the main_thread function, which initializes the two
queues, the running tcb struct, and the main thread (which was immediately
set to the running tcb). We initially struggled to comprehend the logic of
the uthread_yield function, so we visited office hours. There, we learned
that context switching is what executes the contents of the context of a
thread. To implement this, we initialized temporary tcbs to hold the running
tcb and the first dequeued tcb of the ready queue, then changed their
positions, and context switched. For the uthread_exit function, we moved
the running tcb to the zombie queue and made the first dequeued tcb of the
ready queue the running tcb, then context switched. For uthread_join, the
implementation is still the inifinite loop from Phase 2.

### Testing:
We ran the tests in the test directory: uthread_hello and uthread_yield. We
initially got no output at all as well as an infinite loop that repeatedly
switched the positions of the running tcb and the first tcb of the ready
queue. We eventually realized that we were switching the contexts of the
wrong tcbs, so nothing ever executed and exited. When this was corrected,
we passed the uthread_hello test. When we executed the uthread_yield test,
only thread1 and thread2 were outputted. After extensive testing with
printf statements, we determined that our uthread_delete function was not
behaving correctly. We corrected this by including a context switch within
this function. Upon running uthread_yield again, the correct output was
achieved.

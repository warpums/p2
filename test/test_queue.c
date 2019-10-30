#include <stdio.h>
#include <queue.h>
#include <assert.h>

/* Callback function that increments items by a certain value */
static int inc_item(void * data, void * arg) {
    int * a = (int * ) data;
    int inc = (int)(long) arg;

    * a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void * data, void * arg) {
    int * a = (int * ) data;
    int match = (int)(long) arg;

    if ( * a == match)
        return 1;

    return 0;
}

void test_iterator(void) {
    queue_t q;
    int data[] = {1,2,3,4,5,6,7,8,9,10};
    int i;
    int * ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, & data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void * ) 1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void * ) 5, (void ** ) & ptr);
    assert(ptr != NULL);
    assert( * ptr == 5);
    assert(ptr == & data[3]);
}

//function to test whether creating a queue succeeds
void test_create(void) {
    queue_t q;
    q = queue_create();
    assert(q != NULL);
}

//function simply tests enqueuing and dequeuing from queue after created
void test_queue_simple(void) {
    queue_t q;
    int data = 3, * ptr;
    q = queue_create();
    queue_enqueue(q, & data);
    queue_dequeue(q, (void ** ) & ptr);
    assert(ptr == & data);
}

/*function to test the queue length 
items are enqueued and deleted from the queue 
at the end, we are comparing the expected length with the length from the function*/
void test_queue_length(void) {
    queue_t q;
    int final_len = 3;
    q = queue_create();
    queue_enqueue(q, (void * ) 10);
    queue_enqueue(q, (void * ) 20);
    queue_enqueue(q, (void * ) 30);
    queue_enqueue(q, (void * ) 40);
    queue_delete(q, (void * ) 30);
    queue_enqueue(q, (void * ) 50);
    queue_enqueue(q, (void * ) 60);
    queue_delete(q, (void * ) 10);
    queue_delete(q, (void * ) 40);
    int len = queue_length(q);

    assert(len == final_len);
}

/*function to test destroying the queue_t
items are enqueued into the queue and then the queue is destroyed*/
void test_queue_destroy(void) {
    queue_t q;
    q = queue_create();
    queue_enqueue(q, (void * ) 10);
    queue_enqueue(q, (void * ) 20);
    queue_enqueue(q, (void * ) 30);
    queue_enqueue(q, (void * ) 40);
    queue_destroy(q);
    int final_queue_len = queue_length(q);

    assert(final_queue_len == 0);
}

/*a complex test to test various queue functions
first creating a queue, enqueuing 6 values to the queue, 
dequeues 1 value from the queue, deletes 3 values from the queue 
and then compares the length witht the expected length of 3*/
void test_complex(void) {
    queue_t q;
    q = queue_create();

    queue_enqueue(q, (void * ) 10);
    queue_enqueue(q, (void * ) 20);
    queue_enqueue(q, (void * ) 30);
    queue_enqueue(q, (void * ) 40);
    queue_enqueue(q, (void * ) 50);
    queue_enqueue(q, (void * ) 60);


    int data = 5, * ptr;

    queue_enqueue(q, & data);
    queue_dequeue(q, (void ** ) & ptr);

    queue_delete(q, (void * ) 40);

    queue_delete(q, (void * ) 20);


    queue_delete(q, (void * ) 60);

    int final_queue_len = queue_length(q);
    assert(final_queue_len == 3);
}


int main(void) {
    test_create();
    test_queue_simple();
    test_iterator();
    test_queue_length();
    test_queue_destroy();
    test_complex();
    printf("Passed all tests!\n");
    return 0;
}

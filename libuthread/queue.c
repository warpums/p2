#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "queue.h"

struct q_node {
        void *node_data;
        struct q_node *next;
        struct q_node *previous;
};

struct queue {
        struct q_node *first, *last;
};

queue_t queue_create(void)
{
        struct queue *new_q = (struct queue*)malloc(sizeof(struct queue));
        new_q->first = NULL;
        new_q->last = NULL;

        return new_q;
}

int queue_destroy(queue_t queue)
{
        struct q_node *current_node;
        struct q_node *next_node;

        if(queue == NULL){
                return -1;
        }

        current_node = queue->first;
        next_node = current_node->next;
        while(next_node != NULL){
                free(current_node);
                current_node = next_node;
                next_node = current_node->next;
        };
        free(current_node);

        queue->first = NULL;
        queue->last = NULL;

        free(queue);

        return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
        if(queue == NULL || data == NULL){
                return -1;
        }

        struct q_node *new_node = (struct q_node*)malloc(sizeof(struct q_node));
        if(new_node == NULL){
                return -1;
        }

        new_node->node_data = data;
        new_node->next = NULL;
        new_node->previous = NULL;

        if(queue->last == NULL){
                queue->first = new_node;
                queue->last = new_node;

                return 0;
        }

        new_node->previous = queue->last;
        queue->last->next = new_node;
        queue->last = new_node;

        return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
        if(queue == NULL || queue->first == NULL || queue->first->node_data == NULL){
                return -1;
        }

        struct q_node *current = queue->first;
        *data = current->node_data;

        if(queue->first == queue->last){
                queue->first = NULL;
                queue->last = NULL;
        }else{
                queue->first = queue->first->next;
                queue->first->previous = NULL;
        }

        free(current);

        return 0;
}

int queue_get_data(queue_t queue, void **data)
{
        if(queue == NULL || queue->first == NULL || queue->first->node_data == NULL){
                return -1;
        }

        struct q_node *current = queue->first;
        *data = current->node_data;

        return 0;
}

/*int queue_delete(queue_t queue, void *data)
{

        bool data_found = false;

        if(queue == NULL){
                return -1;
        }

        struct q-node *current = queue->first;

        if(current->node_data == data){
                queue->first = current->next;
                queue->first->previous = NULL;
                free(current);

                return 0;
        }

}*/

int queue_delete(queue_t queue, void *data)
{

    struct q_node *current = queue->first;
    //struct q_node *temp_previous;
    //struct q_node *temp_next;
    int q_length = queue_length(queue);

        if(queue == NULL) {
                return -1;
        }

        for(int i = 0; i < q_length; i++){
                if(current->node_data == data) {

                    // temp_previous = current->previous;
                    // temp_next = current->next;

                    if(current == queue->first) {                                   // if first node
                        // current->previous->next = current->next;
                        current->next->previous = NULL;
                        queue->first = current->next;

                    }else if(current == queue->last) {                              // if last node
                        current->previous->next = NULL;
                        // current->next->previous = current->previous;
                        queue->last = current->previous;

                    } else {                                                    // if node in middle

                         current->previous->next = current->next;
                         current->next->previous = current->previous;
                    }

                    free(current);
                    return 0;
                }
                current = current->next;
        }

        return -1;

}


int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
        if(queue == NULL || func == NULL){
                return -1;
        }

        int retval = 0;

        struct q_node *current = queue->first;

        int len = queue_length(queue);

        for(int i = 0; i < len; i++){
                retval = func(current->node_data, arg);
                if(retval == 1){
                        if(data != NULL){
                                *data = current->node_data;
                        }
                        return 0;
                }
                current = current->next;
        }

        return 0;

}

int queue_length(queue_t queue)
{
        int q_size = 0;
        struct q_node *current;
        if(queue == NULL){
                return -1;
        }

        if(queue->last == NULL){
                return 0;
        }

        current = queue->first;
        q_size++;

        for(; current->next != NULL; q_size++){
                current = current->next;
        }

        return q_size;

}

static int inc_item(void *data, void *arg)
{
        int *a = (int*)data;
        int inc = (int)(long)arg;

        *a += inc;

        return 0;
}

static int find_item(void *data, void *arg)
{
        int *a = (int*)data;
        int match = (int)(long)arg;

        if (*a == match)
                return 1;

        return 0;
}

void test_iterator(void)
{
        queue_t q;
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int i;
        int *ptr;

        /* Initialize the queue and enqueue items */
        q = queue_create();
        for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
                queue_enqueue(q, &data[i]);

        /* Add value '1' to every item of the queue */
        queue_iterate(q, inc_item, (void*)1, NULL);
        assert(data[0] == 2);

        /* Find and get the item which is equal to value '5' */
        ptr = NULL;
        queue_iterate(q, find_item, (void*)5, (void**)&ptr);
        assert(ptr != NULL);
        assert(*ptr == 5);
        assert(ptr == &data[3]);
}

void test_create(void)
{
        queue_t q;

        q = queue_create();
        assert(q != NULL);
}

void test_queue_simple(void)
{
        queue_t q;
        int data = 3, *ptr;

        q = queue_create();
        queue_enqueue(q, &data);
        queue_dequeue(q, (void**)&ptr);
        assert(ptr == &data);
}

//int main(){

        /*struct queue* q = queue_create();
        queue_enqueue(q, 10);
        queue_enqueue(q, 20);
        queue_enqueue(q, 30);
        queue_enqueue(q, 40);
        queue_enqueue(q, 50);
        queue_enqueue(q, 60);


        int len = queue_length(q);
        printf("q1 = %d\n", q->first->node_data);
        printf("q length = %d\n", len);
        int i = 5;
        queue_dequeue(q, &i);
        printf("dq = %d\n", i);

        queue_delete(q, 30);

        len = queue_length(q);

        struct q_node* n = q->first;
        for(int k = 0; k < len; k++) {
                printf("index %d, val %d\n", k, n->node_data);
                n = n->next;
        }

        queue_delete(q, 20);

        len = queue_length(q);

        n = q->first;
        for(int k = 0; k < len; k++) {
                printf("index %d, val %d\n", k, n->node_data);
                n = n->next;
        }


        queue_delete(q, 60);

        len = queue_length(q);

        n = q->first;
        for(int k = 0; k < len; k++) {
                printf("index %d, val %d\n", k, n->node_data);
                n = n->next;
        }


        queue_destroy(q);

        len = queue_length(q);

        printf("length of destroyed string: %d\n", len);

        n = q->first;
        for(int k = 0; k < len; k++) {
                printf("index %d, val %d\n", k, n->node_data);
                n = n->next;
        }

        queue_iterate(q, inc_item);

        printf("inc_item retval = %d\n", retval);

        n = q->first;
        for(int k = 0; k < len; k++) {
                printf("index %d, val %d\n", k, n->node_data);
                n = n->next;
        }*/

        /*test_create();

        test_queue_simple();

        test_iterator();

        return 0;
}*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "queue.h"

/*doubly linked list implementation of queue
q_node stores a pointer to the next node as well as previous*/
struct q_node {
    void * node_data;
    struct q_node * next;
    struct q_node * previous;
};

//pointer to first and last nodes in queue
struct queue {
    struct q_node * first, * last;
};

//allocate memory and initializes first and last as NULL
queue_t queue_create(void) {
    struct queue * new_q = (struct queue * ) malloc(sizeof(struct queue));
    new_q->first = NULL;
    new_q->last = NULL;

    return new_q;
}

/*deallocates memory from every node
then deallocates the queue itself
if the queue is destroyed successfully, return 0
if no queue to destroy, return -1*/
int queue_destroy(queue_t queue) {
    struct q_node * current_node;
    struct q_node * next_node;

    if (queue == NULL) {
        return -1;
    }

    current_node = queue->first;
    next_node = current_node->next;
    while (next_node != NULL) {
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

/*creates a new node with a specified data item
if the queue is empty, it gets set to both first and last
else it is enqueued to the last position
if successfully enqueued, return 0, else return -1*/
int queue_enqueue(queue_t queue, void * data) {
    if (queue == NULL || data == NULL) {
        return -1;
    }

    struct q_node * new_node = (struct q_node * ) malloc(sizeof(struct q_node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->node_data = data;
    new_node->next = NULL;
    new_node->previous = NULL;

    if (queue->last == NULL) {
        queue->first = new_node;
        queue->last = new_node;

        return 0;
    }

    new_node->previous = queue->last;
    queue->last->next = new_node;
    queue->last = new_node;

    return 0;
}

/*Dequeues the oldest node from the queue
if there is only one node, it is removed and both first and last set to NULL
Returns the data item of the dequeued node by reference and function returns 0 if successfull*/
int queue_dequeue(queue_t queue, void ** data) {
    if (queue == NULL || queue->first == NULL || queue->first->node_data == NULL) {
        return -1;
    }

    struct q_node * current = queue->first;
    * data = current->node_data;

    if (queue->first == queue->last) {
        queue->first = NULL;
        queue->last = NULL;
    } else {
        queue->first = queue->first->next;
        queue->first->previous = NULL;
    }

    free(current);

    return 0;
}

/*searches for a node with the given data item
Split into 3 cases of how to delete: first node, last node, or any node in middle
if data item was found at first node, the next node becomes the first node of the queue
if data found at last node, sets the previous node to last node
if data in middle, updates previous and next nodes*/
int queue_delete(queue_t queue, void * data) {

    struct q_node * current = queue->first;

    int q_length = queue_length(queue);

    if (queue == NULL) {
        return -1;
    }

    for (int i = 0; i < q_length; i++) {
        if (current->node_data == data) {



            if (current == queue->first) {
                current->next->previous = NULL;
                queue->first = current->next;

            } else if (current == queue->last) {
                current->previous->next = NULL;
                queue->last = current->previous;

            } else {
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

/*iterates through the queue applying specified callback function to each node
if the callback function returns 1, function returns that data item*/
int queue_iterate(queue_t queue, queue_func_t func, void * arg, void ** data) {
    if (queue == NULL || func == NULL) {
        return -1;
    }

    int retval = 0;

    struct q_node * current = queue->first;

    int len = queue_length(queue);

    for (int i = 0; i < len; i++) {
        retval = func(current->node_data, arg);
        if (retval == 1) {
            if (data != NULL) {
                * data = current->node_data;
            }
            return 0;
        }
        current = current->next;
    }

    return 0;

}

/*returns the length of the queue_length
if there is no queue, returns -1
if the queue is empty, it returns 0*/
int queue_length(queue_t queue) {
    int q_size = 0;
    struct q_node * current;
    if (queue == NULL) {
        return -1;
    }

    if (queue->last == NULL) {
        return 0;
    }

    current = queue->first;
    q_size++;

    for (; current->next != NULL; q_size++) {
        current = current->next;
    }

    return q_size;

}

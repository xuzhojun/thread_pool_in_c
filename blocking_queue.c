#include "blocking_queue.h"
#include <stdlib.h>

struct blocking_queue *new_blocking_queue(int capacity) {
    struct blocking_queue *queue = malloc(sizeof(struct blocking_queue));

    queue->capacity = capacity;
    queue->size = 0;

    queue->head = queue->tail = NULL;

    return queue;
}

int bq_push(struct blocking_queue *queue, void *data) {
    if (queue->size < queue->capacity) {
        struct queue_node *node = calloc(1, sizeof(struct queue_node));
        if (node == NULL) {
            return -2;
        }
        node->data = data;

        node->pre = queue->tail;
        node->next = NULL;

        if (queue->head == NULL && queue->tail == NULL) {
            queue->head = node;
            queue->tail = node;
        } else {
            queue->tail = node;    
        }
        queue->size++;

        return 0;
    }

    return -1;
}

void *bq_take(struct blocking_queue *queue) {
    return queue->head;
}

void *bq_offer(struct blocking_queue *queue) {
    return queue->tail;
}

int bq_size(struct blocking_queue *queue) {
    return queue->size;
}

void free_blocking_queue(struct blocking_queue *queue) {
    free(queue);
}
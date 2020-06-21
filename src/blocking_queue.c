#include "blocking_queue.h"
#include <stdlib.h>
#include <stdio.h>

struct blocking_queue *new_blocking_queue(int capacity) {
    struct blocking_queue *queue = malloc(sizeof(struct blocking_queue));

    queue->capacity = capacity;
    queue->size = 0;

    queue->head = queue->tail = NULL;

    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->cond), NULL);

    return queue;
}

int bq_push(struct blocking_queue *queue, void *data) {
    printf("bq_push()\n");
    pthread_mutex_lock(&(queue->mutex));
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

        pthread_cond_signal(&(queue->cond));

        pthread_mutex_unlock(&(queue->mutex));

        return 0;
    }

    pthread_mutex_unlock(&(queue->mutex));

    return -1;
}

void *bq_take(struct blocking_queue *queue) {
    printf("bq_task()\n");
    pthread_mutex_lock(&(queue->mutex));
    
    // 循环判断，TODO：说明为什么需要循环判断
    while (queue->size == 0) {
        pthread_cond_wait(&(queue->cond), &(queue->mutex));
    }

    queue->size--;
    struct queue_node *node = queue->head;
    if (queue->tail == queue->head) {
        queue->tail = NULL;
        queue->head = NULL;
    } else {
        queue->head = queue->head->next;
        queue->head->pre = NULL;
    }
    
    node->next = NULL;
    void *data = node->data;
    free(node);
    pthread_mutex_unlock(&(queue->mutex));

    return data;
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
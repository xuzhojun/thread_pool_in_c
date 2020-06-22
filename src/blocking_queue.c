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
        queue->tail = node;
        node->next = NULL;

        if (node->pre == NULL) {
            queue->head = node;
        } else {
            node->pre->next = node;    
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
    // 防止唤醒了多个线程，队列的元素已经被取走
    while (queue->size == 0) {
        pthread_cond_wait(&(queue->cond), &(queue->mutex));
    }

    queue->size--;
    struct queue_node *node = queue->head;
    queue->head = node->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    } else {
        queue->head->pre = NULL;
    }
    pthread_mutex_unlock(&(queue->mutex));

    node->next = NULL;
    void *data = node->data;
    free(node);

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
#include "blocking_queue.h"
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#define is_empty(queue) (queue->size == 0)
#define not_full(queue) (queue->size < queue->capacity)

#define signal_not_empty(queue)
#define signal_not_full(queue) 
#define lock_queue(queue)
#define unlock_queue(queue)

/**
 * 队列的数据节点
 */
typedef struct queue_node {
    struct queue_node *pre;
    struct queue_node *next;
    void *data;
} queue_node_t;

/**
 * 队列指针，队列里数据采用链表形式存储
 */ 
struct blocking_queue {
    int capacity;
    volatile int size;

    struct queue_node *head;
    struct queue_node *tail;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

blocking_queue_t *bq_new_blocking_queue(int capacity) {
    blocking_queue_t *queue = malloc(sizeof(blocking_queue_t));

    queue->capacity = capacity;
    queue->size = 0;

    queue->head = queue->tail = NULL;

    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->cond), NULL);

    return queue;
}

int bq_push(blocking_queue_t *queue, void *data) {
    printf("bq_push()\n");
    // 防止线程退出时未释放锁
    pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *) &(queue->mutex));
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

	    printf("queue size %d\n", queue->size);
        pthread_cond_signal(&(queue->cond));
        pthread_mutex_unlock(&(queue->mutex));
        pthread_cleanup_pop(0);

        return 0;
    }

    pthread_cond_signal(&(queue->cond));
    pthread_mutex_unlock(&(queue->mutex));
    pthread_cleanup_pop(0);

    return -1;
}

void *bq_take(blocking_queue_t *queue) {
    printf("bq_take()\n");
    pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *)&(queue->mutex));
    pthread_mutex_lock(&(queue->mutex));
    
    // 循环判断
    // 防止唤醒了多个线程，队列的元素已经被取走
    while (queue->size == 0) {
        printf("loop check queue->size %d\n", queue->size);
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
    pthread_cheanup_pop(0);

    node->next = NULL;
    void *data = node->data;
    printf("before free node\n");
    free(node);

    return data;
}

void *bq_offer(blocking_queue_t *queue) {
    return queue->tail;
}

int bq_size(blocking_queue_t *queue) {
    int size;
    pthread_mutex_lock(&(queue->mutex));
    size = queue->size;
    pthread_mutex_unlock(&(queue->mutex));

    return size;
}

void bq_free_blocking_queue(blocking_queue_t *queue) {
    free(queue);
}

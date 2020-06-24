#include "blocking_queue.h"
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#define is_empty(queue) (queue->size == 0)
#define is_full(queue) (queue->size == queue->capacity)


#define wait_not_full() pthread_cond_wait(&(queue->not_full_cond), &(queue->mutex))
#define wait_not_empty() pthread_cond_wait(&(queue->not_empty_cond), &(queue->mutex))
#define wait_not_full_time(timeout) \
                pthread_cond_timewait(&(queue->not_full_cond), &(queue->mutex), timeout)
#define wait_not_empty_time(timeout) \
                pthread_cond_timewait(&(queue->not_empty_cond), &(queue->mutex), timeout)
#define signal_not_empty() pthread_cond_signal(&(queue->not_empty_cond))
#define signal_not_full() pthread_cond_signal(&(queue->not_full_cond))
#define lock_queue()  pthread_mutex_lock(&(queue->mutex))
#define unlock_queue() pthread_mutex_unlock(&(queue->mutex))


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
    pthread_cond_t not_full_cond;
    pthread_cond_t not_empty_cond;
};

blocking_queue_t *bq_new_blocking_queue(int capacity) {
    blocking_queue_t *queue = malloc(sizeof(blocking_queue_t));

    queue->capacity = capacity;
    queue->size = 0;

    queue->head = queue->tail = NULL;

    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->not_full_cond), NULL);
    pthread_cond_init(&(queue->not_empty_cond), NULL);

    return queue;
}

int bq_put(blocking_queue_t *queue, void *data) {
    printf("bq_put()\n");

    int ret;

    // 防止线程退出时未释放锁
    // pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *) &(queue->mutex));
    lock_queue();
    while (is_full(queue)) {
        wait_not_full();
    }

    struct queue_node *node = calloc(1, sizeof(struct queue_node));
    if (node == NULL) {
        ret = -2;
        goto END;
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
    ret = 0;

	printf("queue size %d\n", queue->size);
    signal_not_empty();

END:
    unlock_queue();
    // pthread_cleanup_pop(0);

    return ret;
}

int bq_offer(blocking_queue_t *queue, void *data) {
    printf("bq_put()\n");

    int ret;
    // 防止线程退出时未释放锁，和pop成对在同一代码层级出现
    // pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *) &(queue->mutex));
    lock_queue();

    if (is_full(queue)) {
        ret = -1;
        goto END;
    }

    struct queue_node *node = calloc(1, sizeof(struct queue_node));
    if (node == NULL) {
        ret = -2;
        goto END;
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
    ret = 0;

	printf("queue size %d\n", queue->size);
    signal_not_empty();

END:
    unlock_queue();
    // pthread_cleanup_pop(0);

    return ret;
}

int bq_offer_time(blocking_queue_t *queue, void *data, int timeout) {
    printf("bq_put()\n");
    int ret;
    // 防止线程退出时未释放锁
    // pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *) &(queue->mutex));
    lock_queue();
    if (is_full(queue)) {
        // TODO: 计算超时时间
        struct timeval timeout; 
        wait_not_full_time(timeout);
    }

    if (is_full(queue)) {
        ret = -1;
        goto END;
    }

    struct queue_node *node = calloc(1, sizeof(struct queue_node));
    if (node == NULL) {
        ret = -2;
        goto END;
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
    ret = 0;

	printf("queue size %d\n", queue->size);
    signal_not_empty();

END:
    unlock_queue();
    // pthread_cleanup_pop(0);
    return ret;
}

void *bq_take(blocking_queue_t *queue) {
    printf("bq_take()\n");
    // pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock, (void *)&(queue->mutex));
    lock_queue();
    
    // 循环判断
    // 防止唤醒了多个线程，队列的元素已经被取走
    while (queue->size == 0) {
        printf("loop check queue->size %d\n", queue->size);
        wait_not_empty();
    }

    queue->size--;
    struct queue_node *node = queue->head;
    queue->head = node->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    } else {
        queue->head->pre = NULL;
    }

    unlock_queue();
    // pthread_cheanup_pop(0);

    node->next = NULL;
    void *data = node->data;
    printf("before free node\n");
    free(node);

    return data;
}

int bq_size(blocking_queue_t *queue) {
    int size;
    lock_queue();
    size = queue->size;
    unlock_queue();

    return size;
}

void bq_free_blocking_queue(blocking_queue_t *queue) {
    free(queue);
}

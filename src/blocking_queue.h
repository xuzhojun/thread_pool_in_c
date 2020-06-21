#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <pthread.h>

/**
 * 
 * 
 * 
 * @auth xuzhojun  
 */

/**
 * 队列的数据节点
 */
struct queue_node {
    struct queue_node *pre;
    struct queue_node *next;
    void *data;
};

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

/**
 * 创建指定容量大小的队列，返回的队列，用于之后对队列操作的参数
 * 
 * @param capacity 队列的大小，指定之后不能改变
 * 
 * @return  NULL - 创建队列失败
 *          非NULL - 指向队列的指针
 */ 
struct blocking_queue *new_blocking_queue(int capacity);

/**
 * 如果消息队列大小小于capacity，则在队列尾部插入数据，否则返回错误
 * 
 * @param queue 阻塞的消息队列
 * @param data 需要入队的数据
 * 
 * @return 0 - 入队成功
 *         -1 - 队列满
 *         -2 - 申请节点内存空间失败
 */
int bq_push(struct blocking_queue *queue, void *data);

/**
 * 从消息队列头部取数据，取出成功则把队列里的节点删除
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 获取数据失败
 *         非NULL - 返回void *类型的数据
 */
void *bq_take(struct blocking_queue *queue);

/**
 * 从消息队列头部取数据，不删除节点
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 没有数据
 *         非NULL - 返回void *类型的数据
 */
void *bq_offer(struct blocking_queue *queue);

/**
 * 返回当前队列大小
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 获取数据失败
 *         非NULL - 返回void *类型的数据
 */
int bq_size(struct blocking_queue *queue);

/**
 * 释放队列，必须是调用new_blocking_queue创建的队列.
 * free之前应该确保队列里的数据已经全部取出，否则队列里外部数据动态申请的内存空间将不会释放
 * 
 * @param queue 阻塞的消息队列
 */
void free_blocking_queue(struct blocking_queue *queue);


#endif
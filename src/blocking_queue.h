#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <pthread.h>

/**
 * 一个用链表实现的阻塞队列，
 * 用互斥锁实现多线程操作的互斥
 * 
 * @auth xuzhojun  
 */


typedef struct blocking_queue blocking_queue_t;

/**
 * 创建指定容量大小的队列，返回的队列，用于之后对队列操作的参数
 * 
 * @param capacity 队列的大小，指定之后不能改变
 * 
 * @return  NULL - 创建队列失败
 *          非NULL - 指向队列的指针
 */ 
blocking_queue_t *bq_new_blocking_queue(int capacity);

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
int bq_push(blocking_queue_t *queue, void *data);

/**
 * 从消息队列头部取数据，取出成功则把队列里的节点删除
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 获取数据失败
 *         非NULL - 返回void *类型的数据
 */
void *bq_take(blocking_queue_t *queue);

/**
 * 从消息队列头部取数据，不删除节点
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 没有数据
 *         非NULL - 返回void *类型的数据
 */
void *bq_offer(blocking_queue_t *queue);

/**
 * 返回当前队列大小
 * 
 * @param queue 阻塞的消息队列
 * 
 * @return NULL - 获取数据失败
 *         非NULL - 返回void *类型的数据
 */
int bq_size(blocking_queue_t *queue);

/**
 * 释放队列，必须是调用new_blocking_queue创建的队列.
 * free之前应该确保队列里的数据已经全部取出，否则队列里外部数据动态申请的内存空间将不会释放
 * 
 * @param queue 阻塞的消息队列
 */
void bq_free_blocking_queue(blocking_queue_t *queue);


#endif

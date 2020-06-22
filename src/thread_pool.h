#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "blocking_queue.h"

struct thread_pool {
	int core_size;
	int max_size;
	int current_size;

	struct blocking_queue *queue;
	void **core_workers;
	void **other_workers;

	pthread_mutex_t mutex;

	void (*abort_policy)(void *arg);
};

/**
 * 创建一个线程池的结构体，供后续的执行函数使用
 * 
 * @param core_size 一直保持的线程数量
 * @param max_size  任务队列满时，可以一直保持的线程数量，包含core_size
 * @param queue  一个阻塞队列
 * @param abort_policy 任务队列满且线程数已经达到max_size时，对新任务的处理方式，
 *                     thread_pool会调用这个函数，并且把任务的参数传给此函数
 * 
 * @return NULL - 创建线程池失败
 *         非NULL - 创建线程池成功
 */
struct thread_pool *tp_new_thread_pool(int core_size, 
                                    int max_size, 
									struct blocking_queue *queue,
									void (*abort_policy)(void *arg));

void tp_execute(struct thread_pool *pool, void (*run)(void *), void *arg);
void tp_shut_down(struct thread_pool *pool);

void tp_free_thread_pool(struct thread_pool *pool);



#endif

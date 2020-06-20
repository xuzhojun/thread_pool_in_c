#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "blocking_queue.h"

struct worker {

};

struct task {
	void (*run)(void *arg);
	void *arg;
};

struct thread_pool {
	int core_size;
	int max_size;
	int current_size;

	struct blocking_queue *queue;
	void *core_workers;
	void *other_workers;
};


struct thread_pool *new_thread_pool(int core_size, int max_size, struct blocking_queue *queue);

void execute(struct thread_pool *pool, struct task *task);
void shutdown(struct thread_pool *pool);

void free_thread_pool(struct thread_pool *pool);



#endif

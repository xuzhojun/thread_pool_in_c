#include "thread_pool.h"
#include <stdlib.h>
#include <pthread.h>

struct thread_pool *new_thread_pool(int core_size,
                        int max_size,
						struct blocking_queue *queue) {
	struct thread_pool *pool = malloc(sizeof(struct thread_pool));
	if (pool == NULL) {
		return NULL;
	}

	if (max_size < core_size) {
		max_size = core_size;
	}
	
	pool->core_size = core_size;
	pool->max_size = max_size;
	pool->current_size = 0;

	pool->queue = queue;
	pool->core_workers = calloc(core_size, sizeof(void *));
	pool->other_workers = calloc(max_size - core_size, sizoef(void *));

	return pool;
}

static void __run_work(void *arg) {
	struct thread_pool *pool = (struct thread_pool *)arg;
	void *task;
	while ((task = bp_take(pool->queue)) != NULL) {
		struct task *t = task;

		t->run(t->arg);
	}
}

static int __add_worker(struct thread_pool *pool, struct task *task) {
	if (pool->current_size < pool->core_size) {
		pthread_t *tid = calloc(1, sizeof(pthread_t));
		int r = pthread_create(tid, NULL, __run_work, (void *)pool);
		if (r == 0) {
			pool->core_workers[pool->current_size] = tid;
		} else {
			free(tid);
			return -1;
		}
		pool->current_size++;
	}

    // 队列满
	if (-1 == bq_push(pool->queue, task)) {
		// TODO: 创建超过core_size数量的新线程

	}
}


void execute(struct thread_pool *pool, struct task *task) {
		add_worker(pool, task);
}

void shutdown(struct thread_pool *pool) {

}

void free_thread_pool(struct thread_pool *pool) {
	free(pool);
}

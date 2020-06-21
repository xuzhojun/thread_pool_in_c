#include "thread_pool.h"
#include "blocking_queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct worker {
	pthread_t tid;
	struct thread_pool *pool;
	struct task *first_task;

	int completed_tasks;
};

struct task *new_task(void (*run)(void *arg), void *arg) {
	struct task *t = malloc(sizeof(struct task));
	if (t == NULL) {
		return NULL;
	}

	t->run = run;
	t->arg = arg;

	return t;
}

void free_task(struct task *task) {
	free(task);
}

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
	pool->other_workers = calloc(max_size - core_size, sizeof(void *));

	pthread_mutex_init(&(pool->mutex), NULL);

	return pool;
}

static void *__run(void *arg) {
	printf("__run()\n");
	struct worker *worker = (struct worker *)arg;
	struct thread_pool *pool = worker->pool;
	struct task *task = worker->first_task;

	while (task != NULL || (task = bq_take(pool->queue)) != NULL) {
		printf("%ld 执行任务\n", pthread_self());
		task->run(task->arg);
		worker->completed_tasks++;
		task = NULL;
	}
	printf("worker 结束\n");
	return NULL;
}

static int __run_work(struct worker *worker) {
	printf("__run_worker()\n");
	return pthread_create(&(worker->tid), NULL, __run, worker);
}

static int __add_worker(struct thread_pool *pool, struct task *task) {
	printf("__add_worker()\n");
	pthread_mutex_lock(&(pool->mutex));
	printf("current_size: %d\n", pool->current_size);
	if (pool->current_size < pool->core_size) {
		struct worker *worker = malloc(sizeof(struct worker));

		worker->pool = pool;
		worker->first_task = task;
		worker->completed_tasks = 0;

		int r = __run_work(worker);
		if (r == 0) {
			pool->core_workers[pool->current_size] = worker;
		} else {
			free(worker);
			pthread_mutex_unlock(&(pool->mutex));
			return -1;
		}
		pool->current_size++;
	} else if (-1 == bq_push(pool->queue, task)) { // 队列满
		// TODO: 创建超过core_size数量的新线程
		printf("队列满\n");
	}

	pthread_mutex_unlock(&(pool->mutex));
	return 0;
}


void execute(struct thread_pool *pool, struct task *task) {
	printf("execute()\n");
	__add_worker(pool, task);
}

void shut_down(struct thread_pool *pool) {
	int i = 0;
	for (; i < pool->core_size; i++) {
		if (pool->core_workers[i] != NULL) {
			struct worker *worker = pool->core_workers[i];
			printf("work%ld\n", worker->tid);
			pthread_cancel(worker->tid);
		} 
	}

	for (i = 0; i < pool->max_size - pool->core_size; i++) {
		if (pool->other_workers[i] != NULL) {
			struct worker *worker = pool->other_workers[i];
			printf("%ld\n", worker->tid);
			pthread_cancel(worker->tid);
		}
	}
}

void free_thread_pool(struct thread_pool *pool) {
	free(pool);
}

#include "thread_pool.h"
#include "blocking_queue.h"
#include <stdio.h>

typedef struct blocking_queue bq_t;
typedef struct thread_pool tp_t;
typedef struct task task_t;

static void __run(void *arg) {
    printf("%d\n", *(int *)arg);
}

int main() {
    bq_t *queue = new_blocking_queue(2);
    tp_t *pool = new_thread_pool(3, 5, queue);

    int i;
    for (i = 0; i < 10; i++) {
        int g = i;
        task_t *task = new_task(__run, &g);
        execute(pool, task);
    }
    
}
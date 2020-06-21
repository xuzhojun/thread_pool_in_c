#include "thread_pool.h"
#include "blocking_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct blocking_queue bq_t;
typedef struct thread_pool tp_t;
typedef struct task task_t;

static void __run(void *arg) {
    printf("run output : %d\n", *(int *)arg);
}

int main() {
    bq_t *queue = new_blocking_queue(10);
    tp_t *pool = new_thread_pool(3, 5, queue);

    int i;

    for (i = 0; i < 10; i++) {
        int *g = malloc(sizeof(int));
        *g = i;
        task_t *task = new_task(__run, g);
        execute(pool, task);
    }
    printf("任务添加完毕\n");
    sleep(10);
    shut_down(pool);

    return 0;
}
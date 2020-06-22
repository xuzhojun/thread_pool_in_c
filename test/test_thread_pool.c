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
    free(arg);
}

int main() {
    bq_t *queue = new_blocking_queue(50);
    tp_t *pool = tp_new_thread_pool(3, 3, queue, NULL);

    int i;

    for (i = 0; i < 50; i++) {
        int *g = malloc(sizeof(int));
        *g = i;
        tp_execute(pool, __run, g);
    }
    printf("任务添加完毕\n");
    sleep(10);
    tp_shut_down(pool);
    tp_free_thread_pool(pool);
    free_blocking_queue(queue);

    return 0;
}
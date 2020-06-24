#include "thread_pool.h"
#include "blocking_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void __run(void *arg) {
    printf("run output : %d\n", *(int *)arg);

    free(arg);
}

int main() {
    blocking_queue_t *queue = bq_new_blocking_queue(1);
    thread_pool_t *pool = tp_new_thread_pool(1, 1, queue, NULL);

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
    bq_free_blocking_queue(queue);

    return 0;
}

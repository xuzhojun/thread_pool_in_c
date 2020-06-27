#include "blocking_queue.h"

#include <stdlib.h>
#include <stdio.h>

int main() {

    blocking_queue_t *bq = bq_new_blocking_queue(16);

    int i;
    int ret;
    int *ip = NULL;
    for (i = 0; i < 17; i++) {
        ip = malloc(sizeof(int));
        *ip = i + 64;
        ret = bq_offer(bq, ip);
        switch (ret) {
            case 0: printf("插入数据[%d][%d]成功。\n", i, *ip); break;
            case -1: printf("队列满，插入数据[%d][%d]失败。\n", i, *ip); break;
            case -2: printf("申请节点空间失败，插入数据[%d][%d]失败。\n", i, *ip); break;
            default: printf("未知返回值。\n");
        }
    }

    int *data = NULL;
    while ((data = bq_poll(bq)) != NULL) {
        printf("取出数据[%d].\n", *data);
        printf("queue size %d\n", bq_size(bq));
        free(data);
        data = NULL;
    }

    bq_free_blocking_queue(bq);

    return 0;
}
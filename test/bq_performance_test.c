#include "blocking_queue.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct thread_arg {
    blocking_queue_t *mbq;
    int **arg;
    int start;
    int step;
} thread_arg_t;

#define TEST_DATA_NUM 50000
#define THREAD_NUM 5

blocking_queue_t *mbq;

static void print_time(struct timeval *t) {
    printf("thread id: %ld - second: %ld usecond %d\n", pthread_self(), t->tv_sec, t->tv_usec);
}

static int **fill_array() {
    int **int_array = malloc(TEST_DATA_NUM * sizeof(int *));
    int i;
    for (i = 0; i < TEST_DATA_NUM; i++) {
        int_array[i] = malloc(sizeof(int));
        *(int_array[i]) = i;
    }

    return int_array;
}

static void free_array(int **arr) {
    int i;
    for (i = 0; i < TEST_DATA_NUM; i++) {
        free(arr[i]);
    }
    free(arr);
}

void *run_get(void *arg) {
    thread_arg_t *ta = (thread_arg_t *) arg;
    int i;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
    for (i = 0; i < ta->step; i++) {
        bq_take(ta->mbq);
    }
    gettimeofday(&end, NULL);

    print_time(&start);
    print_time(&end);

    return NULL;
};

void *run_set(void *arg) {
    thread_arg_t *ta = (thread_arg_t *) arg;
    int i;
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    int j = ta->start;
    for (i = 0; i < ta->step; i++) {
        bq_put(ta->mbq, ta->arg[j++]);
    }
    gettimeofday(&end, NULL);

    print_time(&start);
    print_time(&end);

    return NULL;
}


static int multi_thread_test() {
    mbq = bq_new_blocking_queue(TEST_DATA_NUM);
    int **int_array = fill_array();

    pthread_t tid[THREAD_NUM * 2];
    thread_arg_t arg[THREAD_NUM * 2];
    int step = TEST_DATA_NUM / THREAD_NUM;

    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        arg[i].mbq = mbq;
        arg[i].start = i * step;
        arg[i].step = step;
        arg[i].arg = int_array;
    }

    for (i = 0; i < THREAD_NUM; i++) {
        pthread_create(&tid[i], NULL, run_set, &(arg[i]));
    }

    int j = 0;
    for (; i < 2 * THREAD_NUM; i++) {
        pthread_create(&tid[i], NULL, run_get, &arg[j++]);
    }

    for (i = 0; i < 2 * THREAD_NUM; i++) {
        pthread_join(tid[i], NULL);
    }

    free_array(int_array);

    bq_free_blocking_queue(mbq);
    return 0;
}

static int single_thread_test() {
    int **int_array = fill_array();

    struct timeval start;
    struct timeval middle;
    struct timeval end;

    blocking_queue_t *bq = bq_new_blocking_queue(TEST_DATA_NUM);
    if (bq == NULL) {
        printf("new blocing queue faild.\n");
        return -1;
    }

    void *data;
    int i;
    gettimeofday(&start, NULL);
    for (i = 0; i < TEST_DATA_NUM; i++) {
        bq_offer(bq, int_array[i]);
    }

    gettimeofday(&middle, NULL);
    for (i = 0; i < TEST_DATA_NUM; i++) {
        data = bq_poll(bq);
    }

    gettimeofday(&end, NULL);

    print_time(&start);
    print_time(&middle);
    print_time(&end);

    for (i = 0; i < TEST_DATA_NUM; i++) {
        free(int_array[i]);
    }

    free(int_array);
    bq_free_blocking_queue(bq);
    return 0;
}


int main() {

    // single_thread_test();

    multi_thread_test();
    return 0;
}
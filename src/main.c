#define NUM_THREADS 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>
#include "queue.h"

struct threadArgs
{
    struct queue *q;
    char *c;
};

void *putArg(void *params)
{
    struct threadArgs *args = params;
    pushQueue(args->q, args->c);
}

_Atomic long counter = ATOMIC_VAR_INIT(0L);

int main()
{
    pthread_t tids[NUM_THREADS]; //线程id
    uint32_t index = 50;
    struct queue *g_q;
    g_q = initQueue();
    char c[LENTH] = "test\0";
    char b[LENTH] = "btest\0";
    char a[LENTH] = "atest\0";
    char *h = NULL;
    int i;
    clock_t start, end;
    struct timespec start_, end_;

    clock_gettime(CLOCK_REALTIME, &start_);
    for (i = 0; i < NUM_THREADS; ++i)
    {
        struct threadArgs *args = NULL;
        args = (struct threadArgs *)malloc(sizeof(struct threadArgs));
        args->q = g_q;
        args->c = c;
        pthread_create(&tids[i], NULL, putArg, args);
    }

    while (index--)
    {
        h = popQueue(g_q);
        if (h)
        {
            //printf("%s\n", h);
            sleep(1);
        }
        else if (IsQueueEmpty(g_q))
        {
            printf("queue is empty , sleep for a while\n");
            sleep(3);
        }
    }
    clock_gettime(CLOCK_REALTIME, &end_);
    printf("run time is %lf s\n", (double)(end_.tv_nsec - start_.tv_nsec) / 1000000);
    return 0;
}
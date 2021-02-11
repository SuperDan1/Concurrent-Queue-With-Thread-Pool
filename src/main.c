#define NUM_THREADS 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>
#include "queue.h"

_Atomic long counter = ATOMIC_VAR_INIT(0L);

typedef struct threadArgs
{
    queue *q;
    int val;
} threadArgs;

void *putArg(void *params)
{
    threadArgs *args = params;
    pushQueue(args->q, args->val);
    
}

void *popArg(void *params)
{
    queue *args = params;
    int v;

    while (1)
    {
        v = popQueue(args);
        if (v)
        {
            atomic_fetch_add(&counter, v);
        }
        else
        {
            printf("%d\n", v);
            sleep(1);
            if (IsQueueEmpty(args))
            {
                break;
            }
        }
    }
}

int main()
{
    pthread_t tids[NUM_THREADS * 2]; //线程id
    queue *g_q;
    int v;
    int i;
    int res;
    struct timespec start_, end_;

    clock_gettime(CLOCK_REALTIME, &start_);
    g_q = initQueue();
    if (g_q == NULL)
    {
        printf("init queue failed!\n");
        return 0;
    }
    for (i = 0; i < NUM_THREADS; ++i)
    {
        struct threadArgs *args = NULL;
        args = (struct threadArgs *)malloc(sizeof(struct threadArgs));
        args->q = g_q;
        args->val = 1;
        pthread_create(&tids[i], NULL, putArg, args);
    }

    for (i = 0; i < 10; ++i)
    {
        queue *popargs = NULL;
        popargs = (queue *)malloc(sizeof(queue));
        popargs = g_q;
        pthread_create(&tids[i + NUM_THREADS], NULL, popArg, popargs);
    }
    sleep(1);
    res = atomic_load(&counter);
    printf("sum is %d\n", res);
    clock_gettime(CLOCK_REALTIME, &end_);
    printf("run time is %lf s\n", (double)(end_.tv_nsec - start_.tv_nsec) / 1000000);
    return 0;
}
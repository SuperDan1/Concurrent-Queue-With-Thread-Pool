#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>
#include "queue.h"
#include "thread_pool_manage.h"

#define NUM_THREADS 200

_Atomic long counter = ATOMIC_VAR_INIT(0L);

typedef struct threadArgs
{
    listQueue *queue;
    int val;
    int32_t retVal;
} threadArgs;

void *putArg(void *params)
{
    threadArgs *args = params;
    args->retVal = pushQueue(args->queue, &args->val);
}

void *popArg(void *params)
{
    listQueue *args = params;
    int v;
    while (1)
    {
        popQueue(args, (void *)&v);
        if (v)
        {
            atomic_fetch_add(&counter, v);
        }
        else
        {
            //printf("%d\n", v);
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
    uint32_t retVal;
    pthread_t tids[NUM_THREADS * 2]; //线程id
    listQueue g_q;
    int v;
    int i;
    int res;
    struct timespec start_, end_;

    clock_gettime(CLOCK_REALTIME, &start_);
    retVal = initQueue(&g_q, sizeof(int));
    if (unlikely(retVal != RETURN_OK))
    {
        printf("init listQueue failed!\n");
        return -1;
    }
    for (i = 0; i < NUM_THREADS; ++i)
    {
        struct threadArgs *args = NULL;
        args = (struct threadArgs *)malloc(sizeof(struct threadArgs));
        args->queue = &g_q;
        args->val = 1;
        pthread_create(&tids[i], NULL, putArg, args);
    }
    sleep(5);
    //ShowQueueValue(&g_q);

    for (i = 0; i < 10; ++i)
    {
        listQueue *popargs = NULL;
        popargs = &g_q;
        pthread_create(&tids[i + NUM_THREADS], NULL, popArg, popargs);
    }
    sleep(1);
    res = atomic_load(&counter);
    printf("sum is %d\n", res);
    clock_gettime(CLOCK_REALTIME, &end_);
    printf("run time is %lf s\n", (double)(end_.tv_nsec - start_.tv_nsec) / 1000000);
    return 0;
}
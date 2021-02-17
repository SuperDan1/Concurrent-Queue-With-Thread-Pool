#ifndef THREAD_POOL_MANAGE_H
#define THREAD_POOL_MANAGE_H

#include "queue.h"

typedef struct ThreadPool
{
    bool isShutDown;
    pthread_mutex_t mutex;
    pthread_cond_t queue_ready;
    listQueue *taskQueue;
    pthread_t *threadIdArray;
    uint32_t maxThreadNum;
    uint32_t minThreadNum;
    uint32_t threadNum;
} ThreadPool;

// 初始化线程池
int32_t TpInit(ThreadPool *pool, const uint32_t threadNum);

int32_t pool_add_worker(ThreadPool *pool, void *(*process)(void *arg), void *arg);
//销毁线程池
void TpDestroy(ThreadPool *pool);
#endif
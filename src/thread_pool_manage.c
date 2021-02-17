#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread_pool_manage.h"
#include "queue.h"

// 标记线程池是否处于可用状态
static int tp_alive = 1;

static ThreadPool pool;

typedef struct Threadworker
{
    void *(*process)(void *arg); /*任务回调函数*/
    void *arg;                   /*回调函数的参数*/
} Threadworker;

// tp->thread pool
int32_t TpJobQueueInit()
{
}

/*工作线程函数*/
void *thread_routine(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;
    Threadworker *worker = NULL;
    printf("starting thread 0x%lx\n", pthread_self());

    while (1)
    {
        /*线程池要销毁了*/
        if (pool->isShutDown)
        {
            printf("threadid:0x%lx exit.\n", pthread_self());
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&(pool->taskQueue->q_head_lock));
        if (IsQueueEmpty(pool->taskQueue))
        {
            //printf("thread 0x%lx is waiting\n", pthread_self());
            //sleep(1);
            //pthread_cond_wait(&(pool->queue_ready), &(pool->taskQueue->q_head_lock));
            pthread_mutex_unlock(&(pool->taskQueue->q_head_lock));
            continue;
        }
        //printf("%d\n", pool->taskQueue->q_head->next);
        popQueue(pool->taskQueue, (void **)&worker);
        //printf("%d\n", pool->taskQueue->q_head->next);
        pthread_mutex_unlock(&(pool->taskQueue->q_head_lock));
        printf("thread 0x%lx is working\n", pthread_self());
        (*(worker->process))(worker->arg);
        free(worker);
        worker = NULL;
        printf("thread 0x%lx worked end\n", pthread_self());
    }
}

int32_t TpInit(ThreadPool *pool, const uint32_t threadNum)
{
    int32_t retVal;
    pool->isShutDown = false;
    pthread_mutex_init(&(pool->mutex), NULL);
    pthread_cond_init(&(pool->queue_ready), NULL);
    pool->threadNum = threadNum;
    pool->threadIdArray = (pthread_t *)malloc(threadNum * sizeof(pthread_t));
    if (unlikely(pool->threadIdArray == NULL))
    {
        printf("malloc threadIdArray memory failed.\n");
        return RETURN_ERROR;
    }

    pool->taskQueue = (listQueue *)malloc(sizeof(listQueue));
    if (unlikely(pool->taskQueue == NULL))
    {
        free(pool->threadIdArray);
        printf("malloc taskQueue memory failed.\n");
        return RETURN_ERROR;
    }

    retVal = initQueue(pool->taskQueue, sizeof(Threadworker));
    if (unlikely(retVal != RETURN_OK))
    {
        printf("init listQueue failed!\n");
        return RETURN_ERROR;
    }

    /*创建工作线程*/
    int i = 0;
    for (i = 0; i < threadNum; i++)
    {
        pthread_create(&(pool->threadIdArray[i]), NULL, thread_routine, pool);
    }
}

int32_t pool_add_worker(ThreadPool *pool, void *(*process)(void *arg), void *arg)
{
    int32_t retVal;
    /*构造一个新任务*/
    Threadworker *newworker = (Threadworker *)malloc(sizeof(Threadworker));
    newworker->process = process;
    newworker->arg = arg;
    /*将任务加入到等待队列中*/
    retVal = pushQueue(pool->taskQueue, newworker);
    if (unlikely(retVal != RETURN_OK))
    {
        printf("add task queue task failed.");
        return retVal;
    }

    // if (pool->taskQueue->size == 0)
    //     pthread_cond_signal(&(pool->queue_ready));
    /*等待队列中有任务了，唤醒一个等待线程；
    注意如果所有线程都在忙碌，这句没有任何作用*/
    //pthread_cond_signal(&(pool->queue_ready));
    return RETURN_OK;
}

void TpDestroy(ThreadPool *pool)
{
    int i;

    if (pool->isShutDown)
    {
        return;
        //return RETURN_ERROR; /*防止两次调用*/
    }
    printf("pool destroy now\n");
    pool->isShutDown = true;
    pthread_cond_broadcast(&(pool->queue_ready));
    for (i = 0; i < pool->threadNum; ++i)
    {
        pthread_join(pool->threadIdArray[i], NULL);
    }

    pthread_cond_destroy(&(pool->queue_ready));
    free(pool->threadIdArray);
    free(pool->taskQueue);
}

// //线程函数
// void *tp_thread_func(thread_pool *tp)
// {
//     FUNC function;
//     void *arg_buf;
//     thread_pool_job *job_p;

//     while (tp_alive)
//     {
//         // 线程阻塞,等待信号量
//         if (sem_wait(tp->job_queue->quene_sem))
//         {
//             printf("thread waiting for semaphore....\n");
//             exit(1);
//         }
//         if (tp_alive)
//         {
//             pthread_mutex_lock(&mutex);
//             job_p = tp_get_lastjob(tp);
//             if (NULL == job_p)
//             {
//                 pthread_mutex_unlock(&mutex);
//                 continue;
//             }
//             function = job_p->function;
//             arg_buf = job_p->arg;
//             if (tp_delete__lastjob(tp))
//                 return;
//             pthread_mutex_unlock(&mutex);
//             //运行指定的线程函数
//             printf("consumer...get a job from job quene and run it!\n");
//             function(arg_buf);
//             free(job_p);
//         }
//         else
//             return;
//     }

//     return;
// }
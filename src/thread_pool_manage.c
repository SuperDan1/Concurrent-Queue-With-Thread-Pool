#include "thread_pool_manage.h"

// 标记线程池是否处于可用状态
static int tp_alive = 1;

// tp->thread pool
int32_t TpJobQueueInit()
{
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
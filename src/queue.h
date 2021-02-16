#ifndef QUEUE_H
#define QUEUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>

#define RETURN_OK 0
#define RETURN_ERROR -1

#define atomic_inc(x) __sync_fetch_and_add((x), 1)
#define atomic_dec(x) __sync_fetch_and_sub((x), 1)
#define atomic_add(x, y) __sync_fetch_and_add((x), (y))
#define atomic_sub(x, y) __sync_fetch_and_sub((x), (y))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef struct listNode
{
    void *content;
    struct listNode *next;
} listNode;

typedef struct listQueue
{
    listNode *q_head;
    listNode *q_tail;
    pthread_mutex_t q_head_lock;
    pthread_mutex_t q_tail_lock;
    uint32_t data_size; // after the queue is initialized, the value is not allowed to change
                        // and read the value is not protected by lock.
    atomic_size_t size;
    uint32_t max_size;
} listQueue;

int32_t initQueue(listQueue *queue, uint32_t data_size);

int32_t pushQueue(listQueue *queue, void *value);
int32_t popQueue(listQueue *queue, void **val);
bool IsQueueEmpty(listQueue *queue);
void ShowQueueValue(listQueue *queue);
uint32_t GetQueueSize(listQueue *queue);
#endif
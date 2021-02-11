#ifndef QUEUE_H
#define QUEUE_H

#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct node
{
    int content;
    struct node *next;
} node;

typedef struct queue
{
    node *q_head;
    node *q_tail;
    pthread_mutex_t q_head_lock;
    pthread_mutex_t q_tail_lock;
    uint32_t size;
    uint32_t max_size;
} queue;

node *initNode();
queue *initQueue();

void pushQueue(queue *q, int val);
int popQueue(queue *q);

bool IsQueueEmpty(queue *q);

#endif
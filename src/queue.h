#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>

#define LENTH 10240

typedef struct node
{
    void *content;
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

void pushQueue(queue *q, char content[LENTH]);
char *popQueue(queue *q);

bool IsQueueEmpty(queue *q);
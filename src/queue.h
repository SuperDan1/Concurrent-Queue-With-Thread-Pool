#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>

#define LENTH 10240

typedef struct node
{
    char *m_content;
    struct node *p_next;
} node;

typedef struct queue
{
    node *q_head;
    node *q_tail;
    pthread_mutex_t q_head_lock;
    pthread_mutex_t q_tail_lock;
    uint32_t queueNum;
} queue;

queue *initQueue();

void pushQueue(queue *q, char content[LENTH]);
char *popQueue(queue *q);
node *initNode();
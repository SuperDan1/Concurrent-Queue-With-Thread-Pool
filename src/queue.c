#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

node *initNode(void *value, uint32_t data_size)
{
	node *h = NULL;
	h = (node *)malloc(sizeof(node));
	if (h == NULL)
	{
		printf("can not malloc struct node memory;");
		exit(1);
	}
	h->content = (void *)malloc(sizeof(data_size));
	memcpy(h->content, value, data_size);
	// printf("init success  \n");
	h->next = NULL;
	return h;
}

queue *initQueue(uint32_t data_size)
{
	queue *q = NULL;
	node *n = NULL;
	int value = 0;
	q = (queue *)malloc(sizeof(queue));
	if (q == NULL)
	{
		printf("can not malloc struct node memory;");
		exit(1);
	}
	q->data_size = data_size;
	n = initNode(&value, data_size);
	q->q_head = n;
	q->q_tail = n;
	q->max_size = 200;
	q->size = 0;
	pthread_mutex_init(&q->q_head_lock, NULL);
	pthread_mutex_init(&q->q_tail_lock, NULL);
	return q;
};

void pushQueue(queue *q, void *value)
{
	node *n = NULL;
	n = initNode(value, q->data_size);
	pthread_mutex_lock(&q->q_tail_lock);
	if (q->size == q->max_size)
	{
		//printf("queue size reaches max size.\n");
		pthread_mutex_unlock(&q->q_tail_lock);
		return;
	}
	q->q_tail->next = n;
	q->q_tail = n;
	q->size++;
	//printf("num: %d\n", q->size);
	pthread_mutex_unlock(&q->q_tail_lock);

	//printf("put: %s\n", q->q_tail->content);
}

int popQueue(queue *q)
{
	int val;
	node *h = NULL;
	node *newh = NULL;
	pthread_mutex_lock(&q->q_head_lock);
	newh = q->q_head->next;
	if (newh == NULL)
	{
		if (IsQueueEmpty(q))
		{
			printf("queue is empty , sleep for a while\n");
		}
		pthread_mutex_unlock(&q->q_head_lock);
		return 0;
	}
	h = q->q_head->next->next;
	q->q_head->next = h;
	val = *(int*)newh->content;
	//printf("get: %d\n", val);
	//printf("head: %d\n", q->q_head->content);

	free(newh);
	q->size--;
	//printf("num: %d\n", q->size);

	pthread_mutex_unlock(&q->q_head_lock);
	return val;
}

bool IsQueueEmpty(queue *q)
{
	bool IsEmpty = false;
	pthread_mutex_lock(&q->q_head_lock);
	IsEmpty = q->size == 0;
	pthread_mutex_unlock(&q->q_head_lock);
	return IsEmpty;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

node *initNode(int value)
{
	node *h = NULL;
	h = (node *)malloc(sizeof(node));
	if (h == NULL)
	{
		printf("can not malloc struct node memory;");
		exit(1);
	}
	h->content = value;
	// printf("init success  \n");
	h->next = NULL;
	return h;
}

queue *initQueue()
{
	queue *q = NULL;
	node *n = NULL;
	n = initNode(0);
	q = (queue *)malloc(sizeof(queue));
	if (q == NULL)
	{
		printf("can not malloc struct node memory;");
		exit(1);
	}
	q->q_head = n;
	q->q_tail = n;
	q->size = 0;
	pthread_mutex_init(&q->q_head_lock, NULL);
	pthread_mutex_init(&q->q_tail_lock, NULL);
	return q;
};

void pushQueue(queue *q, int value)
{
	node *n = NULL;
	n = initNode(value);

	pthread_mutex_lock(&q->q_tail_lock);
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
	val = newh->content;
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
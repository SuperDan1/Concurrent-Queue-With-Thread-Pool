#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

node *initNode(char c[LENTH])
{
	node *h = NULL;
	h = (node *)malloc(sizeof(node));
	if (h == NULL)
	{
		//printf("can not malloc struct node memory;");
		exit(1);
	}
	h->content = (char *)malloc(sizeof(char) * LENTH);
	strcpy(h->content, c);
	////printf("init success  \n");
	h->next = NULL;
	return h;
}

queue *initQueue()
{
	queue *q = NULL;
	node *n = NULL;
	n = initNode("a");
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

void pushQueue(queue *q, char c[LENTH])
{
	node *n = NULL;
	n = initNode(c);

	pthread_mutex_lock(&q->q_tail_lock);
	q->q_tail->next = n;
	q->q_tail = n;
	q->size++;
	//printf("num: %d\n", q->size);
	pthread_mutex_unlock(&q->q_tail_lock);

	//printf("put: %s\n", q->q_tail->content);
}

char *popQueue(queue *q)
{
	char *c = NULL;
	node *h = NULL;
	node *newh = NULL;
	pthread_mutex_lock(&q->q_head_lock);
	newh = q->q_head->next;
	if (newh == NULL)
	{
		pthread_mutex_unlock(&q->q_head_lock);
		return NULL;
	}
	h = q->q_head->next->next;
	c = newh->content;
	q->q_head->next = h;
	//printf("get: %s\n", c);
	//printf("head: %s\n", q->q_head->content);
	free(c);
	q->size--;
	//printf("num: %d\n", q->size);

	pthread_mutex_unlock(&q->q_head_lock);
	return c;
}

bool IsQueueEmpty(queue *q)
{
	return q->size == 0;
}
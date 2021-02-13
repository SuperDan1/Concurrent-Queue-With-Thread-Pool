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
		printf("can not malloc struct node memory;");
		exit(1);
	}
	h->m_content = (char *)malloc(sizeof(char) * LENTH);
	strcpy(h->m_content, c);
	printf("init success  \n");
	h->p_next = NULL;
	return h;
}

queue *initQueue()
{
	queue *q;
	q = (queue *)malloc(sizeof(queue));
	if (q == NULL)
	{
		printf("can not malloc struct node memory;");
		exit(1);
	}
	q->q_head = NULL;
	q->q_tail = NULL;
	return q;
};

void pushQueue(queue *q, char c[LENTH])
{
	node *n = NULL;
	n = initNode(c);
	if (q->q_tail == NULL)
	{ // queue is empty
		q->q_head = n;
		q->q_tail = n;
	}
	else
	{
		q->q_tail->p_next = n;
		q->q_tail = n;
	}
	printf("put: %s\n", q->q_tail->m_content);
}

char *popQueue(queue *q)
{
	char *c;
	if (q->q_head == NULL)
	{
		c = "0";
		return c;
	}
	node *h;
	h = q->q_head;
	c = h->m_content;
	printf("get: %s\n", c);
	q->q_head = q->q_head->p_next;
	free(h); //这里不能c指针   回收以后c指针的返回值 会出问题
	return c;
}

bool LQueueEmpty(queue *q)
{
	return q->queueNum == 0;
}
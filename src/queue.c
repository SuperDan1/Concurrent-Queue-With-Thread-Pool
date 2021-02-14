#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

int32_t initNode(listNode **node, void *value, uint32_t data_size)
{
	int32_t retVal;

	*node = (listNode *)malloc(sizeof(listNode));
	if (*node == NULL)
	{
		printf("malloc memory failed.");
		return RETURN_ERROR;
	}

	(*node)->content = (void *)malloc(sizeof(data_size));
	if (unlikely((*node)->content == NULL))
	{
		printf("malloc memory failed.\n");
	}

	memcpy((*node)->content, value, data_size);
	//printf("get: %d\n", *(int *)node->content);
	(*node)->next = NULL;
	return RETURN_OK;
}

int32_t initQueue(listQueue *queue, uint32_t data_size)
{
	int32_t retVal;
	listNode *headNode = NULL;

	char *value = NULL;

	value = (char *)malloc(data_size);
	queue->data_size = data_size;
	retVal = initNode(&headNode, value, data_size);
	if (unlikely(retVal != RETURN_OK))
	{
		printf("init head node failed.\n");
		free(value);
		return RETURN_ERROR;
	}
	queue->q_head = headNode;
	queue->q_tail = headNode;
	queue->max_size = 200;
	queue->size = 0;
	pthread_mutex_init(&queue->q_head_lock, NULL);
	pthread_mutex_init(&queue->q_tail_lock, NULL);
	printf("init queue success.\n");
	free(value);
	return RETURN_OK;
};

int32_t pushQueue(listQueue *queue, void *value)
{
	int32_t retVal;
	listNode *node = NULL;

	retVal = initNode(&node, value, queue->data_size);
	if (unlikely(retVal != RETURN_OK))
	{
		printf("init node failed.\n");
	}
	pthread_mutex_lock(&queue->q_tail_lock);
	if (queue->size == queue->max_size)
	{
		//printf("queue size reaches max size.\n");
		pthread_mutex_unlock(&queue->q_tail_lock);
		return RETURN_ERROR;
	}
	queue->q_tail->next = node;
	queue->q_tail = node;
	queue->size++;
	//printf("num: %d\n", q->size);
	pthread_mutex_unlock(&queue->q_tail_lock);
	return RETURN_OK;
}

int32_t popQueue(listQueue *queue, void *val)
{
	int32_t retVal;
	listNode *h = NULL;
	listNode *newh = NULL;
	pthread_mutex_lock(&queue->q_head_lock);
	h = queue->q_head->next;
	if (h == NULL)
	{
		if (IsQueueEmpty(queue))
		{
			printf("queue is empty , sleep for a while\n");
		}
		pthread_mutex_unlock(&queue->q_head_lock);
		return RETURN_ERROR;
	}
	newh = h->next;
	queue->q_head->next = newh;
	memcpy(val, h->content, queue->data_size);
	free(h->content);
	free(h);
	queue->size--;
	pthread_mutex_unlock(&queue->q_head_lock);
	return RETURN_OK;
}

bool IsQueueEmpty(listQueue *queue)
{
	bool IsEmpty = false;
	pthread_mutex_lock(&queue->q_head_lock);
	IsEmpty = queue->size == 0;
	pthread_mutex_unlock(&queue->q_head_lock);
	return IsEmpty;
}

uint32_t GetQueueSize(listQueue *queue)
{
	uint32_t queueSize;
	pthread_mutex_lock(&queue->q_head_lock);
	queueSize = queue->size;
	pthread_mutex_unlock(&queue->q_head_lock);
	return queueSize;
}

void ShowQueueValue(listQueue *queue)
{
	listNode *node = queue->q_head;
	printf("size:%d\n", queue->size);
	while (node != NULL)
	{
		printf("num: %d\n", *(int *)node->content);
		node = node->next;
	}
}
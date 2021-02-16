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

	(*node)->content = value;

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
		printf("queue size reaches max size.\n");
		pthread_mutex_unlock(&queue->q_tail_lock);
		return RETURN_ERROR;
	}
	queue->q_tail->next = node;
	queue->q_tail = node;
	__atomic_fetch_add(&queue->size, 1, memory_order_relaxed);
	//printf("num: %ld\n", queue->size);
	pthread_mutex_unlock(&queue->q_tail_lock);
	return RETURN_OK;
}

int32_t popQueue(listQueue *queue, void **val_)
{
	int32_t retVal;
	listNode *h = NULL;
	listNode *newh = NULL;

	h = queue->q_head;
	newh = queue->q_head->next;
	if (newh == NULL)
	{
		if (IsQueueEmpty(queue))
		{
			printf("queue is empty , sleep for a while\n");
		}
		return RETURN_ERROR;
	}
	*val_ = newh->content;
	free(h);
	h = NULL;
	queue->q_head = newh;
	__atomic_fetch_sub(&queue->size, 1, memory_order_relaxed);
	return RETURN_OK;
}

bool IsQueueEmpty(listQueue *queue)
{
	bool IsEmpty = false;
	IsEmpty = (queue->q_head->next == NULL);
	return IsEmpty;
}

uint32_t GetQueueSize(listQueue *queue)
{
	uint32_t queueSize;
	queueSize = atomic_load(&queue->size);
	return queueSize;
}

void ShowQueueValue(listQueue *queue)
{
	listNode *node = queue->q_head;
	printf("size:%ld\n", queue->size);
	while (node != NULL)
	{
		//printf("num: %d\n", *(int *)node->content);
		node = node->next;
	}
}
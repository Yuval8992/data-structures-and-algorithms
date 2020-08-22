#include <assert.h> /*assert*/
#include <stdio.h> /*puts*/
#include <stdlib.h> /*malloc, free*/

#include "single_list.h"	
#include "queue.h"

/***********************************************************************************/

struct queue 
{
	slist_node_t *head;
	slist_node_t *tail;
};

/***********************************************************************************/

queue_t *QCreate(void)
{
	queue_t *queue = NULL;
	slist_node_t *stub_head = NULL;
	slist_node_t *stub_tail = NULL;
	void *data = NULL;

	stub_tail = SListCreateslist_node(&data, NULL);	
	if (NULL == stub_tail)
	{
		puts("memory error");
		return NULL;
	}
	stub_head = SListCreateslist_node(&data, stub_tail);	
	if (NULL == stub_head)
	{
		puts("memory error");
		free(stub_tail);
		return NULL;
	}
	queue = (queue_t *)malloc(sizeof(queue_t));
	
	if (NULL == queue)
	{
		puts("problem with malloc");
		free(stub_head);
		free(stub_tail);
		return NULL;
	}
	queue -> head = stub_head;
	queue -> tail = stub_tail;

	return queue;
}

/***********************************************************************************/

void QDestroy(queue_t *queue)
{
	assert(queue != NULL);

	SListFreeAll (queue -> head);
	free(queue);
}

/***********************************************************************************/

int QEnqueue(queue_t *queue, void *data)
{
	slist_node_t *new_node = NULL;

	assert(queue != NULL);
	assert(data != NULL);

	new_node = SListCreateslist_node(data, NULL);

	if (new_node == NULL)
	{
		return 1;
	}
	queue -> tail = SListInsert(queue -> tail, new_node) -> next;
	
	return 0;
}


/***********************************************************************************/

void QDequeue(queue_t *queue)
{
	assert(queue != NULL);

	SListRemoveAfter(queue -> head);
}

/***********************************************************************************/

void *QPeek(const queue_t *queue)
{
	assert(queue != NULL);
	
	return (queue -> head ->next -> data);
}

/***********************************************************************************/

int QIsEmpty(const queue_t *queue)
{
	assert(queue != NULL);

	return (queue -> head -> next == queue -> tail);

}

/***********************************************************************************/

size_t QSize(const queue_t *queue)
{
	size_t size = 0;

	assert(queue != NULL);
	size = SListCount(queue->head) - 2;
	
	return size;
}

/***********************************************************************************/

queue_t *QAppend(queue_t *to, queue_t *from)
{
	assert(to != NULL);
	assert(from != NULL);

	to -> tail -> next = from -> head;
	SListRemove(to -> tail);
	SListRemove(to -> tail);
	to -> tail = from -> tail;

	return to;
}













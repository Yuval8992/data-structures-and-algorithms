#include <stdlib.h> /*malloc, free*/ 
#include <assert.h> /*assert*/

#include "pq_heap.h" 
#include "heap.h"

struct pque
{
	heap_t *heap;
};

/**************************************************/

pque_t *PQCreate(int (*is_before)(const void *data1, const void *data2, void *param), void *param)
{
	pque_t *pque = NULL;

	assert(is_before);

	pque = malloc(sizeof(pque_t));
	if (NULL == pque)
	{
		return NULL;
	}

	pque->heap = HEAPCreate(is_before, param);
	if (NULL == pque->heap)
	{
		free(pque);
		return NULL;
	}

	return pque;
}

/**************************************************/

void PQDestroy(pque_t *pque)
{
	assert(NULL != pque);

	HEAPDestroy(pque->heap);
	free(pque); pque = NULL;
}

/**************************************************/

int PQEnque(pque_t *pque, void *data)
{
	assert(NULL != pque);
	assert(NULL != data);

	return HEAPPush(pque->heap, data);
}

/**************************************************/

void PQDeque(pque_t *pque)
{
	assert(NULL != pque);
	
	HEAPPop(pque->heap);
}

/**************************************************/

void *PQPeek(pque_t *pque)
{
	assert(NULL != pque);
	
	return HEAPPeek(pque->heap);
}

/**************************************************/

void PQErase(pque_t *pque, 
				int(*is_match)(const void *data1, const void *data2), 
				void *data)
{
	assert(NULL != pque);
	assert(NULL != is_match);
	assert(NULL != data);

	HEAPRemove(pque->heap, is_match, data);
}

/**************************************************/

int PQIsEmpty(pque_t *pque)
{
	assert(NULL != pque);

	return HEAPIsEmpty(pque->heap);
}

/**************************************************/

size_t PQSize(pque_t *pque)
{
	assert(NULL != pque);

	return HEAPSize(pque->heap);
}

/**************************************************/


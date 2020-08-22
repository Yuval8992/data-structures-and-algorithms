#include <stdlib.h> /*malloc, free*/ 
#include <assert.h> /*assert*/


#include "priority_que.h"

struct pque
{
	sorted_list_t *sdl;
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

	pque->sdl = SDListCreate(is_before, param);
	if (NULL == pque->sdl)
	{
		free(pque);
		return NULL;
	}

	return pque;
}

/**************************************************/

void PQDestroy(pque_t *pque)
{
	assert(pque);

	SDListDestroy(pque->sdl);
	free(pque);
}

/**************************************************/

int PQEnque(pque_t *pque, void *data)
{
	assert(pque);
	assert(data);

	return (SDListIsSameIter(SDListInsert(pque->sdl, data), SDListEnd(pque->sdl)));
}

/**************************************************/

void PQDeque(pque_t *pque)
{
	assert(pque);
	
	SDListPopFront(pque->sdl);
}

/**************************************************/

void *PQPeek(pque_t *pque)
{
	assert(pque);
	
	return SDListGetData(SDListBegin(pque->sdl));
}

/**************************************************/

void PQErase(pque_t *pque, 
				int(*is_match)(const void *data, const void *param), 
				const void *param)
{
	s_dlist_iter_t found = {NULL};

	assert(pque);
	assert(is_match);
	assert(param);

	found = SDListFindIf(SDListBegin(pque->sdl), SDListEnd(pque->sdl), is_match, param);

	if (SDListIsSameIter(found, SDListEnd(pque->sdl)) == 0)
	{
		SDListRemove(found);
	}
}

/**************************************************/

int PQIsEmpty(pque_t *pque)
{
	assert(pque);

	return SDListIsEmpty(pque->sdl);
}

/**************************************************/

size_t PQSize(pque_t *pque)
{
	assert(pque);

	return SDListSize(pque->sdl);
}

/**************************************************/

pque_t *PQAppend(pque_t *to, pque_t *from)
{
	assert(to);
	assert(from);

	SDListsMerge(to->sdl, from->sdl);

	return to;
}


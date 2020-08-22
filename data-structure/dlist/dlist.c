#include <stdlib.h> /*malloc, free*/
#include <stdio.h> /*puts*/
#include <assert.h> /*assert*/

#include "dlist.h"

/***************************************************************/

typedef struct d_node d_node_t;

struct d_node
{
	void *data;
	d_node_t *next;
	d_node_t *prev;  
};

struct dlist
{
	d_node_t head;
	d_node_t tail;
};

/***************************************************************/

dlist_t *DListCreate(void)
{
	dlist_t *dlist = NULL;
	
	dlist = (dlist_t *)malloc(sizeof(dlist_t));
	
	if (NULL == dlist)
	{
		return NULL;
	}
	dlist->head.data = NULL;
	dlist->head.next = &(dlist->tail);
	dlist->head.prev = NULL;
	
	dlist->tail.data = NULL;
	dlist->tail.next = NULL;
	dlist->tail.prev = &(dlist->head);

	return dlist;
}

/***************************************************************/

void DListDestroy(dlist_t *dlist)
{

	assert(NULL != dlist);
	
	while (dlist->head.next->next != NULL)
	{
		DListRemove((dlist->head).next);
	}
	free(dlist);
}

/***************************************************************/

dlist_iter_t DListInsert(dlist_t *dlist, dlist_iter_t where, void *data)
{
	dlist_iter_t new_node = NULL;
	
	assert(NULL != dlist);
	assert(NULL != data);
	
	new_node = (dlist_iter_t)malloc(sizeof(d_node_t));
	if (NULL == new_node)
	{
		return &(dlist->tail);
	}	
	
	new_node->data = data;
	new_node->next = where;
	new_node->prev = where->prev;
	where->prev->next = new_node;
	where->prev = new_node;
		
	return new_node;
}

/***************************************************************/

dlist_iter_t DListPushFront(dlist_t *dlist, void *data) 
{	
	assert(dlist != NULL);
	assert(data != NULL);
	
	return 	(DListInsert(dlist, dlist->head.next, data));
}

/***************************************************************/

dlist_iter_t DListPushBack(dlist_t *dlist, void *data)
{	
	assert(dlist != NULL);
	assert(data != NULL);
	
	return 	(DListInsert(dlist, &(dlist->tail), data));
}

/***************************************************************/

dlist_iter_t DListRemove(dlist_iter_t who)
{
	dlist_iter_t next_el = NULL;
	
	assert(NULL != who);
	
	next_el = who->next;
	who->prev->next = who->next;
	who->next->prev = who->prev;
	free(who);
	
	return next_el;
}

/***************************************************************/

void *DListPopFront(dlist_t *dlist)
{
	void *rm_data = NULL;

	assert(NULL != dlist);
	assert(1 != DListIsEmpty(dlist));
	
	rm_data = dlist->head.next->data;
	DListRemove(dlist->head.next);
	
	return rm_data;
}

/***************************************************************/

void *DListPopBack(dlist_t *dlist)
{
	void *rm_data = NULL;

	assert(NULL != dlist);
	assert(1 != DListIsEmpty(dlist));
	
	rm_data = dlist->tail.prev->data;	
	DListRemove(dlist->tail.prev);
	 
	return rm_data;
}

/***************************************************************/

dlist_iter_t DListBegin (const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return (dlist->head.next);
}

/***************************************************************/

dlist_iter_t DListEnd (const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return (dlist_iter_t)&(dlist->tail);
}

/***************************************************************/

dlist_iter_t DListNext (dlist_iter_t cur)  
{
	assert(NULL != cur);
	
	return (cur->next);
}

/***************************************************************/

dlist_iter_t DListPrev (dlist_iter_t cur) 
{
	assert(NULL != cur);
	
	return (cur->prev);
}

/***************************************************************/

void *DListGetData(dlist_iter_t who)
{
	assert(NULL != who);
	
	return (who->data);

}

/***************************************************************/

size_t DListSize(const dlist_t *dlist)
{
	size_t count = 0;
	dlist_iter_t temp = dlist-> head.next;
	
	assert(NULL != dlist);
	
	while (temp->next != NULL)
	{
		temp = DListNext(temp);
		++count;
	}
	
	return count;
}

/***************************************************************/

int DListIsEmpty(const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return (dlist->head.next->next == NULL);
}

/***************************************************************/

int DListIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

/***************************************************************/

dlist_iter_t DListFind(dlist_iter_t begin, dlist_iter_t end, 
						int (*is_match)(const void *data, const void *param),
						const void *param)
{
	d_node_t *search = NULL;
	int result = 0;
	
	assert(NULL != begin);
	assert(NULL != end);
	assert(NULL != param);
	
	for(search = begin; (DListIsSameIter(search, end) == 0) && (result == 0); search = DListNext(search))
	{
		result = is_match(search->data,param);
	}
	if (result == 1)
	{
		return search->prev;
	}
	
	return end;
}

/***************************************************************/

int DListForEach(dlist_iter_t begin, dlist_iter_t end,
				int (*do_func)(void *data, void *param),
				void *param)
{
	d_node_t *search = NULL;
	int check_if_error = 0;
	
	assert(NULL != begin);
	assert(NULL != end);	
	
	for (search = begin; (DListIsSameIter(search, end) == 0) && (check_if_error == 0); search = DListNext(search))
	{
		check_if_error = do_func(search->data,param);
	}
	if (check_if_error != 0)
	{
		return check_if_error;
	}
	
	return 0;
}

/***************************************************************/

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	dlist_iter_t ptr_to_prev = to->prev;

	assert(NULL != where);
	assert(NULL != from);
	assert(NULL != to);
	
	to->prev->next = where;
	to->prev = from->prev;
	from->prev->next = to;
	from->prev = where->prev;
	where->prev->next = from;
	where->prev = ptr_to_prev;

	return where;
}

/***************************************************************/

















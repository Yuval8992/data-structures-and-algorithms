#include <assert.h>  /*assert*/

#include "sorted_list.h"
#include "dlist.h"

/*************************************************************/
/*sorted_list managment struct*/
struct sorted_list
{
	dlist_t *dlist;
	int(*is_before)(const void *data, const void *el_data, void *param);
	void *param;
};

/*************************************************************/

sorted_list_t *SDListCreate(int (*is_before)(const void *data,
							const void *el_data,
							void *param), void *param)
{
	sorted_list_t *sorted_list = NULL;
	
	assert(is_before);
	
	sorted_list = malloc(sizeof(sorted_list_t));
	if (NULL == sorted_list)
	{
		return NULL;
	}
	
	sorted_list->dlist = DListCreate();
	if (NULL == sorted_list->dlist)
	{
		free(sorted_list);
		return NULL;
	}
	
	sorted_list->is_before = is_before;
	sorted_list->param = &param;

	return sorted_list;
}

/*************************************************************/

void SDListDestroy(sorted_list_t *s_dlist)
{
	assert(s_dlist);
	
	DListDestroy(s_dlist->dlist);
	free(s_dlist);
}

/*************************************************************/

size_t SDListSize(const sorted_list_t *s_dlist)
{
	assert(s_dlist);
	
	return DListSize(s_dlist->dlist);
}

/*************************************************************/

int SDListIsEmpty(const sorted_list_t *s_dlist)
{
	assert(s_dlist);
	
	return DListIsEmpty(s_dlist->dlist);
}

/*************************************************************/

s_dlist_iter_t SDListBegin(sorted_list_t *s_dlist)
{
	s_dlist_iter_t begin = {NULL};
	
	assert(s_dlist);
	
	begin.internal_iter = DListBegin(s_dlist->dlist);
	
	return begin;
}

/*************************************************************/

s_dlist_iter_t SDListEnd(sorted_list_t *s_dlist)
{
	s_dlist_iter_t end = {NULL};
	
	assert(NULL != s_dlist);
	
	end.internal_iter = DListEnd(s_dlist->dlist);
	
	return end;
}

/*************************************************************/

s_dlist_iter_t SDListNext(s_dlist_iter_t curr)
{	
	curr.internal_iter = DListNext(curr.internal_iter);
	
	return curr;
}

/*************************************************************/

s_dlist_iter_t SDListPrev(s_dlist_iter_t curr)
{		
	curr.internal_iter = DListPrev(curr.internal_iter);
	
	return curr;
}

/*************************************************************/

int SDListIsSameIter(s_dlist_iter_t iter1, s_dlist_iter_t iter2)
{
	return DListIsSameIter(iter1.internal_iter,iter2.internal_iter);
}

/*************************************************************/

void *SDListGetData(s_dlist_iter_t curr)
{
	return DListGetData(curr.internal_iter);
}

/*************************************************************/

s_dlist_iter_t SDListInsert(sorted_list_t *s_dlist, void *data)
{
	s_dlist_iter_t cur_iter = {NULL};
	
	assert(s_dlist);
	assert(data);
	
	cur_iter = SDListBegin(s_dlist);

	while ((0 == SDListIsSameIter(cur_iter, SDListEnd(s_dlist)))&&
	(0 == s_dlist->is_before(data, SDListGetData(cur_iter), s_dlist->param)))
	{
		cur_iter = SDListNext(cur_iter);
	}
	
	cur_iter.internal_iter = DListInsert(s_dlist->dlist, cur_iter.internal_iter, data);

	return cur_iter;
}

/*************************************************************/

s_dlist_iter_t SDListRemove(s_dlist_iter_t who)
{
	s_dlist_iter_t next_el = {NULL};
	
	next_el.internal_iter = DListRemove(who.internal_iter);
	
	return next_el;
}

/*************************************************************/

void *SDListPopFront(sorted_list_t *s_dlist)
{
	assert(s_dlist);

	return DListPopFront(s_dlist->dlist);
} 

/*************************************************************/

void *SDListPopBack(sorted_list_t *s_dlist)
{
	assert(s_dlist);

	return DListPopBack(s_dlist->dlist);
}

/*************************************************************/

int SDListForEach(s_dlist_iter_t from, s_dlist_iter_t to,
					int (*do_func)(void *data, void *param),
					void *param)
{
	return DListForEach(from.internal_iter, to.internal_iter, do_func, param);
}

/*************************************************************/

s_dlist_iter_t SDListFindIf(s_dlist_iter_t begin, s_dlist_iter_t end, 
							int(*is_match)(const void *data, const void *param), 
							const void *param)
{
	s_dlist_iter_t found_el = {NULL};
		
	found_el.internal_iter = DListFind(begin.internal_iter, end.internal_iter, is_match, param);
	
	return found_el; 
}

/*************************************************************/

s_dlist_iter_t SDListFind(sorted_list_t *s_dlist, s_dlist_iter_t begin, 
								s_dlist_iter_t end, const void *data_to_find)
{
	s_dlist_iter_t cur_iter = {NULL};
	
	assert(s_dlist);
	assert(data_to_find);
	
	cur_iter = begin;

	while ((SDListIsSameIter(cur_iter, end) == 0) && (s_dlist->is_before(data_to_find, SDListGetData(cur_iter), s_dlist->param) == 0))
	{
		cur_iter = SDListNext(cur_iter);
	}

	if ((SDListIsSameIter(SDListPrev(cur_iter), SDListPrev(SDListBegin(s_dlist))) == 0) && 
		(s_dlist->is_before(SDListGetData(SDListPrev(cur_iter)), data_to_find, s_dlist->param) == 0))

	{
		return SDListPrev(cur_iter);
	}
	
	return end;
}

/*************************************************************/

void SDListsMerge(sorted_list_t *dest, sorted_list_t *src)
{
	s_dlist_iter_t where = {NULL};
	s_dlist_iter_t from = {NULL};
	s_dlist_iter_t to = {NULL};
			
	assert(NULL != dest);
	assert(NULL != src);
	
	where = SDListBegin(dest);
	to = SDListBegin(src);
	from = SDListBegin(src);
	
	while (SDListIsSameIter(to,SDListEnd(src)) == 0)
	{
		while (SDListIsSameIter(where,SDListEnd(dest)) == 0 &&
		 (dest->is_before(SDListGetData(from), SDListGetData(where), dest->param) == 0))          
		{
			where = SDListNext(where);
		}

		if (SDListIsSameIter(where, SDListEnd(dest)) == 1)
		{
			to = SDListEnd(src);			  
		} 

		while ((SDListIsSameIter(to,SDListEnd(src)) == 0) && (dest->is_before(SDListGetData(to), SDListGetData(where), dest->param) == 1))
		{
			to = SDListNext(to);
		}
		
		DListSplice(from.internal_iter, to.internal_iter, where.internal_iter);

		from = to;
	}
}

/*************************************************************/

























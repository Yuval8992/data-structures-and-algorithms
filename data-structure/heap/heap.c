#include <stdlib.h> /*malloc,free*/
#include <assert.h> /*assert*/
#include <string.h> /* memcpy */

#include "heap.h"
#include "vector.h"

struct heap
{
	is_before_t is_before;
	void *param;
	vector_t *vector;
};

/********************************************************/

static void SwapData(heap_t *heap, size_t idx1, size_t idx2);
static void SiftDown(heap_t *heap, size_t size, size_t idx);
static int IsBefore(heap_t *heap, size_t idx1, size_t idx2);
static void SiftUp(heap_t *heap, size_t idx);

/********************************************************/

heap_t *HEAPCreate(is_before_t is_before, void *param)
{
	heap_t *heap = NULL;

	heap = malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}

	heap->is_before = is_before;
	heap->param = param;

	heap->vector = VectorCreate(sizeof(void *), 1);
	if (NULL == heap->vector)
	{
		free(heap);

		return NULL;
	}

	VectorPopBack(heap->vector); /* So that vector will start empty */

	return heap;
}

/********************************************************/

void HEAPDestroy(heap_t *heap)
{
	assert(NULL != heap);

	VectorDestroy(heap->vector);

	free(heap); heap = NULL;
}

/********************************************************/

int HEAPPush(heap_t *heap, const void *data)
{
	int parent = 0;
	int check_err = 0;

	assert(NULL != heap);
	assert(NULL != data);

	check_err = VectorPushBack(heap->vector, &data);
	if (check_err != 0)
	{
		return 1;
	}

	parent = VectorSize(heap->vector) - 1;

	SiftUp(heap, parent);

	return 0;
}

/********************************************************/

void HEAPPop(heap_t *heap)
{
	assert(NULL != heap);

	SwapData(heap, 0, VectorSize(heap->vector) - 1);

	VectorPopBack(heap->vector);

	SiftDown(heap, VectorSize(heap->vector), 0);
}

/********************************************************/


void *HEAPPeek(heap_t *heap)
{
	assert(NULL != heap);

	return (*(void **)VectorGetItemAdd(heap->vector, 0));
}

/********************************************************/

size_t HEAPSize(const heap_t *heap)
{
	assert(NULL != heap);

	return (VectorSize(heap->vector));
}

/********************************************************/

int HEAPIsEmpty(const heap_t *heap)
{
	assert(NULL != heap);

	return (0 == VectorSize(heap->vector));
}

/********************************************************/

void HEAPRemove(heap_t *heap, is_match_t is_match, void *data)
{
	size_t i = 0;
	size_t heap_size = 0;
	int parent_idx = 0;

	assert(NULL != heap);
	assert(NULL != data);

	heap_size = VectorSize(heap->vector);

	for (i = 0; i < heap_size; ++i)
	{
		if (is_match(*(void **)VectorGetItemAdd(heap->vector, i), data))
		{
			SwapData(heap, i, VectorSize(heap->vector) - 1);

			VectorPopBack(heap->vector);

			parent_idx = (i == 0) ? (0) : ((i - 1) / 2);

			if (IsBefore(heap, i, parent_idx))
			{
				SiftUp(heap, i);
			}
			else
			{
				SiftDown(heap, heap_size - 1, i);
			}

			return;		
		}
	}
}

/********************************************************/

static void SiftUp(heap_t *heap, size_t idx)
{
	int parent = 0;
		
	parent = (idx - 1) / 2;

	while((idx > 0) && IsBefore(heap, idx, parent))
	{
		SwapData(heap, idx, parent);
		idx = parent;
		parent = (idx - 1) / 2;
	}
}

/********************************************************/

static void SiftDown(heap_t *heap, size_t size, size_t parent_idx)
{
	size_t left = 0;
	size_t right = 0;
	size_t higher = 0;
	size_t new_parent_idx = parent_idx;

	left = (new_parent_idx * 2) + 1;
	right = (new_parent_idx * 2) + 2;

	if(left >= size) /*case of no children*/
	{
		return;
	}

	higher = left; 

	if (right < size) /*case of 2 children*/
	{
		higher = IsBefore(heap, left, right) ? left : right;
	}
		new_parent_idx = IsBefore(heap, higher, new_parent_idx) ? higher : new_parent_idx;

	if (new_parent_idx != parent_idx) /*in case sift needs to be repeated*/
	{
		SwapData(heap, new_parent_idx, parent_idx);
		SiftDown(heap, size, new_parent_idx);
	}
}

/********************************************************/
static void SwapData(heap_t *heap, size_t idx1, size_t idx2)
{
	size_t temp = 0;
	
	temp = *(size_t *)VectorGetItemAdd(heap->vector, idx1);
	*(size_t *)VectorGetItemAdd(heap->vector, idx1) = *(size_t *)VectorGetItemAdd(heap->vector, idx2);
	*(size_t *)VectorGetItemAdd(heap->vector, idx2) = temp;
}

/********************************************************/

static int IsBefore(heap_t *heap, size_t idx1, size_t idx2)
{
	return (heap->is_before(*(void **)VectorGetItemAdd(heap->vector, idx1), *(void **)VectorGetItemAdd(heap->vector, idx2), heap->param));
}

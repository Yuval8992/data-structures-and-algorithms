#include <stdlib.h> /*malloc, free*/
#include <stdio.h>  /*puts*/
#include <string.h> /*memcpy*/
#include <assert.h> /*assert*/
#include "vector.h"

#define FACTOR 2

/**************************************************************************/
struct vector
{
	void *head; 
	size_t size;
	size_t capacity;
	size_t element_size;
};

/**************************************************************************/
vector_t *VectorCreate(size_t element_size, size_t size)
{
	vector_t *vector = NULL;

	if (0 == element_size)		/*special case*/
	{
		puts("invalid element_size");
		return NULL; 
	}

	vector = malloc(sizeof(vector_t));

	if (NULL == vector)
	{
		puts("Allocation error");

		return NULL;
	}
	vector -> head = malloc(element_size * size);

	if (NULL == vector -> head)
	{
		puts("Allocation error");
		free(vector);

		return NULL;
	}
	vector -> size = size;
	vector -> capacity = vector -> size;
	vector -> element_size = element_size;
	
	return vector;
}

/**************************************************************************/
void VectorDestroy(vector_t *vector)
{
	assert(vector != NULL);
	
	free(vector -> head);
	free(vector);
}

/**************************************************************************/
int VectorPushBack(vector_t *vector, const void *el)
{	
	assert(vector != NULL);
	assert(el != NULL);

	if (vector -> size == vector -> capacity)
	{
		VectorReserve(vector, FACTOR * (vector -> size) * (vector -> element_size));
	}

	memcpy(VectorGetItemAdd(vector, (vector->size)), el, vector -> element_size);
	vector -> size += 1;

	return 0;
}

/**************************************************************************/
int VectorReserve(vector_t *vector, size_t capacity)
{
	void *temp = realloc((vector -> head), capacity);
	
	assert(vector != NULL);
	
	if (NULL == temp)
	{
		puts("Allocation error");
		return 1;
	}
	vector -> head = temp;
	vector -> capacity = capacity / (vector -> element_size);
		
	return 0;
}

/**************************************************************************/
void VectorPopBack(vector_t *vector)
{
	assert(vector != NULL);
	
	if (4 * (vector -> size) == (vector -> capacity))
	{
		VectorReserve(vector, (vector -> element_size) * (2 * (vector -> size)));
	}
	vector -> size -= 1;
}

/**************************************************************************/
void *VectorGetItemAdd(const vector_t *vector, size_t idx)
{
	assert(vector != NULL);

	/*check if idx is valid*/
	if (idx > VectorSize(vector))
	{
		puts("idx is not valid\n");
		return NULL;
	}
	
	return ((char *)(vector -> head) + ((vector -> element_size) * idx));
}

/**************************************************************************/
size_t VectorSize(const vector_t *vector)
{
	assert(vector != NULL);
		
	return (vector -> size);
}

/**************************************************************************/
size_t VectorCapacity(const vector_t *vector)
{
	assert(vector != NULL);
	
	return (vector -> capacity);
}




















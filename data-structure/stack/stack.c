#include <stdlib.h> /*malloc, free*/
#include <stdio.h>  /*puts*/
#include <assert.h> /*assert*/
#include <string.h> /*memcpy*/
#include "stack.h"

/**************************************************************************/
 struct stack
{
	void *current; 
	void *head;
	size_t size_of_element;
	
	#ifndef NDEBUG	
	void *end;
	#endif
};

/**************************************************************************/
stack_t *StackCreate(size_t capacity, size_t el_size) /*initalization of the struct and stack*/
{
	stack_t *stack = malloc(sizeof(stack_t));	
	if (NULL == stack)
	{
		puts("Allocation error");

		return NULL;
	}
	stack -> head = malloc(capacity * el_size);
	if (NULL == stack -> head)
	{
		puts("Allocation error");

		return NULL;
	}

	stack -> size_of_element = el_size;
	stack -> current = stack -> head;

	#ifndef NDEBUG
	stack -> end = (char *)stack -> head  + (capacity * el_size);
	#endif

	return stack;
}

/**************************************************************************/
void StackDestroy(stack_t *stack) /*destroy the stack*/
{
	assert(stack != NULL);
	free(stack -> head);
	free(stack);
}

/**************************************************************************/
int StackPush(stack_t *stack, const void *el) /*push new last member*/
{
	assert(stack != NULL);
	assert(el != NULL);

	if ((stack -> current) == (stack -> end))
	{
		puts("invalid action, your stack is full.");
		
		return 1;	
	}
	memcpy(stack -> current, el, stack -> size_of_element);
	stack -> current = (char *)stack -> current + stack -> size_of_element;

	return 0;
}

/**************************************************************************/
void StackPop(stack_t *stack) /*pop the last member*/
{
	assert(stack != NULL);
	if ((stack -> current) == (stack -> head))
	{
		puts("invalid action, your stack is empty.");
		
		return;	
	}
	
	stack -> current = ((char *)stack -> current) - (stack -> size_of_element);
}

/**************************************************************************/
void *StackPeek(const stack_t *stack) /*peak on last member*/
{
	assert(stack != NULL);
	
	return ((char *)(stack -> current) - (stack -> size_of_element));
}

/**************************************************************************/
size_t StackSize(const stack_t *stack) /*size of current stack*/
{
	assert(stack != NULL);
	
	return (((size_t)(stack -> current) - (size_t)(stack -> head)) / (stack -> size_of_element));
}

/**************************************************************************/
int StackIsEmpty(const stack_t *stack) /*check if the stack is empty*/
{
	assert(stack != NULL);
	
	return ((stack -> current) == (stack -> head));
}

/**************************************************************************/

void StackSorted(stack_t *stack)
{
	int peeked_num = *(int *)StackPeek(stack);

	if (1 == StackSize(stack))
	{
		return;
	} 

	StackPop(stack);
	StackSorted(stack);

	if (peeked_num > *(int *)StackPeek(stack))
	{
		int temp = *(int *)StackPeek(stack);
		StackPop(stack);
		StackPush(stack, &peeked_num);
		peeked_num = temp;
		StackSorted(stack);
	}
	
		StackPush(stack, &peeked_num);
}	












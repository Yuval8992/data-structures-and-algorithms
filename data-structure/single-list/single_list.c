#include <assert.h> /*assert*/
#include <stdio.h> /*puts*/
#include <stdlib.h> /*malloc, free*/

#include "single_list.h"	

slist_node_t *SListCreateslist_node(void *data, slist_node_t *next)
{
	slist_node_t *new_node = NULL;

	assert(data != NULL);

	new_node = (slist_node_t *)malloc(sizeof(slist_node_t));
	
	if (NULL == new_node)
	{
		puts("memory error");
		return NULL;
	}
	
	new_node -> data = data;
	new_node -> next = next;
	
	return new_node;
}

/***********************************************************************************/
void SListFreeAll(slist_node_t *head)
{
	slist_node_t *current = NULL;

	assert(SListHasLoop(head) != 1);
	assert(head != NULL);
	
	while (head != NULL)
	{
		current = head;
		head = head -> next;	
		free(current);
	}
} 

/***********************************************************************************/
slist_node_t *SListInsertAfter(slist_node_t *where, slist_node_t *new_slist_node)
{
	assert(new_slist_node != NULL);
	assert(where != NULL);
	
	new_slist_node -> next = where -> next; 
	where -> next = new_slist_node;

	return new_slist_node;
}

/***********************************************************************************/
slist_node_t *SListInsert(slist_node_t *where, slist_node_t *new_slist_node)
{
	void *temp_data = NULL;
	
	assert(new_slist_node != NULL);
	assert(where != NULL);

	SListInsertAfter(where, new_slist_node);

	/*switch between the data of where and the new node*/
	temp_data = new_slist_node -> data; 
	new_slist_node -> data = where -> data;
	where -> data = temp_data; 
	
	return where;
}

/***********************************************************************************/
slist_node_t *SListRemoveAfter(slist_node_t *slist_node)
{
	slist_node_t *temp = NULL;
	
	assert(slist_node != NULL);
	assert(slist_node -> next != NULL); /*check if slist_node is the last node*/

	temp = slist_node -> next;
	slist_node -> next = temp -> next;

	temp -> next = NULL;
	SListFreeAll(temp);

	return slist_node -> next;
}

/***********************************************************************************/
slist_node_t *SListRemove(slist_node_t *slist_node)
{
	slist_node_t *temp = NULL;
	
	assert(slist_node != NULL);
	assert(slist_node -> next != NULL); /*check if slist_node is the last node*/
	
	/*switch the data and next between current node and the node after and remove the node after*/
	temp = slist_node -> next;
	slist_node -> next = temp -> next;
	slist_node -> data = temp -> data; 

	temp -> next = NULL;
	SListFreeAll(temp);

	return slist_node;
}

/***********************************************************************************/
size_t SListCount(const slist_node_t *slist_node)
{
	size_t count = 0;
	
	assert(slist_node != NULL);	
	assert(SListHasLoop(slist_node) != 1);
	
	while (slist_node != NULL)
	{
		slist_node = slist_node -> next;	
		++count;
	}

	return count;
}

/***********************************************************************************/
slist_node_t *SListFind(slist_node_t *head, match_func_t is_match_func, void *param)
{	
	assert(head != NULL);
	assert(param != NULL);
	assert(SListHasLoop(head) != 1);
	
	while ((head != NULL) && (is_match_func(head -> data, param) != 1))
	{
		head = head -> next;
	}

	return head;
}

/***********************************************************************************/
int SListForEach(slist_node_t *head, action_func_t action_func, void *param)
{
	int err_found = 0;
	
	assert(head != NULL);
	assert(param != NULL);
	
	while ((err_found == 0) && (head != NULL))
	{
		err_found = action_func(head -> data, param);
		head = head -> next;
	}

	return err_found;
}

/***********************************************************************************/
int SListHasLoop(const slist_node_t *head)
{
	const slist_node_t *slow = NULL;
	const slist_node_t *fast = NULL;

	assert(head != NULL);

	slow = head;
	fast = head;

	while ((fast -> next != NULL) && (fast -> next -> next != NULL))
	{
		slow = slow -> next;
		fast = fast -> next -> next;
		
		if (slow == fast)
		{
			return 1;
		}
	}
	
	return 0;
}

/***********************************************************************************/
static slist_node_t *CheckIntersection(slist_node_t *head1, slist_node_t *head2, size_t diff); /*defenition of service function*/

slist_node_t *FindIntersection(slist_node_t *head1, slist_node_t *head2)
{
	size_t length1 = SListCount(head1);
	size_t length2 = SListCount(head2);
	size_t diff = 0;

	assert(head1 != NULL);
	assert(head2 != NULL);
		
	if (length1 > length2)
	{
		diff = length1 - length2;
		return CheckIntersection(head1, head2, diff);
	}
	else
	{
		diff = length2 - length1;
		return CheckIntersection(head2, head1, diff);
	}
}

/***********************************************************************************/
static slist_node_t *CheckIntersection(slist_node_t *head1, slist_node_t *head2, size_t diff)
{
	size_t i = 0;

	for (i = 0; i < diff; ++i)
	{
		head1 = head1 -> next;  /*promote head1 to the same number of nodes from the intesection (like head2), if there is one*/ 
	}	
	while (head2 != NULL)
	{
		if (head1 == head2)
		{	
			return head1;
		}
		
		head1 = head1 -> next;
		head2 = head2 -> next;
	}

	return NULL;
}

/***********************************************************************************/
slist_node_t *SListFlip(slist_node_t *head)
{
	slist_node_t *current = NULL, *previous = NULL;

	assert(head != NULL);
	
	while (head != NULL)
	{
		current = head -> next;
		head ->next = previous;
		previous = head;
		head = current;
	}
	
	return previous;
}

slist_node_t *SListFlipRecursive(slist_node_t *head)
{
	slist_node_t *new_head = NULL;

	if (NULL == head->next)
	{
		return head;
	}

	new_head = SListFlipRecursive(head->next);
	head->next->next = head;
	head->next = NULL;

	return new_head;
}












#include <stddef.h>

typedef int (*match_func_t)(const void *data, void *param); /* a comparecment function - will be created by the user */
typedef int (*action_func_t)(void *data, void *param); /* an action function - will be created by the user */

typedef struct slist_node slist_node_t;

struct slist_node
{
	void *data;  
	slist_node_t *next;
};


	/* create and destroy */
slist_node_t	*SListCreateslist_node(void *data, slist_node_t *next); /* return NULL if fails */ /* O(1) */

void			SListFreeAll(slist_node_t *head); /* O(n) */


	/* add and remove */
slist_node_t	*SListInsertAfter(slist_node_t *where, slist_node_t *new_slist_node); /* return pointer to the new node */ /* O(1) */

slist_node_t	*SListInsert(slist_node_t *where, slist_node_t *new_slist_node); /* insert before the provided node, return pointer to the new node */ /* O(1) */

slist_node_t	*SListRemoveAfter(slist_node_t *slist_node); /* return pointer to the next node */ /* O(1) */ 

slist_node_t	*SListRemove(slist_node_t *slist_node); /* remove the provided node, and return pointer to the next node */ /* O(1) */


	/* count */
size_t	SListCount(const slist_node_t *slist_node);	/* O(n) */


	/* find and activate */
slist_node_t	*SListFind(slist_node_t *head, match_func_t is_match_func, void *param); /* will return the first node that meet the requirement */ /* O(n) */

int				SListForEach(slist_node_t *head, action_func_t action_func, void *param); /* in case the function has been activated till last node will return 0, otherwise return != 0 and will stop in the first failure */ /* O(n) */


	/* check if loop and where */
int				SListHasLoop(const slist_node_t *head); /* return 1 if there is a loop, 0 otherwise */ /* O(n) */

slist_node_t	*FindIntersection(slist_node_t *head1, slist_node_t *head2); /* return null if Intersection has not been found*/ /* O(n) */


	/* flip */
slist_node_t	*SListFlip(slist_node_t *head); /* return pointer to the new head*/ /* O(n) */


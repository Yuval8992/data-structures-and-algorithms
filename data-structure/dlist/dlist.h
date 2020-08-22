#include <stddef.h> /* size_t */

typedef struct dlist dlist_t;
typedef struct d_node *dlist_iter_t;

 	/* create and destroy */
dlist_t *DListCreate(void);  /* return NULL if fails */ /* O(1) */
void DListDestroy(dlist_t *dlist);  /* O(n)*/


/* Add Elements returns END in case of failure, or iter to created element*/
dlist_iter_t DListInsert( dlist_t *dlist, dlist_iter_t where, void *data) ; /* O(1) (before)*/
dlist_iter_t DListPushFront(dlist_t *dlist, void *data); /* O(1) */
dlist_iter_t DListPushBack(dlist_t *dlist, void *data);  /* O(1) */


/* Remove Elements returns itarator to next or end if the last element was removed*/ 
dlist_iter_t DListRemove(dlist_iter_t who); /* O(1) */
void *DListPopFront(dlist_t *dlist); /* O(1) */
void *DListPopBack(dlist_t *dlist);  /* O(1) */

dlist_iter_t DListBegin (const dlist_t *dlist);/* O(1) */ /*Returns itarator to the first element*/
dlist_iter_t DListEnd (const dlist_t *dlist);  /* O(1) */ /*Returns itarator to the stub at the end*/
dlist_iter_t DListNext (dlist_iter_t cur);     /* O(1) */ /*Returns itarator to the next element*/
dlist_iter_t DListPrev (dlist_iter_t cur);     /* O(1) */ /*Returns itarator to the prev element*/

void *DListGetData(dlist_iter_t who);	/* O(1) */
size_t DListSize(const dlist_t *dlist);	/* O(n) */
int DListIsEmpty(const dlist_t *dlist); /* O(1) */

int DListIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2); /* O(1) */

dlist_iter_t DListFind(dlist_iter_t begin, dlist_iter_t end, 
						int (*is_match)(const void *data, const void *param),
						const void *param);/* O(n) */

int DListForEach  	(dlist_iter_t begin, dlist_iter_t end,
					int (*do_func)(void *data, void *param),
					void *param);/* O(n) */

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);  /* O(1) */





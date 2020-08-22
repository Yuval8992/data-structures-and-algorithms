#include <stddef.h> /* size_t */

#include "dlist.h"

typedef struct sorted_list sorted_list_t;

typedef struct iter_sdl
{
	dlist_iter_t internal_iter;
} s_dlist_iter_t;

 /* create and destroy */
/* checks if data1 (of new element) is before data2. while data1 is equal or after returns 0. when data1 is_before - return 1. */
sorted_list_t *SDListCreate(int (*is_before)(const void *data1, const void *data2, void *param), void *param);  /* return NULL if fails */ /* O(1) */
void SDListDestroy(sorted_list_t *s_dlist);  /* O(n)*/

 /* size and is empty */
size_t SDListSize(const sorted_list_t *s_dlist);	/* O(n) */
int SDListIsEmpty(const sorted_list_t *s_dlist); /* O(1) */

/* Find Iterators */ 
s_dlist_iter_t SDListBegin (sorted_list_t *s_dlist);/* O(1) */ /*Returns iterator to the first element*/
s_dlist_iter_t SDListEnd (sorted_list_t *s_dlist);  /* O(1) */ /*Returns iterator to after the last element*/
s_dlist_iter_t SDListNext (s_dlist_iter_t curr);     /* O(1) */ /*Returns iterator to the next element*/
s_dlist_iter_t SDListPrev (s_dlist_iter_t curr);     /* O(1) */ /*Returns iterator to the prev element*/

int SDListIsSameIter(s_dlist_iter_t iter1, s_dlist_iter_t iter2); /* O(1) */
void *SDListGetData(s_dlist_iter_t curr);	/* O(1) */

/* Add and Remove elements*/
/* Inserts element before element that returns from is_before function.
	 returns iter to inserted element or to end on failure*/
s_dlist_iter_t SDListInsert( sorted_list_t *s_dlist, void *data) ; /* O(n) */
s_dlist_iter_t SDListRemove(s_dlist_iter_t who); /* O(1) returns iter to element after removed one */


void *SDListPopFront(sorted_list_t *s_dlist); /* O(1) */
void *SDListPopBack(sorted_list_t *s_dlist);  /* O(1) */

/* iters -from element until element before to.
	stops on success or at the end of the list*/
int SDListForEach  	(s_dlist_iter_t from, s_dlist_iter_t to,
					int (*do_func)(void *data, void *param),
					void *param);/* O(n) */

/* merges src into dest. src stays empty (don't destroy) */
void SDListsMerge(sorted_list_t *dest, sorted_list_t *src);  /* O(n) */

/* Find functions. SDListFind searches for sorted data. FindIf searches for unsorted data. 
	Both find functions return iter to end if param/data_to_find does not match data in list*/ 
s_dlist_iter_t SDListFind(sorted_list_t *s_dlist, s_dlist_iter_t begin, s_dlist_iter_t end, const void *data_to_find);/* O(n) */
s_dlist_iter_t SDListFindIf(s_dlist_iter_t begin, s_dlist_iter_t end, int(*is_match)(const void *data, const void *param), const void *param);/* O(n) */



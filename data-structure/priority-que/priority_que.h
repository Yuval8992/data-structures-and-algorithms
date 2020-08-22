#include <stddef.h>		/*size_t*/

#include "sorted_list.h"	/*operation infrastructure*/

typedef struct pque pque_t;	/*managment struct of pque*/


pque_t *PQCreate(int (*is_before)(const void *data1, const void *data2, void *param), 								
				void *param);	
	
void PQDestroy(pque_t *pque);	/*free up all allocated memorey*/ /*O(n)*/

int PQEnque(pque_t *pque, void *data);	/*insert new node to the end of the queue O(1)*/

void PQDeque(pque_t *pque);	/*remove the first node from the queue. O(1)*/ 		
		
void *PQPeek(pque_t *pque);	/*peek last element*/ /*O(1)*/	
	
void PQErase(pque_t *pque, 
				int(*is_match)(const void *data, const void *param), 
				const void *param);		

int PQIsEmpty(pque_t *pque);	/*O(1)*/

size_t PQSize(pque_t *pque);	/*O(n)*/

pque_t *PQAppend(pque_t *to, pque_t *from); /*append between 2 different queues*/  /*O(1)*/





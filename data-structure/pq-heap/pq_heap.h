#include <stddef.h>		/*size_t*/

typedef struct pque pque_t;	/*managment struct of pque*/


pque_t *PQCreate(int (*is_before)(const void *data1, const void *data2, void *param), 								
				void *param);	
	
void PQDestroy(pque_t *pque);	/*free up all allocated memorey*/ 

int PQEnque(pque_t *pque, void *data);	/*insert new el to the end of the queue */

void PQDeque(pque_t *pque);	/*remove the first el from the queue */ 		
		
void *PQPeek(pque_t *pque);	/*peek last element*/ 	
	
void PQErase(pque_t *pque, 
				int(*is_match)(const void *data, const void *param), 
				void *param);		

int PQIsEmpty(pque_t *pque);	

size_t PQSize(pque_t *pque);	


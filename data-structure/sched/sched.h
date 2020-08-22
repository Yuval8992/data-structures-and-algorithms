#include <stddef.h>		/*size_t*/

#include "uid.h"

typedef struct sched sched_t;	/*managment struct of sched*/

/********************************************************/
/*creating of data struct*/

sched_t *SchedCreate(void); /* O(1) */

void SchedDestroy(sched_t *sched); /* O(n) */

/********************************************************/
/*element functions*//*O(1)*/

ilrd_uid_t SchedAddTask(sched_t *sched, size_t interval_in_sec, 
				  int (*action_func)(void *data), void *data); 
/*NOTE: the return value of "action_func" determine if task is perfoemd periodically or not
	0 - performed once
	1 - performed periodically
*/
int SchedRemoveTask(sched_t *sched, ilrd_uid_t task_uid); 	

/********************************************************/
/*sched execution*/

int SchedRun(sched_t *sched);
void SchedStop(sched_t *sched); /*O(1)*/

/********************************************************/
size_t SchedSize(sched_t *sched); /*O(n)*/
int SchedIsEmpty(sched_t *sched); /*O(1)*/

/********************************************************/
int SchedClear(sched_t *sched); /*O(n)*/






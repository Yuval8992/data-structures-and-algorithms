#include <stdlib.h> /*malloc, free*/ 
#include <assert.h> /*assert*/

#include "sched.h"
#include "priority_que.h"

#define UNUSED(x) (void)(x)

/*******************************************************/

struct sched
{
	pque_t *pque;
	int stop_status;
};

/*******************************************************/

typedef struct task task_t;

struct task
{
	ilrd_uid_t uid;
	void *data;
	int (*action_func)(void *data);
	size_t interval_in_sec;
	size_t total_time;
};

/*******************************************************/

static int IsBefore(const void *data1, const void *data2, void *param)
{
	UNUSED(param);
	
	assert(NULL != data1);
	assert(NULL != data2);

	return (((task_t *)data1)->total_time < ((task_t *)data2)->total_time);
}

/*******************************************************/

int IsMatchUid(const void *data, const void *param)
{
	int is_same_uid = UidIsSameUid(((task_t *)data)->uid, *(ilrd_uid_t *)param);

	if (is_same_uid)
	{
		free((task_t *)data);
	}
	return is_same_uid;	
}

/*******************************************************/

sched_t *SchedCreate(void)
{
	sched_t *new_sched = NULL;
	
	new_sched = malloc(sizeof(sched_t));
	if (NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched->pque = PQCreate(IsBefore, NULL);
	if (NULL == new_sched->pque)
	{
		free(new_sched);

		return NULL;
	}
	
	new_sched->stop_status = 0;
	
	return new_sched;
}

/*******************************************************/

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);
	PQDestroy(sched->pque);
	free(sched);
}

/*******************************************************/

ilrd_uid_t SchedAddTask(sched_t *sched, size_t interval_in_sec, 
				  int (*action_func)(void *data), void *data)
{

	task_t *new_task = NULL;
	
	assert(NULL != sched);
	assert(NULL != action_func);	
	
	new_task = malloc(sizeof(task_t));
	if (NULL == new_task)
	{
		return BAD_UID;
	}
	
	new_task->uid = UidCreate();
	if (UidIsSameUid(BAD_UID, new_task->uid))
	{
		free(new_task);

		return BAD_UID;
	}	

	new_task->data = data;
	new_task->action_func = action_func;
	new_task->interval_in_sec = interval_in_sec;
	new_task->total_time = time(NULL) + interval_in_sec;
	
	PQEnque(sched->pque, new_task);
		
	return new_task->uid;
}

/*******************************************************/

int SchedRemoveTask(sched_t *sched, ilrd_uid_t task_uid)
{
	assert(NULL != sched);

	PQErase(sched->pque, IsMatchUid, &task_uid);
	
	return 0;
}

/*******************************************************/

int SchedRun(sched_t *sched)
{
	int is_periodically = 0;
	task_t *current_task = NULL;
	
	assert(NULL != sched);

	/*initialized stop status in order to run again after stop*/
	sched->stop_status = 0;
	
	while ((sched->stop_status == 0) && (!SchedIsEmpty(sched)))
	{
		current_task = PQPeek(sched->pque); 

		if (time(NULL) >= (time_t)current_task->total_time)
		{
			is_periodically = current_task->action_func(current_task->data);

			if ((current_task == (task_t *)PQPeek(sched->pque)) && /*Check if the task remove himself*/
				(!SchedIsEmpty(sched))) /*IsEmpty for situation that there is only one task that remove himself*/
			{
				current_task->total_time += current_task->interval_in_sec;
				PQDeque(sched->pque);

				if (is_periodically)
				{	
					PQEnque(sched->pque, current_task);
				}
				else
				{
					free(current_task);
				}		
			}
		}
	}

	return 0;
}		

/********************************************************/

void SchedStop(sched_t *sched)
{
	assert(NULL != sched);
	
	sched->stop_status = 1;
}

/********************************************************/

size_t SchedSize(sched_t *sched)
{
	assert(NULL != sched);

	return (PQSize(sched->pque));
}

/********************************************************/

int SchedIsEmpty(sched_t *sched)
{
	assert(NULL != sched);

	return (PQIsEmpty(sched->pque));
}

/********************************************************/

int SchedClear(sched_t *sched)
{
	assert(NULL != sched);

	while (!SchedIsEmpty(sched))
	{
		free(PQPeek(sched->pque));
		PQDeque(sched->pque);
	}

	return 0;
}

/********************************************************/







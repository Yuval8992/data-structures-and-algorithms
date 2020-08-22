#include <unistd.h>	/*getpid*/
#include "uid.h"

const ilrd_uid_t BAD_UID = {-1, 0, 0};

/*******************************************************/

ilrd_uid_t UidCreate()
{
	static int counter = 0;
	
	ilrd_uid_t uid = {0};

	uid.time = time(NULL);
	uid.pid = getpid();
	uid.count = ++counter;

	if (-1 == uid.time)
	{
		return BAD_UID;
	}

	return uid;
}

/*******************************************************/

int UidIsSameUid(ilrd_uid_t u1, ilrd_uid_t u2)
{
	return ((u1.time == u2.time) && (u1.pid == u2.pid) && (u1.count == u2.count));
}




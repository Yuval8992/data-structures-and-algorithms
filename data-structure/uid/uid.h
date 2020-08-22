#include <stddef.h>	/*size_t*/
#include <time.h>	/*time_t*/
#include <sys/types.h>	/*pid_t*/

typedef struct ilrd_uid ilrd_uid_t;

/*NOTE: DO NOT USE INNER FIELDS OF STRUCT UID, WORK WITH UidCreate ONLY*/ 
struct ilrd_uid
{
	time_t time;
	pid_t pid;
	size_t count;
};

extern const ilrd_uid_t BAD_UID;

/*return uid or BAD_UID in case of failure*/ /*O(1)*/
ilrd_uid_t UidCreate();

int UidIsSameUid(ilrd_uid_t u1, ilrd_uid_t u2); /*O(1)*/





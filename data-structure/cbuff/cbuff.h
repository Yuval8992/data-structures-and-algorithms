#include <stddef.h>	/*size_t*/
#include <unistd.h> /*ssize_th*/

typedef struct cbuff cbuff_t;			/*managment struct of ds*/

/********************************************************/
/*creating of data struct*/ /*O(1)*/
cbuff_t *CBuffCreate(size_t capacity);			
void CBuffDestroy(cbuff_t *cbuff);

/********************************************************/
/*buffer actions*/
ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes);
ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes);

/********************************************************/
/*buffer status*/
size_t CBuffFreeSpace(const cbuff_t *cbuff);
size_t CBuffCapacity(const cbuff_t *cbuff);
int CBuffIsEmpty(const cbuff_t *cbuff);

/********************************************************/





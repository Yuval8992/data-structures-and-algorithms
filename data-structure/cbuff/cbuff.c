#include <stddef.h>	/*size_t*/
#include <unistd.h> /*ssize_t*/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "cbuff.h"


struct cbuff
{
	size_t start;
	size_t size;
	size_t capacity;
	char buff[1];
};

/********************************************************/

cbuff_t *CBuffCreate(size_t capacity)
{
	cbuff_t *cbuff = NULL;

	assert(capacity > 0);

	cbuff = malloc(sizeof(cbuff_t) + capacity - 1); /* -1 to decrease buff[1]*/
	if (NULL == cbuff)
	{
		return NULL;
	}	
	
	cbuff->start = 0;
	cbuff->size = 0;
	cbuff->capacity = capacity;
	*cbuff->buff = '\0';

	return cbuff;
}

/********************************************************/

void CBuffDestroy(cbuff_t *cbuff)
{
	assert(NULL != cbuff);

	free(cbuff);
}

/********************************************************/

ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes)
{
	ssize_t bytes_written = 0;

	assert(NULL != cbuff);
	assert(NULL != src);

	while ((ssize_t)nbytes > bytes_written)
	{
		cbuff->buff[(cbuff->start + cbuff->size) % cbuff->capacity] = *(char *)src;
		++cbuff->size;
		src = (char *)src + 1;
		++bytes_written;
	}
	if (cbuff->size > cbuff->capacity)
	{
		cbuff->start = (cbuff->start + cbuff->size) % cbuff->capacity;
		cbuff->size = cbuff->capacity;
	}
 
	return bytes_written;
}

/********************************************************/

ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes)
{
	ssize_t bytes_read = 0;	

	assert(NULL != cbuff);
	assert(NULL != dest);

	if (nbytes > cbuff->size)	
	{
		nbytes = cbuff->size;
	}
	while (bytes_read < (ssize_t)nbytes)
	{
		*(char *)dest = cbuff->buff[cbuff->start];
		dest = (char *)dest + 1;
		--cbuff->size;
		++bytes_read;
		++cbuff->start;
		cbuff->start = cbuff->start % cbuff->capacity;
	}

	return bytes_read;
}

/********************************************************/

size_t CBuffFreeSpace(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);

	return (cbuff->capacity - cbuff->size);
}

/********************************************************/

size_t CBuffCapacity(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);

	return cbuff->capacity;
}

/********************************************************/

int CBuffIsEmpty(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);

	return (0 == cbuff->size);
}



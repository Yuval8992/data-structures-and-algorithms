#include <stddef.h> /*size_t*/

typedef struct queue queue_t;


queue_t *QCreate(void); /*the user will recieve pointer to the new memory location, if allocation fails return NULL*/ /*O(1)*/

void QDestroy(queue_t *queue); /*free up all allocated memorey*/ /*O(n)*/

int QEnqueue(queue_t *queue, void *data); /*insert new node to the end of the queue O(1)*/

void QDequeue(queue_t *queue); /*remove the first node from the queue. O(1)*/ 

void *QPeek(const queue_t *queue); /*peek last element*/ /*O(1)*/

int QIsEmpty(const queue_t *queue); /*O(1)*/

size_t QSize(const queue_t *queue); /*O(n)*/

queue_t *QAppend(queue_t *to, queue_t *from); /*append between 2 different queues*/  /*O(1)*/


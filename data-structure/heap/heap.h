#include <stddef.h>		/*size_t*/

typedef struct heap heap_t;
typedef int (*is_before_t)(const void *data1, const void *data2, void *param);
typedef int (*is_match_t)(const void *data1, const void *data2);

/********************************************************/
heap_t *HEAPCreate(is_before_t is_before, void *param);
void HEAPDestroy(heap_t *heap);

/********************************************************/
int HEAPPush(heap_t *heap, const void *data);
void HEAPPop(heap_t *heap);
void *HEAPPeek(heap_t *heap);

/********************************************************/
size_t HEAPSize(const heap_t *heap);
int HEAPIsEmpty(const heap_t *heap);

/********************************************************/

void HEAPRemove(heap_t *heap, is_match_t is_match, void *data);

/********************************************************/


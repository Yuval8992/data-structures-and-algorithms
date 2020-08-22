#include <stddef.h> /*size_t*/

typedef struct stack stack_t;

stack_t *StackCreate (size_t capacity, size_t el_size);
void  StackDestroy   (stack_t *stack);
int StackPush        (stack_t *stack, const void *el); 
void StackPop        (stack_t *stack); 
void *StackPeek      (const stack_t *stack);
size_t StackSize     (const stack_t *stack); 
int StackIsEmpty     (const stack_t *stack);
void StackSorted (stack_t *stack);


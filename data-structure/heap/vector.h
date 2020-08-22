#include <stddef.h> /*size_t*/

typedef struct vector vector_t;

vector_t *VectorCreate(size_t element_size, size_t size);
void VectorDestroy(vector_t *vector);
int VectorPushBack(vector_t *vector, const void *el);
void VectorPopBack(vector_t *vector);
void *VectorGetItemAdd(const vector_t *vector, size_t idx);
size_t VectorSize(const vector_t *vector);
size_t VectorCapacity(const vector_t *vector);
int VectorReserve(vector_t *vector, size_t capacity);


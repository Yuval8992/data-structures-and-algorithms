#include <stddef.h>

typedef int (*is_before)(const void *data1, const void *data2);

void *BinarySearch(void *array, size_t nmemb, size_t memb_size, void *key, is_before is_before);
void *BinarySearchRec(void *array, size_t nmemb, size_t memb_size, void *key, is_before is_before);
void *JumpSearch(void *array, size_t nmemb, size_t num_of_rec, size_t memb_size, void *key, is_before is_before);




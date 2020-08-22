#include <stddef.h>		/*size_t*/

void BubbleSort(int *array, size_t size);
void InsertionSort(int *array, size_t size);
void SelectionSort(int *array, size_t size);
int CountSort(const int *array, size_t size, int lowest, int highest, int *result);
int RadixSort(size_t *array, size_t size, size_t nbits);
void MergeSort(void *base, size_t nmemb, size_t size,
               int (*is_before)(const void *, const void *));

void HeapSort(void *base, size_t nmemb, size_t size,
				 int (*compar)(const void *, const void *));

void QuickSort(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *));


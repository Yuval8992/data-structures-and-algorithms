#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <string.h> /*memcpy,memset*/
#include <stdio.h> /*fprint*/

#include "sort.h" /*sort API*/

static void SwapInt(int *num1, int *num2);
static void Rotate(int *array, size_t j, size_t i);
static int RadixCount(size_t *array, size_t *result,size_t *count_array,
                      size_t size, size_t nbits, size_t iteration);
static void Merge(void *base, size_t nmemb, size_t size,
                  int (*is_before)(const void *, const void *));
static void *GetAddress(void *start, size_t el_num, size_t el_size);
static void Heapify(void *base, size_t root, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
static void SwapData(void *base, size_t idx1, size_t idx2, size_t el_size);
static int Partition(void *base, size_t size, size_t nmemb, int (*compar)(const void *, const void *));


/*******************************************************/

void BubbleSort(int *array, size_t size)
{
    int is_sorted = 0;
    size_t i = 0 , k = 0;

    assert(NULL != array);

    while (0 == is_sorted)
    {
        is_sorted = 1;

        for (i = 0; i < size -1 - k; ++i)
        {
            if (array[i] > array[i + 1])
            {
                SwapInt(&(array[i]), &(array[i + 1]));
                is_sorted = 0;
            }
        }

		++k;
    }
}

/*******************************************************/

void InsertionSort(int *array, size_t size)
{
    size_t i = 0 ,j = 0;

    assert(NULL != array);

    for (i = 1; i < size; ++i)
    {
        if (array[i] < array[i -1])
        {
            j = i -1;
            while ( array[i] < array[j] && j > 0)
            {
                --j;
            }
            array[i] > array[j]? Rotate(array, j + 1 , i): Rotate(array, j, i);
        }      
    }
}

/*******************************************************/

void SelectionSort(int *array, size_t size)
{
    size_t i = 0, j = 0, lowest = 0;

    assert(NULL != array);

    for (i = 0; i < size - 1; ++i)
    {
        lowest = i;
        
        for (j = i + 1; j < size; ++j)
        {
            if (array[j] < array[lowest])
            {
                lowest = j;
            }
        }
        if (i != lowest)
        {
            SwapInt(&array[lowest], &array[i]);
        } 
    }
}

/*******************************************************/

int CountSort(const int *array, size_t size, int lowest, int highest, int *result)
{
    size_t range = 0;
    int i = 0, temp = 0;
    int *count_array = NULL; 

    assert(NULL != array);

    range = highest - lowest + 1;;

    count_array = (int *)calloc(range , sizeof(int));
    if (NULL == count_array)
    {
        return 1;
    }

    for (i = 0; i < size; ++i)
    {
        ++count_array[array[i] - lowest];
    }

    --count_array[0];

    for (i = 1; i < range; ++i)
    {
        count_array[i] += count_array[i - 1];
    }

    for (i = (size - 1); i >= 0; --i)
    {
        temp = count_array[array[i] - lowest];
        result[temp] = array[i];

        --count_array[array[i] - lowest];
    }

    free(count_array);
    count_array = NULL;

    return 0;
}

/*******************************************************/

int RadixSort(size_t *array, size_t size, size_t nbits) 
{
    size_t iterations = 0, iteration_number = 0;
    size_t biggest = 0 ,i = 0, count = 0;
    size_t *result = NULL; 
    size_t *count_array = NULL;

    assert(array != NULL);
	assert(nbits < 65);
	assert(nbits != 0);

    result = (size_t *)malloc(size * sizeof(size_t));
    if (NULL == result)
    {
        return 1;
    }

    count_array = (size_t *)calloc(1 << nbits, sizeof(size_t));
    if (NULL == count_array)
    {
        free(result);
        return 1;
    }

    for (i = 0; i < size; ++i)
    {
        if (biggest < array[i])
        {
            biggest = array[i];
        }
    }

    while (biggest > 0)
    {
        ++count;
        biggest >>= 1;
    }

    iterations = count / nbits; 

    if (count % nbits != 0) 
    {
        ++iterations;
    }

    for (iteration_number = 0; iteration_number < iterations; ++iteration_number)
    {
        RadixCount(array, result, count_array, size, nbits, iteration_number);
        memcpy(array, result, sizeof(size_t) * size);
        memset(count_array, 0, (1 << nbits) * sizeof(size_t));
    }

    free(count_array);
    count_array = NULL;

    free(result);
    result = NULL;

    return 0;
}

/*******************************************************/

static int RadixCount(size_t *array, size_t *result, size_t *count_array, 
                      size_t size, size_t nbits, size_t iteration_number)
{
    int i = 0;
    size_t temp = 0;
    size_t range = 0; 
    size_t mask = 0;

    range = (size_t)1 << nbits;
    mask = range - 1;

    for (i = 0; i < size; ++i)
    {
        ++count_array[(array[i] >> (nbits * iteration_number)) & mask];
    }

    --count_array[0];

    for (i = 1; i < range; ++i)
    {
        count_array[i] += count_array[i - 1];
    }

    for (i = (size - 1); i >= 0; --i)
    {
        temp = count_array[(array[i] >> (nbits * iteration_number)) & mask];
        result[temp] = array[i];

        --count_array[(array[i] >> (nbits * iteration_number)) & mask];
    }

    return 0;
}

/*******************************************************/

static void Rotate(int *array, size_t j, size_t i)
{
    int temp = array[i];

    while (i > j)
    {
        array[i] = array[i -1];
        --i;
    } 
    array[i] = temp;
}

/*******************************************************/

void MergeSort(void *base, size_t nmemb, size_t size,
           int (*is_before)(const void *, const void *))
{
    size_t left_size =  0;
    size_t right_size =  0;
    void *end = NULL;   
    void *middle = NULL;

    left_size = nmemb / 2;
    right_size = nmemb - left_size;
    end = GetAddress(base, nmemb - 1, size); 
    middle = GetAddress(base, left_size, size);

    if (base < end)
    {
        MergeSort(base, left_size, size, is_before);
        MergeSort(middle, right_size, size, is_before);
        Merge(base, nmemb, size, is_before);
    }
}

/*******************************************************/

static void Merge(void *base, size_t nmemb, size_t size,
           int (*is_before)(const void *, const void *))
 {
 	
    size_t left_size = 0;
    size_t right_size = 0;

    void *left_arr = NULL;
    void *right_arr = NULL;   
    void *ptr_right_arr = NULL;
    void *ptr_left_arr = NULL;

    void *left_end = NULL;
    void *right_end = NULL;

 	if (1 == nmemb)
 	{
 		return;
 	}
 	
    left_size = (nmemb / 2) * size;
    right_size = (nmemb * size) - left_size;
    
    left_arr = malloc(left_size);
    if (NULL == left_arr)
    {
        fprintf(stderr,"left_arr allocate failed\n");

        return;
    }

    right_arr = malloc(right_size);
    if (NULL == right_arr)
    {
        free(left_arr);
        fprintf(stderr,"right_arr allocate failed\n");

        return;
    }

    ptr_right_arr = right_arr;
    ptr_left_arr = left_arr;

    memcpy(left_arr, base, left_size);
    memcpy(right_arr, GetAddress(base, 1, left_size), right_size);

    left_end = GetAddress(left_arr, 1, left_size);
    right_end = GetAddress(right_arr, 1, right_size);

    while ((left_arr < left_end) && (right_arr < right_end)) 
    {
        if (1 == is_before(left_arr, right_arr))
        {
            memcpy(base, left_arr , size);
            left_arr = GetAddress(left_arr, 1, size);
        }
        else
        {
            memcpy(base, right_arr , size);
            right_arr = GetAddress(right_arr, 1, size);
        }

        base = GetAddress(base,1, size);
    }   

    if (left_arr < left_end)
    {   
        memcpy(base, left_arr, (size_t)left_end - (size_t)left_arr);
    }

    else
    {   
        memcpy(base, right_arr, (size_t)right_end - (size_t)right_arr);
    } 

    free(ptr_left_arr);
    ptr_left_arr = NULL;

    free(ptr_right_arr);
    ptr_right_arr = NULL;
}

/*************************************************/

void HeapSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
	int i = 0;

	for (i = (nmemb - 1) / 2; i >= 0; --i)
	{
		Heapify(base, i, nmemb, size, compar);
	}

	for (i = nmemb - 1; i >= 0; --i)
	{
		SwapData(base, 0, i, size);

		Heapify(base, 0, i, size, compar); 
	}
}

/***************************************************/

static void Heapify(void *base, size_t root, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
	size_t left = 0;
	size_t right = 0;
	size_t higher = 0;
	size_t parent = root;
	char *base1 = base;

	left = (root * 2) + 1;
	right = (root * 2) + 2;

	higher = left;

	if(left >= nmemb)
	{
		return;
	}

	if (right < nmemb)
	{
		higher = compar(base1 + (right * size), base1 + (left * size)) ? left : right;
	}
		parent = compar(base1 + (parent * size), base1 + (higher * size)) ? higher : parent;


	if (parent != root)
	{
		SwapData(base, parent, root, size);
		Heapify(base, parent, nmemb, size, compar);
	}
}

/*************************************************/

void QuickSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
 {
    int left_memb = 0, right_memb = 0;
    void *right_base = NULL;

    if (nmemb < 2)
    {
        return;
    }

    left_memb = Partition(base, size, nmemb, compar);
    
    right_memb = nmemb - left_memb - 1;
    right_base = GetAddress(base, left_memb + 1, size);

    QuickSort(base, left_memb, size, compar);
    QuickSort(right_base, right_memb, size, compar);
 }

/*************************************************/

static int Partition(void *base, size_t size, size_t nmemb, int (*compar)(const void *, const void *))
{
    int i = 0, j = 0;
    int pivot = (rand() % nmemb);

    SwapData(base, pivot, nmemb - 1, size);

    for (j = 0; j < nmemb - 1; ++j)
    {
        if(compar(GetAddress(base, j, size), GetAddress(base, nmemb -1, size))) 
        {
            SwapData(base, i, j, size);
            ++i;
        }
    }
    
    SwapData(base, nmemb - 1, i, size);

    return i;
}

/***************************************************/





/***************************************************/
/***************************************************/
/***************************************************/
static void SwapData(void *base, size_t idx1, size_t idx2, size_t el_size)
{
	unsigned char temp = '\0';
	unsigned char *p1 = (unsigned char *)base + (idx1 * el_size);
	unsigned char *p2 = (unsigned char *)base + (idx2 * el_size);

	size_t i = 0;

    for (i = 0; i < el_size; ++i)
    {
        temp = *(p1 + i);
        *(p1 + i) = *(p2 + i);
        *(p2 + i) = temp;
    }

}

/*******************************************************/

static void SwapInt(int *num1, int *num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

/*******************************************************/

static void *GetAddress(void *start, size_t index, size_t el_size)
{
	return ((char *)start + (index * el_size));
}

/*******************************************************/


















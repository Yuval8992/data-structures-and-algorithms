#include <math.h>
#include <assert.h>

#include "search.h"

/************************************************************************/

static void *GetAddress(void *start, size_t index, size_t el_size);

/************************************************************************/

void *BinarySearch(void *array, size_t nmemb ,size_t memb_size, void *key , is_before is_before)
{
    int mid = 0;
    int lower = 0;
    int higher = 0;

    assert (NULL != array);
    assert (NULL != key);

    higher = nmemb;

    while (lower <= higher)
    {
        mid = lower + ((higher - lower) / 2);

        if (is_before(key,GetAddress(array, mid, memb_size)))
        {
            higher = mid - 1;   
        }

        else if (is_before(GetAddress(array, mid, memb_size), key))
        {
            lower = mid + 1;
        }

        else
        {
            return GetAddress(array, mid ,memb_size);
        }

    }

    return NULL;
}

/************************************************************************/

void *BinarySearchRec(void *array, size_t nmemb, size_t memb_size, void *key , is_before is_before)
{
    size_t mid = 0;

    assert(NULL != array);
    assert(NULL != key);

    while (nmemb > 0)
    {
        mid = nmemb / 2;

        if (is_before(key, GetAddress(array, mid, memb_size)))
        {         
            nmemb = mid;

            return BinarySearchRec(array, nmemb, memb_size, key ,is_before);
        }

        else if (is_before(GetAddress(array, mid, memb_size), key))
        {
            nmemb = nmemb % 2 == 0 ? mid - 1 : mid;

            array = GetAddress(array, mid + 1, memb_size);

            return BinarySearchRec(array, nmemb, memb_size, key ,is_before);
        }

        else
        {
            return GetAddress(array, mid ,memb_size);
        }
    }

    return NULL;
}

/************************************************************************/

void *JumpSearch(void *array, size_t nmemb, size_t num_of_rec, size_t memb_size, void *key, is_before is_before)
{
    size_t jump = 0;
    size_t i = 0;

    assert(NULL != array);
    assert(NULL != key);

    if (num_of_rec > 1)
    {
        jump = sqrt(nmemb);
        i = jump;
        
        while (is_before(GetAddress(array, i, memb_size), key) && i < nmemb)
        {
            i += jump;
        }

        if (i > nmemb )
        {
            i = nmemb;
            jump -= (i - nmemb);
        }

        nmemb = jump;
        array = GetAddress(array, i - jump, memb_size);
        --num_of_rec;

        return JumpSearch(array, nmemb, num_of_rec, memb_size, key, is_before);
    }

    for (i = 0; i <= nmemb; ++i) 
    {
        if (!is_before(key, array) && (!is_before(array, key)))
        {
            return array;
        }

        array = GetAddress(array,1 , memb_size);
    }

    return NULL;
}


/*******************************************************/

static void *GetAddress(void *start, size_t index, size_t el_size)
{
	return ((char *)start + (index * el_size));
}

/*******************************************************/





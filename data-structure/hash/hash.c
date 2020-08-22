#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "hash.h"
#include "dlist.h"

struct hash
{
    hash_func_t hash_func;
    hash_cmp_func_t cmp_func;
    size_t table_size;
    dlist_t *hash_table[1];
};

/******************************************************************************/

hash_t *HASHCreate(hash_func_t hash_func, hash_cmp_func_t cmp_func, size_t table_size)
{
    size_t i = 0;
    hash_t *hash = NULL;

    hash = malloc((sizeof(hash_t) - sizeof(dlist_t *)) + (table_size * sizeof(void *)));
    if (NULL == hash)
    {
        return NULL;
    }

    hash->hash_func = hash_func;
    hash->cmp_func = cmp_func;
    hash->table_size = table_size;

    for (i = 0; i < table_size; ++i)
    {   
        if(NULL == (hash->hash_table[i] = DListCreate()))
        {
            break;
        }
    }
    
    if (i != table_size)
    {
        for (; i > 0; --i)
        {
            DListDestroy(hash->hash_table[i]);
        }
        
        free(hash);

        return NULL;
    }

    return hash;
}

/******************************************************************************/

void HASHDestroy(hash_t *hash)
{
    size_t i = 0;

    assert(NULL != hash);

    for (i = 0; i < hash->table_size; ++i)
    {
        DListDestroy(hash->hash_table[i]);
    }
    
    free(hash); hash = NULL;
}

/******************************************************************************/

int HASHInsert(hash_t *hash, const void* data)
{
    size_t idx = 0;
    dlist_t *dlist = NULL;

    assert(NULL != hash);
    assert(NULL != data);

    idx = hash->hash_func((void *)data) % hash->table_size;
    dlist = hash->hash_table[idx];

    return DListIsSameIter((DListPushFront(dlist, (void *)data)), DListEnd(dlist));
}

/******************************************************************************/

void HASHRemove(hash_t *hash, const void *data)
{
    size_t idx = 0;
    dlist_iter_t node_to_remove;
    dlist_t *dlist = NULL;

    assert(NULL != hash);
    assert(NULL != data);

    idx = hash->hash_func((void *)data) % hash->table_size;
    dlist = hash->hash_table[idx];
    node_to_remove = DListFind(DListBegin(dlist), DListEnd(dlist), hash->cmp_func, data); /*if not found return stub with null data*/

    if (NULL != DListGetData(node_to_remove)) 
    {
        DListRemove(node_to_remove);
    }
}

/******************************************************************************/
size_t HASHSize(const hash_t *hash)
{
    size_t i = 0, size = 0;

    assert(NULL != hash);

    for (i = 0; i < hash->table_size; ++i)
    {
        size += DListSize(hash->hash_table[i]);
    }

    return size;
}

/******************************************************************************/

int HASHIsEmpty(const hash_t *hash)
{
    size_t i = 0;

    assert(NULL != hash);

    for (i = 0; i < hash->table_size; ++i)
    {
        if (0 == DListIsEmpty(hash->hash_table[i]))
        {
            return 0;
        }
    }

    return 1;
}

/******************************************************************************/

void *HASHFind(const hash_t *hash, const void *data)
{
    size_t idx = 0;
    dlist_t *dlist = NULL;

    assert(NULL != hash);
    assert(NULL != data);

    idx = hash->hash_func((void *)data) % hash->table_size;
    dlist = hash->hash_table[idx];

    return DListGetData(DListFind(DListBegin(dlist), DListEnd(dlist), hash->cmp_func, data)); /*if not found return null*/
}

/******************************************************************************/

int HASHForEach(hash_t *hash, hash_action_func_t action_func, void *param)
{
    size_t idx = 0;
    dlist_t *dlist = NULL;

    assert(NULL != hash);

    for (idx = 0; idx < hash->table_size; ++idx)
    {
        dlist = hash->hash_table[idx];

        if (1 == DListForEach(DListBegin(dlist), DListEnd(dlist),action_func, param))
        {
            return 1;
        }

    }

    return 0;
}

/******************************************************************************/

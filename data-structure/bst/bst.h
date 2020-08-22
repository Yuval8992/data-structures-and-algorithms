#include <stddef.h>		/*size_t*/

typedef struct bst bst_t;
typedef struct bst_iter bst_iter_t;
typedef struct bst_node bst_node_t;

typedef int (*bst_is_before_func_t)(const void* key1, const void* key2, void *param);
typedef int (*bst_action_func_t)(void* data, void* param);

/************************* Functions ***************************/
bst_t *BSTCreate(bst_is_before_func_t func, void *param); 

bst_iter_t BSTInsert(bst_t *tree, const void* data);

/*removes the asked element - returns pointer to the next element*/
bst_iter_t BSTRemove(bst_iter_t iter);

/*returns the asked element iterator if found - else : returns end*/
bst_iter_t BSTFind(const bst_t *tree, void *data);

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action_func_t func, void *param);

int BSTIsEmpty(const bst_t *tree);

void BSTDestroy(bst_t *tree);

/***************** iter - functions **************/

/*the most left elelment in the tree*/
bst_iter_t BSTBegin(const bst_t *tree);

bst_iter_t BSTEnd(const bst_t *tree);

bst_iter_t BSTIterNext(bst_iter_t iter);

bst_iter_t BSTIterPrev(bst_iter_t iter);

void* BSTIterGetData(bst_iter_t iter);

int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2);


/**************** for internal use ONLY *********/

struct bst_iter
{
    bst_node_t *node;
    bst_t *tree;
};


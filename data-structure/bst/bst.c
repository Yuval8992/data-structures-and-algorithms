#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "bst.h"

struct bst
{
    bst_node_t *root;
    bst_is_before_func_t is_before;
    void *param;
};

struct bst_node
{
    bst_node_t *parent;
    bst_node_t *smaller;
    bst_node_t *bigger;
    void *data;
};

/***************************************************/

static bst_node_t *CreateNode(const void *data);

/***************************************************/

bst_t *BSTCreate(bst_is_before_func_t is_before, void *param)
{
    bst_t *bst = NULL;
    bst_node_t *end_stub = NULL;

    bst = malloc(sizeof(bst_t));
    if (NULL == bst)
    {
        return NULL;
    }

	bst->root = CreateNode(NULL);
    end_stub = CreateNode(NULL);

    bst->root->parent = end_stub; 
    bst->is_before = is_before;
    bst->param = param;

    return bst;
}

/***************************************************/

bst_iter_t BSTInsert(bst_t *tree, const void *data)
{
    bst_iter_t parent = {0};
    bst_iter_t current = {0};

    assert(NULL != tree);
    assert(NULL != data);

    if (tree->root->data == NULL)  /*case the tree is empty*/
    {
        tree->root->data = (void *)data;
        current.node = tree->root;
        current.tree = tree;

        return current;
    }

    else
    {
        current.node = tree->root;
         
        while (current.node != NULL)
        {
            parent = current;

            if (tree->is_before(data, BSTIterGetData(current), tree->param) == 1)
            {
                current.node = current.node->smaller;
            }
            else
            {
                current.node = current.node->bigger;
            }
        }    

        current.node = CreateNode(data);
        current.tree = tree;
        current.node->parent = parent.node;

        if (tree->is_before(data ,BSTIterGetData(parent), tree->param) == 1)
        {
            parent.node->smaller = current.node;
        }
        else
        {
            parent.node->bigger = current.node;
        }
    }

    return current;        
}

/***************************************************/

bst_iter_t BSTRemove(bst_iter_t iter)
{
    bst_iter_t next_iter = BSTIterNext(iter);
    bst_iter_t iter_to_replace ={0};

    if (iter.node->smaller == NULL && iter.node->bigger == NULL) /*remove node with no childs*/
    {
        if (iter.node == iter.tree->root)
        {
            iter.node->data = NULL;
        }
        else
        {
            if (iter.node->parent->bigger == iter.node)
            {
                iter.node->parent->bigger = NULL;

                free(iter.node);
            }
            else
            {
                iter.node->parent->smaller = NULL;

                free(iter.node);
            }
        }
    }
    
    else if (iter.node->smaller != NULL && iter.node->bigger != NULL) /*remove node with 2 childs*/
    {
        /*it means that the bigger iter after this iter its his right son*/ 
        if (iter.node->bigger->smaller == NULL) 
        {
            iter.node->bigger->smaller = iter.node->smaller;
            iter.node->smaller->parent = iter.node->bigger;
            iter.node->bigger->parent = iter.node->parent;

            if (iter.node == iter.tree->root)
            {
                iter.tree->root = iter.node->bigger;
            }
            else
            {
                if (iter.tree->is_before(iter.node->parent->data ,iter.node->bigger->data, NULL) == 1)
                {
                    iter.node->parent->bigger = iter.node->bigger;
                }
                else
                {
                    iter.node->parent->smaller = iter.node->bigger;
                }
            }
    
            free(iter.node);
        }   
        else /*it means that to find the bigger iter after this iter you need 
               go to his right son and go from him left intil you get NULL*/
        {
            iter_to_replace.node = iter.node->bigger;
            
            while (iter_to_replace.node->smaller != NULL)
            {
                iter_to_replace.node = iter_to_replace.node->smaller;
            }

            /*in case the founf iter have bigger child*/
            if (iter_to_replace.node->bigger == NULL)
            {
                iter_to_replace.node->parent->smaller = NULL;
            }
            else
            {
                iter_to_replace.node->parent->smaller = iter_to_replace.node->bigger;
                iter_to_replace.node->bigger->parent = iter_to_replace.node->parent;
            }
  
            iter_to_replace.node->parent = iter.node->parent;
            iter_to_replace.node->bigger = iter.node->bigger; 
            iter_to_replace.node->smaller = iter.node->smaller;

            iter.node->smaller->parent = iter_to_replace.node;
            iter.node->bigger->parent = iter_to_replace.node;
           
            if (iter.node != iter.tree->root)
            {
                if (iter.tree->is_before(iter_to_replace.node->data ,iter_to_replace.node->parent->data, NULL) == 1)
                {
                    iter_to_replace.node->parent->smaller = iter_to_replace.node;
                }
                else
                {
                    iter_to_replace.node->parent->bigger = iter_to_replace.node;
                }
            }
            else
            {
                iter.tree->root = iter_to_replace.node;
            }

            free(iter.node);
        }
    }
    
    else /*remove node with ono child*/
    {
        if (iter.node == iter.tree->root) 
        {
            if (iter.node->bigger != NULL)
            {
                iter.node->bigger->parent = iter.node->parent;
                iter.tree->root = iter.node->bigger;

                free(iter.node);
            }
            else
            {
                iter.node->smaller->parent = iter.node->parent;
                iter.tree->root = iter.node->smaller;

                free(iter.node);
            }
        }
        else if (iter.tree->is_before(iter.node->parent->data, iter.node->data ,NULL) == 1)
        {
            if (iter.node->bigger != NULL)
            {
                iter.node->parent->bigger = iter.node->bigger;
                iter.node->bigger->parent = iter.node->parent;

                free(iter.node);
            }
            else 
            {
                iter.node->parent->bigger = iter.node->smaller;
                iter.node->smaller->parent = iter.node->parent;

                free(iter.node);
            }
        }
        
        else
        {
            if (iter.node->bigger != NULL)
            {
                iter.node->parent->smaller = iter.node->bigger;
                iter.node->bigger->parent = iter.node->parent;

                free(iter.node);
            }
            else 
            {
                iter.node->parent->smaller = iter.node->smaller;
                iter.node->smaller->parent = iter.node->parent;

                free(iter.node);
            }
            
        }   
    }

    return next_iter;
}

/***************************************************/

bst_iter_t BSTFind(const bst_t *tree, void *data)
{
	bst_iter_t current = {0};

    assert (NULL != tree);
	assert (NULL != data);
    
    current.node = tree->root;
    current.tree = (bst_t *)tree;

	while (current.node != NULL)
	{
		if (tree->is_before(data, BSTIterGetData(current), tree->param) == 1)
		{
			current.node = current.node->smaller;
		}
		else if (tree->is_before(BSTIterGetData(current), data, tree->param) == 1)
		{
			current.node = current.node->bigger;
		}
        else
        {
			return current;
        }
	}

    return BSTEnd(tree);
}

/***************************************************/

int BSTForEach(bst_iter_t from, bst_iter_t to, bst_action_func_t func, void *param)
{
	int is_error = 0;

	while (!BSTIterIsSame(from, to) && (is_error == 0))
	{
		is_error = func(BSTIterGetData(from), param);
		from = BSTIterNext(from);
	}

	return is_error;
}

/***************************************************/

int BSTIsEmpty(const bst_t *tree)
{
    assert(NULL != tree);

	return (tree->root->data == NULL);
}

/**************************************************/

bst_iter_t BSTBegin(const bst_t *tree)
{
    bst_iter_t current = {0};

    assert(NULL != tree);

    if (BSTIsEmpty(tree) == 1)
	{
		return BSTEnd(tree);
	}
    
    current.node = tree->root;
    current.tree = (bst_t *)tree;

    while (current.node->smaller != NULL)
    {
        current.node = current.node->smaller;
    }

    return current;
}

/*****************************************************/

bst_iter_t BSTEnd(const bst_t *tree)
{
    bst_iter_t current = {0};

    assert(NULL != tree);

    current.node = tree->root;
    current.tree = (bst_t *)tree;
    
    current.node = current.node->parent;

	return current;
}

/**************************************************/

bst_iter_t BSTIterNext(bst_iter_t iter)
{
    if (iter.node->bigger != NULL)
    {
        iter.node = iter.node->bigger;

		while (iter.node->smaller != NULL)
		{
			iter.node = iter.node->smaller;
		}

	    return iter;
    }

    while (iter.node->parent->bigger == iter.node)
    {
        iter.node = iter.node->parent;
    }

    iter.node = iter.node->parent;    

    return iter;    
}

/**************************************************/

bst_iter_t BSTIterPrev(bst_iter_t iter)
{
    if (iter.node == iter.tree->root->parent)
    {
        iter.node = iter.tree->root;

        while (iter.node->bigger != NULL)
        {
           iter.node = iter.node->bigger;
        }
    }
    
    else if (iter.node->smaller != NULL)
    {
        iter.node = iter.node->smaller;

        while (iter.node->bigger != NULL)
        {
           iter.node = iter.node->bigger;
        }
    }

    else
    {
        while (iter.node == iter.node->parent->smaller)
        {
            iter.node = iter.node->parent;
        }

        iter.node = iter.node->parent;
    }

    return iter;
}

/**************************************************/

void *BSTIterGetData(bst_iter_t iter)
{
    return (iter.node->data);
}

/**************************************************/

int BSTIterIsSame(bst_iter_t iter1, bst_iter_t iter2)
{
    return (iter1.node == iter2.node);
}

/**************************************************/

void BSTDestroy(bst_t *tree)
{
	assert(tree != NULL);

	while (!BSTIsEmpty(tree))
	{
		BSTRemove(BSTBegin(tree));
	}

	free(BSTEnd(tree).node);
	free(tree->root); 
	free(tree);
}

/**************************************************/

static bst_node_t *CreateNode(const void *data)
{
    bst_node_t *node = NULL;

    node = malloc(sizeof(bst_node_t));
    if(NULL == node)
    {
        return NULL;
    }

    node->data = (void *)data;
    node->parent = node->smaller = node->bigger = NULL;

    return node;
}






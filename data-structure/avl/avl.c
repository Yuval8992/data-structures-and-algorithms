#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <stdio.h> /*print tree*/

#include "avl.h"

#define COUNT 11 

typedef enum {SMALLER, BIGGER} child_t;
typedef struct avl_node avl_node_t;

/*********************************************************************/

struct avl
{
    avl_node_t *root;
    avl_is_before_func_t is_before; 
    void *param;
};

struct avl_node
{
	avl_node_t *sons[2]; /* 0 - pointer to smaller, 1 - pointer to bigger */
    void *data;
    size_t height;
};

/*********************************************************************/

static avl_node_t *InsertRec(avl_t *tree, avl_node_t *head, void *data);
static avl_node_t *RemoveRec(avl_t *tree, avl_node_t *head, void *data);
static int ForEachRec(avl_node_t *head, avl_action_func_t action_func, void *param);
static void *FindRec(const avl_t *tree, const avl_node_t *head, const void *data);
static size_t SizeRec(avl_node_t *head);
static void DestroyRec(avl_node_t *head);

static avl_node_t *CreateNode(const void *data);
static avl_node_t *NextNode(avl_node_t * head);
static int IsRoot(avl_t *tree, avl_node_t *head);
static int IsMatch(const avl_t *tree, const void *data1, const void *data2);
static int UpdateHeight(avl_node_t *node);
static int FindBalance(avl_node_t *node);

static avl_node_t *LeftRotate(avl_t *tree, avl_node_t *node);
static avl_node_t *RightRotate(avl_t *tree, avl_node_t *node);
static avl_node_t *FindFixRotation(avl_t *tree, avl_node_t *node);

/*********************************************************************/

void PrintAVLTree2DUtil(avl_node_t *root, int space);
void PrintAVLTree2D(avl_node_t *root);

/*********************************************************************/

avl_t *AVLCreate(avl_is_before_func_t is_before, void *param)
{
    avl_t *avl = NULL;

    avl = malloc(sizeof(avl_t));
    if (NULL == avl)
    {
        return NULL;
    }

	avl->root = NULL;
    avl->is_before = is_before;
    avl->param = param;

    return avl;
}

/*********************************************************************/

int AVLInsert(avl_t *tree, void *data)
{
    assert(NULL != tree);
    assert(NULL != data);

    return (NULL == InsertRec(tree, tree->root, data));
}

/*********************************************************************/

static avl_node_t *InsertRec(avl_t *tree, avl_node_t *head, void *data)
{
    size_t idx = 0;

	if (NULL == head)
	{
		head = CreateNode(data);
		if(head == NULL)
		{
			return NULL;
		}

		if (AVLIsEmpty(tree))
		{
			tree->root = head;
		} 

		return head;
	}

	idx = tree->is_before(head->data, data, tree->param);
	head->sons[idx] = InsertRec(tree, head->sons[idx], data);

    head->height = UpdateHeight(head);
	head = FindFixRotation(tree, head);

    return head;
}

/*********************************************************************/

size_t AVLSize(const avl_t *tree)
{
    assert(NULL != tree);

    return SizeRec(tree->root);
}

/*********************************************************************/

static size_t SizeRec(avl_node_t *head)
{
    if (NULL == head)
    {
        return 0;
    }

    return 1 + SizeRec(head->sons[SMALLER]) + SizeRec(head->sons[BIGGER]);    
}

/*********************************************************************/

int AVLIsEmpty(const avl_t *tree)
{
    assert(NULL != tree);

    return (NULL == tree->root);
}

/*********************************************************************/

int AVLForEach(avl_t *tree, avl_action_func_t action_func, void *param)
{
    assert(NULL != tree);

	return (ForEachRec(tree->root, action_func, param));
}

/*********************************************************************/

static int ForEachRec(avl_node_t *head, avl_action_func_t action_func, void *param)
{
	int is_err = 0;

	if (NULL == head)
	{
		return 0;
	}

	is_err = ForEachRec(head->sons[SMALLER], action_func, param);

	if (0 == is_err)
	{	
		action_func(head->data, param);
	}

	if (0 == is_err)
	{	
		is_err += ForEachRec(head->sons[BIGGER], action_func, param);
	}

	return is_err;
}

/*********************************************************************/

void *AVLFind(const avl_t *tree, const void *data)
{
    assert(NULL != tree);
    assert(NULL != data);

	return FindRec(tree, tree->root, data);
}

/*********************************************************************/

static void *FindRec(const avl_t *tree, const avl_node_t *head, const void *data)
{
	size_t idx = 0;

    if (NULL == head)
    {
        return NULL;
    }
	
	if (IsMatch(tree, head->data, data))
	{
		return head->data;
	}

	idx = tree->is_before(head->data, data, tree->param);	
	return FindRec(tree, head->sons[idx], data);
}

/*********************************************************************/

void AVLRemove(avl_t *tree, const void *data)
{
    assert(NULL != tree);
    assert(NULL != data);

    RemoveRec(tree, tree->root, (void *)data);   
}

/*********************************************************************/

static avl_node_t *RemoveRec(avl_t *tree, avl_node_t *head, void *data)
{
    avl_node_t *replaced_node = NULL;
	size_t idx = 0;

	if (IsMatch(tree, head->data, data) == 0)
	{
		idx = tree->is_before(head->data, data, tree->param);	
		head->sons[idx] = RemoveRec(tree, head->sons[idx], data);
	}
    else
    {
        if (NULL == head->sons[SMALLER])
        {
            replaced_node = head->sons[BIGGER];
            tree->root = IsRoot(tree, head) ? replaced_node : tree->root; 
            free(head);

            return replaced_node;
        }

        else if (NULL == head->sons[BIGGER])
        {
            replaced_node = head->sons[SMALLER];
            tree->root = IsRoot(tree, head) ? replaced_node : tree->root; 
            free(head);

            return replaced_node;
        }

        else
        {
            replaced_node = NextNode(head);
            head->data = replaced_node->data;
            head->sons[BIGGER] = RemoveRec(tree, head->sons[BIGGER], replaced_node->data);
        }
    }

    head->height = UpdateHeight(head);
    head =  FindFixRotation(tree, head);

    return head;
}

/*********************************************************************/

size_t AVLHeight(const avl_t *tree)
{
    assert(NULL != tree);

    return (tree->root->height);
}

/*********************************************************************/

void AVLDestroy(avl_t *tree)
{
    assert(NULL != tree);

    DestroyRec(tree->root);

    free(tree);
}

/*********************************************************************/

static void DestroyRec(avl_node_t *head)
{
    if (NULL == head)
    {
        return;
    }

    DestroyRec(head->sons[SMALLER]);
    DestroyRec(head->sons[BIGGER]);
    free(head);
}

/*********************************************************************/

static avl_node_t *CreateNode(const void *data)
{
    avl_node_t *node = NULL;

    node = malloc(sizeof(avl_node_t));
    if (NULL == node)
    {
        return NULL;
    }

    node->data = (void *)data;
    node->sons[SMALLER] = node->sons[BIGGER] = NULL;
    node->height = 1;

    return node;
}

/*********************************************************************/

static avl_node_t *NextNode(avl_node_t * head)
{
    avl_node_t *iter = NULL;
    iter = head->sons[BIGGER];

    while (NULL != iter->sons[SMALLER])
    {
        iter = iter->sons[SMALLER];
    }

    return iter;
}

/*********************************************************************/

static int IsRoot(avl_t *tree, avl_node_t *head)
{
    return (tree->root == head);
}

/*********************************************************************/

static int IsMatch(const avl_t *tree, const void *data1, const void *data2)
{
	return (!tree->is_before(data1, data2, tree->param) &&
			!tree->is_before(data2, data1, tree->param));			 
}

/*********************************************************************/

static int UpdateHeight(avl_node_t *node)
{
	int left_height = 0;
	int right_height = 0;
	left_height = (node->sons[SMALLER] == NULL ? 0 : node->sons[SMALLER]->height);
	right_height = (node->sons[BIGGER] == NULL ? 0 : node->sons[BIGGER]->height);

	return (left_height > right_height ? left_height + 1 : right_height + 1);
}

/*********************************************************************/

static int FindBalance(avl_node_t *node)
{
	int left_height = 0;
	int right_height = 0;

	left_height = node->sons[SMALLER] == NULL ? 0 : node->sons[SMALLER]->height;
	right_height = node->sons[BIGGER] == NULL ? 0 : node->sons[BIGGER]->height;

	return (left_height - right_height); 
}

/*********************************************************************/

static avl_node_t *FindFixRotation(avl_t *tree, avl_node_t *node)
{
    if(FindBalance(node) > 1)
    {
        if (FindBalance(node->sons[SMALLER]) < 0)
        {
            node->sons[SMALLER] = RightRotate(tree, node->sons[SMALLER]);
        }

        return LeftRotate(tree, node);
    }

	else if(FindBalance(node) < -1)
	{
        if (FindBalance(node->sons[BIGGER]) > 0)
        {
            node->sons[BIGGER] = LeftRotate(tree, node->sons[BIGGER]);
        }
        
        return RightRotate(tree, node);
    }

    return node;
}

/*********************************************************************/

static avl_node_t *LeftRotate(avl_t *tree, avl_node_t *node)
{
	avl_node_t *temp = NULL;

	if(IsRoot(tree, node))
	{
		tree->root = node->sons[SMALLER];
	}

	temp = node->sons[SMALLER];
	node->sons[SMALLER] = node->sons[SMALLER]->sons[BIGGER];
	temp->sons[BIGGER] = node;

    node->height = UpdateHeight(node);
    temp->height = UpdateHeight(temp);

	return temp;
}

/*********************************************************************/

static avl_node_t *RightRotate(avl_t *tree, avl_node_t *node)
{
	avl_node_t *temp = NULL;

	if(IsRoot(tree, node))
	{
		tree->root = node->sons[BIGGER];
	}
    
	temp = node->sons[BIGGER];
	node->sons[BIGGER] = node->sons[BIGGER]->sons[SMALLER];
	temp->sons[SMALLER] = node;

    node->height = UpdateHeight(node);
    temp->height = UpdateHeight(temp);

	return temp;
}

/******************************************************************************/


void AVLTreePrint(const avl_t *tree)
{
    PrintAVLTree2D(tree->root);
}

/* Function to print binary tree in 2D  */
/*  It does reverse inorder traversal */
void PrintAVLTree2DUtil(avl_node_t *root, int space) 
{ 
    int i = 0;
     /* Base case  */
    if (root == NULL) 
        return; 
  
    /*  Increase distance between levels  */
    space += COUNT; 
  
    /* Process right child first  */
    PrintAVLTree2DUtil(root->sons[BIGGER], space); 
  
    /*Print current node after space 
     count */ 
    printf("\n"); 
    for (i = 0; i < space; i++) 
        printf(" "); 
    printf("%d\n", *((int *)root->data)); 
  
    /*  Process left child  */
    PrintAVLTree2DUtil(root->sons[SMALLER], space); 
} 
  
/* Wrapper over PrintAVLTree2DUtil()  */
void PrintAVLTree2D(avl_node_t *root) 
{ 
   /* Pass initial space count as 0 */
   PrintAVLTree2DUtil(root, 0); 
} 

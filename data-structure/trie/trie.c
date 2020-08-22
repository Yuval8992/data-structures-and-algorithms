#include <stdlib.h> /*malloc, free*/

#include "trie.h"
 
/*********************************************************/

typedef struct trie_node trie_node_t;

struct trie_node
{
	trie_node_t *sons[2];
	int is_occupied;
	void *data;
};

struct trie
{
	trie_node_t *root;
	size_t height;
};

/*********************************************************/

/* Recursive Functions */
static int TrieOccupyPathRec(trie_node_t *node, size_t bit_path, size_t height);
static int TrieOccupyNextFreePathRec(trie_node_t *node, size_t height, uint32_t *path);
static void TrieReleasePathRec(trie_node_t *node, size_t height, uint32_t path);
static void TrieDestroyRec(trie_node_t *node);

static int IsSonFree(trie_node_t *node, int *side);
static void UpdateOccupation(trie_node_t *node);
static size_t SwapBits(size_t bit_path, size_t length); /* In order to check path from left */
static trie_node_t *TrieCreateNode(void *data);

/*********************************************************/

trie_t *TrieCreate(size_t height)
{
	trie_t *trie = NULL;

	trie = (trie_t *)malloc(sizeof(trie_t));
	if (NULL == trie)
	{
		return NULL;
	}

	trie->root = TrieCreateNode(NULL);
	if (NULL == trie->root)
	{
		free(trie); trie = NULL;

		return NULL;
	}

	trie->height = height;

	return trie;
}

/***********************************************************/

int TrieOccupyNextFreePath(trie_t *trie, uint32_t *path)
{
	return TrieOccupyNextFreePathRec(trie->root, trie->height, path);
}


static int TrieOccupyNextFreePathRec(trie_node_t *node, size_t height, uint32_t *path)
{	
	int status = 0;
	int side = 0;

	if (0 == height)
	{
		node->is_occupied = 1;
		status = 0;

		return status;
	}

	else
	{
		if (IsSonFree(node, &side))
		{
			if (NULL == node->sons[side])
			{
				node->sons[side] = TrieCreateNode(NULL);
				if (NULL == node->sons[side])
				{
					status = 1;

					return status;
				}
			}

			*path = *path << 1;

			*path = (side == 1) ? *path | 1 : *path;

			TrieOccupyNextFreePathRec(node->sons[side], height - 1, path);
		}

		else /* if all addresses are occupied */
		{
			status = 1;

			return status;
		}		
	}

	UpdateOccupation(node);

	return status;
}

/***********************************************************/

int TrieOccupyPath(trie_t *trie, size_t path)
{
	path = SwapBits(path, trie->height); 

	return TrieOccupyPathRec(trie->root, path, trie->height);
}


static int TrieOccupyPathRec(trie_node_t *node, size_t path, size_t height)
{
	int side = 0;
	int status = 0;

	if (0 == height)
	{
		node->is_occupied = 1;

		return 0;
	}
	
	side = path & 1;

	if (NULL == node->sons[side])
	{
		node->sons[side] = TrieCreateNode(NULL);
		if (NULL == node->sons[side])
		{
			status = 1;

			return status;
		}
	}
	
	status = TrieOccupyPathRec(node->sons[side], path >> 1, height - 1);

	UpdateOccupation(node);

	return status;
}

/***********************************************************/

void TrieReleasePath(trie_t *trie, uint32_t path)
{
	path = SwapBits(path, trie->height);

	TrieReleasePathRec(trie->root, trie->height, path);
}

static void TrieReleasePathRec(trie_node_t *node, size_t height, uint32_t path)
{
	int side = 0;

	if (0 == height)
	{
		node->is_occupied = 0;

		return;
	}

	side = path & 1;

	TrieReleasePathRec(node->sons[side], height - 1, path >> 1);

	UpdateOccupation(node);
}

/***********************************************************/

void TrieDestroy(trie_t *trie)
{
	TrieDestroyRec(trie->root);

	free(trie); trie = NULL;
}


static void TrieDestroyRec(trie_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	TrieDestroyRec(node->sons[0]);
	TrieDestroyRec(node->sons[1]);

	free(node); node = NULL;
}

/***********************************************************/

static int IsSonFree(trie_node_t *node, int *side)
{
	int status = 0;

	if (node->sons[0] == NULL || node->sons[0]->is_occupied == 0)
	{
		*side = 0;
		status = 1;
	}
	else if (node->sons[1] == NULL || node->sons[1]->is_occupied == 0)
	{
		*side = 1;
		status = 1;
	}

	return status;
}

/***********************************************************/

static void UpdateOccupation(trie_node_t *node)
{
	int side = 0;

	if (IsSonFree(node, &side))
	{
		node->is_occupied = 0;
	}
	else
	{
		node->is_occupied = 1;
	}
}

/***********************************************************/

static size_t SwapBits(size_t bit_path, size_t length)
{
	size_t temp = 0;
	size_t i = 0;
	
	for (i = 0; i < length; ++i)
	{
		temp = ((temp << 1) | (bit_path & 1));
		bit_path >>= 1;
	}

	return temp;
}

/***********************************************************/

static trie_node_t *TrieCreateNode(void *data)
{
	trie_node_t *node = NULL;

	node = (trie_node_t *)malloc(sizeof(trie_node_t));
	if (NULL == node)
	{
		return NULL;
	} 

	node->data = data;

	node->is_occupied = 0;
	node-> sons[0] = node-> sons[1] = NULL;

	return node;
}

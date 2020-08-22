#include <stdlib.h> /*malloc, free*/

#include "dhcp.h"
#include "trie.h"

struct dhcp
{
	uint32_t net_add;
	trie_t *trie;
};

/********************************************************/

static int InitRestrictedPaths(dhcp_t *dhcp);
static size_t CalculateHeight(uint32_t net_mask);

/********************************************************/

dhcp_t* DHCPCreate(uint32_t net_add, uint32_t net_mask)
{
	dhcp_t *dhcp = NULL;
	size_t height = 0;
	int status = 0;

	dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (NULL == dhcp)
	{
		return NULL;
	}

	dhcp->net_add = net_add;

    height = CalculateHeight(net_mask);

	dhcp->trie = TrieCreate(height);
	if (NULL == dhcp->trie)
	{
		free(dhcp); dhcp = NULL;

		return NULL;
	}

	status = InitRestrictedPaths(dhcp);
	if (0 != status)
	{
		TrieDestroy(dhcp->trie);
		free(dhcp); dhcp = NULL;

		return NULL;
	}

	return dhcp;
}

/***********************************************************/

int DHCPAllocIp(dhcp_t *dhcp, uint32_t *addr, void *data)
{
	int status = 0;
	*addr = 0;

	status = TrieOccupyNextFreePath(dhcp->trie, addr);

	*addr = dhcp->net_add | *addr;

	return status;
}

/***********************************************************/

void DHCPRelease(dhcp_t *dhcp, uint32_t addr)
{
	addr = addr - dhcp->net_add;

	TrieReleasePath(dhcp->trie, addr); 
}

/***********************************************************/

void DHCPDestroy(dhcp_t *dhcp)
{
	TrieDestroy(dhcp->trie);
	free(dhcp); dhcp = NULL;
}

/***********************************************************/

static int InitRestrictedPaths(dhcp_t *dhcp)
{
	int status = 0;

	status = TrieOccupyPath(dhcp->trie, 0); /*Net address*/

    if (0 == status)
    {
	    status = TrieOccupyPath(dhcp->trie, ~0); /*Broadcast address*/
    }

    if (0 == status)
    {
	    status = TrieOccupyPath(dhcp->trie, (~0) - 1); /*Server address*/
    }

	return status;
}

/***********************************************************/

static size_t CalculateHeight(uint32_t net_mask)
{
    size_t count = 0;

    while ((net_mask & 1) != 1)
    {
        ++count;
        net_mask >>= 1;
    }

    return count;
}

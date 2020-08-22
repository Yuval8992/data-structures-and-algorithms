#include <stddef.h> /* size_t */
#include <stdint.h> /* uint32_t */

typedef struct dhcp dhcp_t;

/****************** API ************************/

dhcp_t* DHCPCreate(uint32_t net_add, uint32_t net_mask);

int DHCPAllocIp(dhcp_t *dhcp, uint32_t *addr, void *data);

void DHCPRelease(dhcp_t *dhcp, uint32_t addr);

void DHCPDestroy(dhcp_t *dhcp);




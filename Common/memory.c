#include "memory.h"

//逆序复制
void mem_copy_reverse(void * pdest, void * psrc, uint32_t len)
{
    uint32_t index;
    uint8_t * ptemp_src = (uint8_t*)psrc + (len - 1);
	uint8_t * ptemp_dest = (uint8_t*)pdest;
	
    for( index = 0; index < len; ++index )
        *(ptemp_dest++) = *(ptemp_src--);
}
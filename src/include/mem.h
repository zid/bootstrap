#ifndef MEM_H
#define MEM_H
#include "../../boot/mem.h"
#include <types.h>
void mem_init(struct mem_info *m);
void early_map(u64 virt, u64 phys, u32 flags);
enum PT_FLAGS
{
	PT_PRESENT = 1,
	PT_WR = 2,
	PT_USER = 4,
	PT_NX = 0x8000000000000000UL
};
#endif

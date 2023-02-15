#include <types.h>
#include <stdlib.h>
#include <mem.h>

#define RECURSE 510UL
#define RECURSE_PML4 (0xFFFFFF0000000000UL)

static u64 free_page;

static u64 early_palloc(void)
{
	u64 page;

	page = free_page;
	free_page += 4096;

	return page;
}

static u64 *page_table(u64 p3, u64 p2, u64 p1)
{
	return (u64 *)(RECURSE_PML4 | (p3 << 30) | (p2 << 21) | (p1 << 12));
}

void early_map(u64 virt, u64 phys, u32 flags)
{
	u64 *p;
	u64 pml4e, pdpte, pde, pte;

	pml4e = (virt >> 39) & 0x1FF;
	pdpte = (virt >> 30) & 0x1FF;
	pde   = (virt >> 21) & 0x1FF;
	pte   = (virt >> 12) & 0x1FF;

	if(!(flags & PT_WR))
		flags |= PT_NX;

	p = page_table(RECURSE, RECURSE, RECURSE) + pml4e;
	if(!*p)
		*p = early_palloc() | flags | PT_PRESENT;

	p = page_table(RECURSE, RECURSE, pml4e) + pdpte;
	if(!*p)
		*p = early_palloc() | flags | PT_PRESENT;

	p = page_table(RECURSE, pml4e, pdpte) + pde;
	if(!*p)
		*p = early_palloc() | flags | PT_PRESENT;

	p = page_table(pml4e, pdpte, pde) + pte;
	*p = phys | flags | PT_PRESENT;
}

void mem_init(struct mem_info *m)
{
	free_page = m->free;
}

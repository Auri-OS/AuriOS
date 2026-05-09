#ifndef MM_H
#define MM_H

#include <stdint.h>
#include <stdbool.h>

extern char kernel_end;

extern void pmm_init(uint32_t max_ram_addr, uint32_t kernel_end_addr);
extern void pmm_mark_region_free(uint32_t base, uint32_t size);
extern void pmm_mark_region_used(uint32_t base, uint32_t size);
extern uint32_t pmm_alloc_frame(void);
extern uint32_t pmm_free_frame(uint32_t phys_addr);
extern void pmm_dump_bitmap(uint32_t bytes_to_dump);
extern void mmu_init();
extern void mmu_enable();
extern uint8_t mmu_is_paging_enabled(void);
extern void mmu_map_page(uint32_t phys_addr, uint32_t virt_addr, bool is_writeable, bool is_user); 

extern void mmu_view_mappings(void);
extern void mmu_debug_peek(uint32_t addr);

#endif

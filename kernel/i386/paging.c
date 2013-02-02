#include <pirix/paging.h>
#include <pirix/memory.h>

void paging_init() {

}

paging_context* paging_create_context() {
    return 0;
}

int paging_map(paging_context* context, unsigned virt, unsigned phys, unsigned access) {
    return 0;
}

unsigned paging_map_kernel(unsigned phys) {
    return 0;
}

void paging_activate_context(paging_context* context) {
    return;
}

#include <stdlib.h>
#include "vnode.h"

static vnode* vnode_cache;
static unsigned vnode_cache_size;

void vnode_init_cache(unsigned size) {
    vnode_cache = calloc(size, sizeof(vnode));
    vnode_cache_size = size;
}

vnode* vnode_get_free() {
    for (unsigned i = 0; i < vnode_cache_size; i++) {
        if (vnode_cache[i].inode == 0) {
            return vnode_cache+i;
        }
    }
    return 0;
}

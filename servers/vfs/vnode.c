#include <stdlib.h>
#include "vnode.h"

static vnode** vnode_cache;
static unsigned vnode_cache_size;

void vnode_init(unsigned size) {
    vnode_cache = calloc(sizeof(vnode), size);
    vnode_cache_size = size;
}

vnode* vnode_get_free() {
    for (unsigned i = 0; i < vnode_cache_size; i++) {
        if (vnode_cache[i] == 0) return vnode_cache[i];
    }
    return 0;
}

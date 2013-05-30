#pragma once

#include <arch.h>

static inline void spin_lock(int* lock) {
    while (sync_lock(lock));
}

static inline void spin_unlock(int* lock) {
    sync_unlock(lock);
}

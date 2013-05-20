#pragma once

#include <arch.h>

static inline void spin_lock(int* lock) {
    while (test_lock(lock));
}

static inline void spin_unlock(int* lock) {
    *lock = 0;
}

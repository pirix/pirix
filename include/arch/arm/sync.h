#pragma once

static inline int sync_lock(int* ptr) {
    return __sync_lock_test_and_set(ptr, 0x01);
}

static inline void sync_unlock(int* ptr) {
    return __sync_lock_release(ptr);
}

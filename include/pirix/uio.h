#pragma once

#include <pirix/types.h>

struct iovec {
    void* iov_base;
    size_t iov_len;
};

ssize_t readv(int, const struct iovec*, int);
ssize_t writev(int, const struct iovec*, int);

#define SETIOV(_iov, _base, _len) (     \
   (_iov)->iov_base = (void*)(_base),   \
   (_iov)->iov_len = (_len) )

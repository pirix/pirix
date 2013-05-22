#pragma once

#include <termios.h>

#define IOCTLDTYPE

#define IOCTL_DTYPE_UNKNOWN -1
#define IOCTL_DTYPE_FILE     1
#define IOCTL_DTYPE_TTY      2

int ioctl(int fd, int request, ...);

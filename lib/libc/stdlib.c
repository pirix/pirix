#include <stdlib.h>

char* itoa(int value, char* buffer, int base) {
    char* charset = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* ret = buffer;
    char scratch[64];
    int idx = 0;
    if(value < 0) {
        *buffer++ = '-';
        value = -value;
    }
    if(value == 0) {
        *buffer++ = '0';
        *buffer = 0;
        return ret;
    }
    while(value > 0) {
        scratch[idx++] = charset[value % base];
        value /= base;
    }
    while(idx > 0) {
        *buffer++ = scratch[--idx];
    }
    *buffer = 0;
    return ret;
}

void exit(int status) {
  return;
}

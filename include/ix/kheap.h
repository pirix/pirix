#ifndef IX_KMALLOC_H
#define IX_KMALLOC_H

typedef unsigned size_t;

void* kmalloc(size_t size);
void kfree(void* addr);

#endif

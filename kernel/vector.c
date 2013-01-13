#include <pirix/vector.h>
#include <pirix/kheap.h>

void vector_init(vector* self) {
    self->content = kcalloc(8, sizeof(void*));
    self->bounds = 8;
    self->count = 0;
}

int vector_add(vector* self, void* value) {
    for (int i = 0; i < self->bounds; i++) {
        if (!self->content[i]) {
            self->content[i] = value;
            self->count++;
            return i;
        }
    }
    return -1;
}

void vector_set(vector* self, int index, void* value) {
    if (index > self->bounds) return;
    if (self->content[index] && !value) self->count--;
    else if (!self->content[index] && value) self->count++;
    self->content[index] = value;
}

void* vector_get(vector* self, int index) {
    if (index > self->bounds) return 0;
    return self->content[index];
}

int vector_count(vector* self) {
    return self->count;
}

void vector_clear(vector* self) {
    kfree(self->content, sizeof(void*)*self->bounds);
    self->content = 0;
}

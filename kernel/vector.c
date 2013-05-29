#include <pirix/vector.h>
#include <pirix/kheap.h>
#include <string.h>

static void vector_resize(vector* self) {
    if (self->bounds > 0) {
        void** tmp = kmalloc(self->bounds*2*sizeof(void*));
        memcpy(tmp, self->content, self->bounds*sizeof(void*));
        memset(tmp+self->bounds, 0, self->bounds*sizeof(void*));
        kfree(self->content, self->bounds*sizeof(void*));
        self->bounds *= 2;
        self->content = tmp;
    }
    else {
        vector_init(self, 4);
    }
}

void vector_init(vector* self, int size) {
    if (size > 0) {
        self->content = kcalloc(size, sizeof(void*));
    }
    self->bounds = size;
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

    vector_resize(self);
    return vector_add(self, value);
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
    if (self->bounds <= 0) return;
    memset(self->content, 0, sizeof(void*)*self->bounds);
    self->count = 0;
}

void vector_free(vector* self) {
    if (self->bounds <= 0) return;
    kfree(self->content, sizeof(void*)*self->bounds);
    self->bounds = 0;
    self->count = 0;
    self->content = 0;
}

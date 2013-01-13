#pragma once

typedef struct vector {
    int size;
    int bounds;
    void** content;
} vector;

void vector_init(vector* self);
int vector_add(vector* self, void* value);
void vector_set(vector* self, int index, void* value);
void* vector_get(vector* self, int index);
void vector_delete(vector* self);

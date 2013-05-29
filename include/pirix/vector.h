#pragma once

typedef struct vector {
    int count;
    int bounds;
    void** content;
} vector;

void vector_init(vector* self, int size);
int vector_add(vector* self, void* value);
void vector_set(vector* self, int index, void* value);
void* vector_get(vector* self, int index);
int vector_count(vector* self);
void vector_clear(vector* self);
void vector_free(vector* self);

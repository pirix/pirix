#pragma once

/**
 * A dynamic array-like data structure.
 */
typedef struct vector {
    int count;
    int bounds;
    void** content;
} vector;

/**
 * Intitialize a vector with an initial size.
 * @memberof vector
 */
void vector_init(vector* self, int size);

/**
 * Add an element to a vector.
 * @return The index of the added element.
 * @memberof vector
 */
int vector_add(vector* self, void* value);

/**
 * Insert an element at a specified index.
 * Does nothing if the index is out of bounds.
 * @memberof vector
 */
void vector_set(vector* self, int index, void* value);

/**
 * Get the element at the specified index.
 * @return Either the element or NULL.
 * @memberof vector
 */
void* vector_get(vector* self, int index);

/**
 * Get the element count.
 * @return The amount of elements the vector holds.
 * @memberof vector
 */
int vector_count(vector* self);

/**
 * Removes all elements from the vector.
 * @memberof vector
 */
void vector_clear(vector* self);

/**
 * Frees all memory allocated for the vector.
 * Only vector_init() is safe to use thereafter.
 * @memberof vector
 */
void vector_free(vector* self);

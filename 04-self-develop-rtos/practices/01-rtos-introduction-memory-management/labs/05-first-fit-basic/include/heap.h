#ifndef LAB05_HEAP_H
#define LAB05_HEAP_H

#include <stdbool.h>
#include <stddef.h>

bool heap_init(void *memory, size_t size);
void *heap_alloc(size_t size);
bool heap_free(void *pointer);
size_t heap_capacity(void);
bool heap_is_free(void);

#endif

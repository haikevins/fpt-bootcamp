#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_heap_alloc_whole_block(void)
{
    uint8_t memory[128];
    void *pointer;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    pointer = heap_alloc(8U);
    TEST_ASSERT(pointer != (void *)0);
    TEST_ASSERT(heap_alloc(8U) == (void *)0);
    TEST_ASSERT(heap_free(pointer));
    TEST_ASSERT(heap_is_free());
    return 0;
}

int test_heap_rejects_bad_free(void)
{
    uint8_t memory[128];
    uint8_t foreign;
    void *pointer;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    pointer = heap_alloc(8U);
    TEST_ASSERT(!heap_free(&foreign));
    TEST_ASSERT(heap_free(pointer));
    TEST_ASSERT(!heap_free(pointer));
    return 0;
}

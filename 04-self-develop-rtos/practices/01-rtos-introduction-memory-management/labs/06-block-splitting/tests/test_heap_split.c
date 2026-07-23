#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_split_allows_multiple_allocations(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    a = heap_alloc(32U);
    b = heap_alloc(48U);
    TEST_ASSERT(a != (void *)0);
    TEST_ASSERT(b != (void *)0);
    TEST_ASSERT(a != b);
    TEST_ASSERT(heap_block_count() >= 3U);
    TEST_ASSERT(heap_validate());
    return 0;
}

int test_free_does_not_coalesce_yet(void)
{
    uint8_t memory[512];
    void *a;
    void *b;
    size_t before;
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    a = heap_alloc(32U);
    b = heap_alloc(32U);
    before = heap_block_count();
    TEST_ASSERT(heap_free(a));
    TEST_ASSERT(heap_free(b));
    TEST_ASSERT(heap_block_count() == before);
    TEST_ASSERT(heap_validate());
    return 0;
}

#include "test.h"
#include "heap.h"
#include <stdint.h>

int test_heap_init_valid(void)
{
    uint8_t memory[128];
    TEST_ASSERT(heap_init(memory, sizeof(memory)));
    TEST_ASSERT(heap_capacity() > 0U);
    TEST_ASSERT(heap_is_free());
    return 0;
}

int test_heap_init_invalid(void)
{
    TEST_ASSERT(!heap_init((void *)0, 0U));
    return 0;
}

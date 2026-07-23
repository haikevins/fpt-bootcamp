#include "test.h"

int test_heap_init_valid(void);
int test_heap_init_invalid(void);
int test_heap_alloc_whole_block(void);
int test_heap_rejects_bad_free(void);

int main(void)
{
    TEST_RUN(test_heap_init_valid);
    TEST_RUN(test_heap_init_invalid);
    TEST_RUN(test_heap_alloc_whole_block);
    TEST_RUN(test_heap_rejects_bad_free);
    return 0;
}

#include "test.h"

int test_stack_monitor_empty(void);
int test_stack_monitor_downward(void);
int test_stack_monitor_upward(void);

int main(void)
{
    TEST_RUN(test_stack_monitor_empty);
    TEST_RUN(test_stack_monitor_downward);
    TEST_RUN(test_stack_monitor_upward);
    return 0;
}

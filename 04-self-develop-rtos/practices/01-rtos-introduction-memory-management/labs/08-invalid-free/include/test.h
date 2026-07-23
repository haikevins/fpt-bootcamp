#ifndef COMMON_TEST_H
#define COMMON_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(condition)                                         \
    do                                                                 \
    {                                                                  \
        if (!(condition))                                              \
        {                                                              \
            fprintf(stderr, "FAIL %s:%d: %s\n",                      \
                    __FILE__, __LINE__, #condition);                   \
            return 1;                                                  \
        }                                                              \
    } while (0)

#define TEST_RUN(function)                                             \
    do                                                                 \
    {                                                                  \
        const int result = (function)();                               \
        if (result != 0)                                               \
        {                                                              \
            return result;                                             \
        }                                                              \
        printf("PASS: %s\n", #function);                              \
    } while (0)

#endif

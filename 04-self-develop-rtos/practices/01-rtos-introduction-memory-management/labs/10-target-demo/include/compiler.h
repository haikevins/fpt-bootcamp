#ifndef COMMON_COMPILER_H
#define COMMON_COMPILER_H

#if defined(__GNUC__) || defined(__clang__)
#define ATTR_USED               __attribute__((used))
#define ATTR_WEAK               __attribute__((weak))
#define ATTR_NORETURN           __attribute__((noreturn))
#define ATTR_SECTION(name)      __attribute__((section(name)))
#define ATTR_ALIGNED(value)     __attribute__((aligned(value)))
#else
#define ATTR_USED
#define ATTR_WEAK
#define ATTR_NORETURN
#define ATTR_SECTION(name)
#define ATTR_ALIGNED(value)
#endif

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#endif

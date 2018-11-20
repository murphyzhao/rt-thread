#include "utest.h"
#include <rtthread.h>

void _utest_assert(int cond, const char *file, int line, const char *msg);

#define utest_assert(value, file, line, func, msg, ...)    \
    _utest_assert(value, file, line, msg)

#define utest_assert_true(value, msg, ...) utest_assert(value, \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is false, expected true value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_false(value, msg, ...) utest_assert(!(value), \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is true, expected false value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

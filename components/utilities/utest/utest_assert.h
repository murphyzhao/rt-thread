#include "utest.h"
#include <rtthread.h>

void _utest_assert(int cond, int32_t err_level, const char *file, int line, const char *msg);

#define utest_assert(value, err_level, file, line, func, msg, ...)    \
    _utest_assert(value, err_level, file, line, msg)

#define utest_assert_true(value, err_level, msg, ...) utest_assert(value, err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is false, expected true value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_false(value, err_level, msg, ...) utest_assert(!(value), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is true, expected false value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_is_null(value, err_level, msg, ...) utest_assert((value) == NULL, err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is not NULL, expected NULL value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_not_null(value, err_level, msg, ...) utest_assert((value) != NULL, err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: value " #value " is NULL, expected Non-NULL value)-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_equal(a, b, err_level, msg, ...) utest_assert((a) == (b), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: " #a " not equal to " #b ")-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_not_equal(a, b, err_level, msg, ...) utest_assert((a) != (b), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: " #a " equal to " #b ")-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_equal_ptr(a, b, err_level, msg, ...) utest_assert((void *)(a) == (void *)(b), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: " #a " not equal to " #b ")-" \
    "(user: " msg ")", ##__VA_ARGS__)

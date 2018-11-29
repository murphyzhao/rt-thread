/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-19     MurphyZhao   the first version
 */

#ifndef __UTEST_ASSERT_H__
#define __UTEST_ASSERT_H__

#include "utest.h"
#include <rtthread.h>

void utest_assert(int value, const char *file, int line, const char *func, const char *msg);

#define utest_assert_true(value, ...) utest_assert(value, \
    __FILE__, __LINE__, __func__, "(" #value") is false!")

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

#define utest_assert_is_equal(a, b, err_level, msg, ...) utest_assert((a) == (b), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: " #a " not equal to " #b ")-" \
    "(user: " msg ")", ##__VA_ARGS__)

#define utest_assert_not_equal(a, b, err_level, msg, ...) utest_assert((a) != (b), err_level, \
    __FILE__, __LINE__, __func__, \
    "(default: " #a " equal to " #b ")-" \
    "(user: " msg ")", ##__VA_ARGS__)

#endif /* __UTEST_ASSERT_H__ */

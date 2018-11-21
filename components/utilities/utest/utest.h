/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-19     MurphyZhao   the first version
 */

#ifndef __UTEST_H__
#define __UTEST_H__

#include <rtthread.h>

#define UTEST_SW_VERSION                  "0.0.1"

struct utest_unit {
    const char *name;
    void (*test)(void);
    void (*setup)(void);
    void (*teardown)(void);
    rt_uint32_t ops;
};
typedef struct utest_unit *utest_unit_t;

struct utest_suite {
    const char *name;
    int (*init)(void);
    int (*cleanup)(void);
    const struct utest_unit *unit;
    rt_uint32_t unit_size;
};
typedef struct utest_suite *utest_suite_t;

struct utest_cmd {
    const char *name;
    void (*cmd_func)(void);
    const char *desc;
};
typedef struct utest_cmd *utest_cmd_t;

#define ENABLE_UTEST_ASSERT_VERBOSE (2)

#define utest_suite_init(suite_name, init, cleanup)                            \
    struct utest_suite _test_suite = {                                  \
        suite_name, init, cleanup, RT_NULL                                     \
    };

#define utest_register_to_suite(...)                                           \
    static const struct utest_unit _test_unit[] = {                            \
        __VA_ARGS__, { 0 }                                                     \
    };                                                                         \
    _test_suite.unit = _test_unit;                                             \
    _test_suite.unit_size = sizeof(_test_unit)/sizeof(struct utest_unit);      \
    rt_kprintf("unit size: %d\n", _test_suite.unit_size);

#define utest_unit_setup_teardown(fn, setup, teardown) {                       \
    #fn, fn, setup, teardown, 0                                                \
}

#define UTEST_SUITE_EXPORT(test_main, suite_name, desc)                                            \
    static void utest_cmd_##test_main(void)                                    \
    {                                                                          \
        test_main();                                                           \
    }                                                                          \
    RT_USED static const struct utest_cmd _utest_cmd_##test_main               \
    SECTION("UtestCmdTab") =                                                   \
    {                                                                          \
        suite_name,                                                            \
        utest_cmd_##test_main,                                                 \
        #desc                                                                  \
    }

#define utest_unit_add(fn)                                                     \
    utest_unit_setup_teardown(fn, RT_NULL, RT_NULL)

int utest_init(void);
void _utest_suite_run(struct utest_suite *suite);

#define utest_suite_run()                                                      \
    _utest_suite_run((struct utest_suite *)&_test_suite)

#endif

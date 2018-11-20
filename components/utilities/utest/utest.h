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

struct utest_unit {
    const char *name;
    void (*test)(void);
    void (*setup)(void);
    void (*teardown)(void);
    rt_uint32_t ops;
};
typedef struct utest_unit *utest_unit_t;

struct utest_suite {
    rt_slist_t list;
    const char *name;
    int (*init)(void);
    int (*cleanup)(void);
    struct utest_unit *unit;
    rt_uint32_t unit_size;
};
typedef struct utest_suite *utest_suite_t;

#define ENABLE_UTEST_ASSERT_VERBOSE (2)
// rt_err_t utest_unit_add();

#define utest_suite_init(name, init, cleanup) \
        static struct utest_suite _test_suite = { \
        {RT_NULL}, name, init, cleanup, RT_NULL \
    }

#define utest_register_to_suite(...) \
        static struct utest_unit _test_unit[] = { \
        __VA_ARGS__, { 0 } \
    }; \
    _test_suite.unit = _test_unit; \
    _test_suite.unit_size = sizeof(_test_unit)/sizeof(struct utest_unit); \
    rt_kprintf("unit size: %d\n", _test_suite.unit_size);

#define utest_unit_setup_teardown(fn, setup, teardown) { \
    #fn, fn, setup, teardown, 0 \
}

#define UTEST_INIT_APP_EXPORT(name, fn) \
    static int utest_autoinit_##name(void) \
    { \
        fn(); \
        return 0; \
    } \
    INIT_APP_EXPORT(utest_autoinit_##name);

#define utest_unit_add(fn) \
    utest_unit_setup_teardown(fn, RT_NULL, RT_NULL)

int utest_init(void);
void _utest_suite_run(struct utest_suite *suite);

#define utest_suite_run() \
    _utest_suite_run((struct utest_suite *)&_test_suite)

void utest_run(const char *suite_name);

#endif

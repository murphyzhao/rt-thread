#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_assert_first(void)
{
    utest_assert_true(1, UTEST_ERR_LEVEL_LOW, "not assert");
    utest_assert_true(0, UTEST_ERR_LEVEL_LOW, "assert 0");
    utest_assert_false(0, UTEST_ERR_LEVEL_LOW, "assert 1");
    utest_assert_false(1, UTEST_ERR_LEVEL_LOW, "assert 1");
}

static void test_assert_second(void)
{
    utest_assert_true(1, UTEST_ERR_LEVEL_LOW, "1 was false");
}

static void test_main(void)
{
    utest_suite_init("utest_self_test.utest_test", RT_NULL, RT_NULL);

    utest_register_to_suite(
        utest_unit_add(test_assert_first),
        utest_unit_add(test_assert_second)
    );

    utest_suite_run();
}
UTEST_SUITE_EXPORT(test_main, "utest_self_test.utest_test", test_main);

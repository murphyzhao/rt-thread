#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_assert_fail(void)
{
    utest_assert_true(0);
    utest_assert_false(1);

    utest_assert_null(!RT_NULL);
    utest_assert_not_null(RT_NULL);

    utest_assert_int_equal(1, 2);
    utest_assert_int_not_equal(1, 1);

    utest_assert_string_not_equal("Hello RT-Thread!", "Hello RT-Thread!");
    utest_assert_string_equal("Hello RT-Thread!", "Hello");

    utest_assert_in_range(0, 1024, 4096);
    utest_assert_not_in_range(2048, 1024, 4096);
}

static rt_err_t utest_tc_init(void)
{
    return RT_EOK;
}

static rt_err_t utest_tc_cleanup(void)
{
    return RT_EOK;
}

static void testcase(void)
{
    UTEST_UNIT_RUN(test_assert_fail);
}
UTEST_TC_EXPORT(testcase, "utest_self_test.self_test_fail.test_main", utest_tc_init, utest_tc_cleanup);

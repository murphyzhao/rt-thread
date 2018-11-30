#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_assert_fail(void)
{
    uassert_true(0);
    uassert_false(1);

    uassert_null(!RT_NULL);
    uassert_not_null(RT_NULL);

    uassert_int_equal(1, 2);
    uassert_int_not_equal(1, 1);

    uassert_str_not_equal("Hello RT-Thread!", "Hello RT-Thread!");
    uassert_str_equal("Hello RT-Thread!", "Hello");

    uassert_in_range(0, 1024, 4096);
    uassert_not_in_range(2048, 1024, 4096);
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
UTEST_TC_EXPORT(testcase, "utest_self_test.fail_test.fail_tc", utest_tc_init, utest_tc_cleanup);

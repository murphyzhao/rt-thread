#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_assert_first(void)
{
    utest_assert_is_null(NULL, UTEST_ERR_LEVEL_LOW, "NULL was not NULL");
    utest_assert_not_null("foo", UTEST_ERR_LEVEL_LOW, "\"foo\" was NULL");
}

static void test_assert_second(void)
{
    utest_assert_equal(1, 1, UTEST_ERR_LEVEL_LOW, "1 was not equal to 1");
    utest_assert_equal_ptr(NULL, NULL, UTEST_ERR_LEVEL_LOW, "NULL was not equal to NULL");
}

static void test_main(void)
{
    utest_suite_init("utest_self_test.utest_test2", RT_NULL, RT_NULL);

    utest_register_to_suite(
        utest_unit_add(test_assert_first),
        utest_unit_add(test_assert_second)
    );

    utest_suite_run();
}
UTEST_SUITE_EXPORT(test_main, "utest_self_test.utest_test2", test_main);

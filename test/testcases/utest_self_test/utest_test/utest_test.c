#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_assert_first(void)
{
    utest_assert_true(1, "not assert");
    utest_assert_true(0, "assert 0");
    utest_assert_false(0, "assert 1");
    utest_assert_false(1, "assert 1");

    utest_assert_is_null(NULL, "NULL was not NULL");
    utest_assert_not_null("foo", "\"foo\" was NULL");
    utest_assert_equal(1, 1, "1 was not equal to 1");
    utest_assert_equal_ptr(NULL, NULL, "NULL was not equal to NULL");
}

static void test_assert_second(void)
{
    utest_assert_true(1, "1 was false");
    utest_assert_false(0, "0 was true");
    utest_assert_is_null(NULL, "NULL was not NULL");
    utest_assert_not_null("foo", "\"foo\" was NULL");
    utest_assert_equal(1, 1, "1 was not equal to 1");
    utest_assert_equal_ptr(NULL, NULL, "NULL was not equal to NULL");
}

static void test_main(void)
{
    utest_suite_init("utest.utest_test", RT_NULL, RT_NULL);

    utest_register_to_suite(
        utest_unit_add(test_assert_first),
        utest_unit_add(test_assert_second)
    );

    utest_suite_run();
}

MSH_CMD_EXPORT_ALIAS(test_main, test_main_0, test_main);
UTEST_INIT_APP_EXPORT(utest_testmain, test_main);

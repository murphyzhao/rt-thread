#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

static void test_posix_open(void)
{
    // utest_assert_true(1, "1 was false");
}

static void test_main(void)
{
    utest_suite_init("components.posix_api.test_main", RT_NULL, RT_NULL);

    utest_register_to_suite(
        utest_unit_add(test_posix_open)
    );

    utest_suite_run();
}
UTEST_SUITE_EXPORT(test_main, "components.posix_api.test_main", test_main);

#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>

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
    TC_LOG_I("in testcase func...");
}
UTEST_TC_EXPORT(testcase, "components.filesystem.posix.posix_api_tc", utest_tc_init, utest_tc_cleanup, 30);

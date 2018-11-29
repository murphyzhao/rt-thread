#include "utest.h"
#include "utest_assert.h"
#include <finsh.h>
#include <rtthread.h>
#include "dfs_file.h"

// static struct dfs_mount_tbl test_tbl = {
//     .device_name = "sd0",
//     .path = "/",
//     .filesystemtype = "elm",
//     .rwflag = 0,
//     .data = RT_NULL
// };

static struct dfs_fd fd;
static const char write_buf[] = "Hello RT-Thread!";

static void test_dfs_mount(void)
{
    // rt_err_t rst = RT_EOK;
    // rst = dfs_mount("sd0", "/", "elm", 0, 0);
    // LOG_D("dfs mount rst: %d", rst);
	// utest_assert_true(rst == 0);
}

static void test_dfs_open(void)
{
    utest_assert_true(dfs_file_open(&fd, "/testfile.txt", O_CREAT | O_RDWR) == 0);
}

static rt_err_t test_write(void)
{
    rt_err_t rst = RT_EOK;

    if (dfs_file_lseek(&fd, 0) != 0)
    {
        rst = -RT_ERROR;
        LOG_D("lseek failed. %d", dfs_file_lseek(&fd, 0));
        return rst;
    }

    rst = dfs_file_write(&fd, write_buf, rt_strlen(write_buf));
    if (rst < 0)
    {
        LOG_D("dfs write failed! rst = %d", rst);
        rst = -RT_ERROR;
    }
    else if (rst < rt_strlen(write_buf))
    {
        rst = -RT_ERROR;
        LOG_D("Did not write all data! rst = %d", rst);
    }
    else
    {
        rst = RT_EOK;
    }
    return rst;
}

static void test_dfs_write(void)
{
    utest_assert_true(test_write() == RT_EOK);
}

static rt_err_t test_read(void)
{
    rt_err_t rst = RT_EOK;

    char read_buf[24];

    if (dfs_file_lseek(&fd, 0) != 0)
    {
        rst = -RT_ERROR;
        LOG_D("lseek failed. %d", dfs_file_lseek(&fd, 0));
        return rst;
    }

    rst = dfs_file_read(&fd, read_buf, rt_strlen(write_buf));
    if (rst < 0)
    {
        LOG_D("dfs read failed");
        rst = -RT_ERROR;
        return rst;
    }

    read_buf[rst] = '\0';

    if (rt_strcmp(write_buf, read_buf))
    {
        LOG_D("dfs read data err, readbuf:%s; rst:%d", read_buf, rst);
        rst = -RT_ERROR;
        return rst;
    }
    rst = RT_EOK;

    return rst;
}

static void test_dfs_read(void)
{
    utest_assert_true(test_read() == RT_EOK);
}

static void test_dfs_close(void)
{
    utest_assert_true(dfs_file_close(&fd) == 0);
}

static void test_dfs_mkdir(void)
{

}

static rt_err_t utest_init(void)
{
    return RT_EOK;
}

static rt_err_t utest_cleanup(void)
{
    return RT_EOK;
}

static void suite_runner(void)
{
    UTEST_UNIT_RUN(test_dfs_mount);
    UTEST_UNIT_RUN(test_dfs_open);
    UTEST_UNIT_RUN(test_dfs_write);
    UTEST_UNIT_RUN(test_dfs_read);
    UTEST_UNIT_RUN(test_dfs_close);
    UTEST_UNIT_RUN(test_dfs_mkdir);
}
UTEST_SUITE_EXPORT(suite_runner, "components.dfs_api.test_main", utest_init, utest_cleanup);

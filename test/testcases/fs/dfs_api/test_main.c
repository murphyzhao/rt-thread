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
    rt_err_t rst = RT_EOK;
    rst = dfs_mount("sd0", "/", "elm", 0, 0);
    LOG_D("dfs mount rst: %d", rst);
	utest_assert_true(rst == 0, UTEST_ERR_LEVEL_FATAL, "mount failed");
}

static void test_dfs_open(void)
{
    utest_assert_true(dfs_file_open(&fd, "/testfile.txt", O_CREAT | O_RDWR) == 0, UTEST_ERR_LEVEL_FATAL, "");
}

static rt_err_t test_write(void)
{
    rt_err_t rst = RT_EOK;

    utest_assert_true(dfs_file_lseek(&fd, 0) == 0, UTEST_ERR_LEVEL_LOW, "");
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
    utest_assert_true(test_write() == RT_EOK,
        UTEST_ERR_LEVEL_LOW, "dfs write failed");
}

static rt_err_t test_read(void)
{
    rt_err_t rst = RT_EOK;

    char read_buf[24];

    if (dfs_file_lseek(&fd, 0) != 0)
    {
        rst = -RT_ERROR;
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
        LOG_D("dfs read data err");
        rst = -RT_ERROR;
        return rst;
    }

    return rst;
}

static void test_dfs_read(void)
{
    utest_assert_true(test_read() == RT_EOK,
        UTEST_ERR_LEVEL_LOW, "dfs read failed");
}

static void test_dfs_close(void)
{
    utest_assert_true(dfs_file_close(&fd) == 0, UTEST_ERR_LEVEL_LOW, "");
}

static void test_dfs_mkdir(void)
{

}

static void test_main(void)
{
    utest_suite_init("components.dfs.test_main", RT_NULL, RT_NULL);

    utest_register_to_suite(
        utest_unit_add(test_dfs_mount),
        utest_unit_add(test_dfs_open),
        utest_unit_add(test_dfs_write),
        utest_unit_add(test_dfs_read),
        utest_unit_add(test_dfs_close),
        utest_unit_add(test_dfs_mkdir)
    );

    utest_suite_run();
}
UTEST_SUITE_EXPORT(test_main, "components.dfs_api.test_main", test_main);

/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-19     MurphyZhao   the first version
 */

#include "utest.h"
#include <rtthread.h>
#include <finsh.h>

#if RT_CONSOLEBUF_SIZE < 256
#error "RT_CONSOLEBUF_SIZE is less than 256!"
#endif

static utest_tc_export_t tc_table = RT_NULL;
static rt_size_t tc_num;
static struct utest local_utest = {UTEST_PASSED, 0, 0};

#if defined(__ICCARM__) || defined(__ICCRX__)         /* for IAR compiler */
#pragma section="UtestTcTab"
#endif

int utest_init(void)
{
    /* initialize the utest commands table.*/
#if defined(__CC_ARM)                                 /* ARM C Compiler */
    extern const int UtestTcTab$$Base;
    extern const int UtestTcTab$$Limit;
    tc_table = (utest_tc_export_t)&UtestTcTab$$Base;
    tc_num = (utest_tc_export_t)&UtestTcTab$$Limit - tc_table;
#elif defined (__ICCARM__) || defined(__ICCRX__)      /* for IAR Compiler */
    tc_table = (utest_tc_export_t)__section_begin("UtestTcTab");
    tc_num = (utest_tc_export_t)__section_end("UtestTcTab") - tc_table;
#elif defined (__GNUC__)                              /* for GCC Compiler */
    extern const int __rtatcmdtab_start;
    extern const int __rtatcmdtab_end;
    tc_table = (utest_tc_export_t)&__rtatcmdtab_start;
    tc_num = (utest_tc_export_t) &__rtatcmdtab_end - tc_table;
#endif /* defined(__CC_ARM) */

    LOG_D("[----------] total utest testcase num: (%d)", tc_num);
    return tc_num;
}
INIT_COMPONENT_EXPORT(utest_init);

static void utest_tc_list(void)
{
    rt_size_t i = 0;

    LOG_D("Commands list : ");

    for (i = 0; i < tc_num; i++)
    {
        LOG_D("%s", tc_table[i].name);
    }
}
MSH_CMD_EXPORT_ALIAS(utest_tc_list, utest_tc_list, output all utest testcase);

static char *file_basename(const char *file)
{
    char *ptr = RT_NULL;
    char *rst = RT_NULL;
    char *file_bak = rt_strdup(file);
    uint8_t len = 0;
    if ((ptr = strrchr(file_bak, '\\')) != RT_NULL || (ptr = strrchr(file_bak, '/')) != RT_NULL)
    {
        rst = ptr;
    }
    else
    {
        rst = file_bak;
    }

    len = rst - file_bak;
    if (rst != file)
    {
        file_bak[len] = '\0';

        if ((ptr = strrchr(file_bak, '\\')) != RT_NULL || (ptr = strrchr(file_bak, '/')) != RT_NULL)
        {
            rst = ptr;
        }
        else
        {
            rst = file_bak;
        }
        len = rst - file_bak;
    }

    rt_free(file_bak);
    len = len != 0? len + 1 : len;
    return (char *)(file + len);
}

static void utest_run(const char *utest_name)
{
    rt_size_t i = 0;

    LOG_D("[----------] total utest testcase num: (%d)", tc_num);

    LOG_I("[==========] utest started");
    while(i < tc_num)
    {
        if (utest_name && rt_strcmp(utest_name, tc_table[i].name))
        {
            i++;
            continue;
        }
        LOG_I("[==========] testcase (%s) started", tc_table[i].name);
        tc_table[i].tc();
        if (local_utest.failed_num == 0)
        {
            LOG_I("[  PASSED  ] testcase (%s) test finished", tc_table[i].name);
        }
        else
        {
            LOG_I("[  FAILED  ] testcase (%s) test finished", tc_table[i].name);
        }
        i++;
    }
    LOG_I("[==========] utest finished");
}

static void utest_testcase_run(int argc, char** argv)
{
    char utest_name[UTEST_NAME_MAX_LEN];

    if (argc == 1)
    {
        utest_run(RT_NULL);
    }
    else if (argc == 2)
    {
        rt_memset(utest_name, 0x0, sizeof(utest_name));
        rt_strncpy(utest_name, argv[1], sizeof(utest_name) -1);
        utest_run(utest_name);
    }
    else
    {
        LOG_E("[$$$$$$$$$$] [run] error at (%s:%d)", __func__, __LINE__);
    }
}
MSH_CMD_EXPORT_ALIAS(utest_testcase_run, utest_run, utest_run [testcase name]);

utest_t utest_handle_get(void)
{
    return (utest_t)&local_utest;
}

void utest_unit_run(test_unit_func func, const char *unit_func_name)
{
    LOG_I("[==========] utest unit name: (%s)", unit_func_name);
    local_utest.error = UTEST_PASSED;
    local_utest.passed_num = 0;
    local_utest.failed_num = 0;
    func();
}

void utest_assert(int value, const char *file, int line, const char *func, const char *msg)
{
    if (!(value))
    {
        local_utest.error = UTEST_FAILED;
        local_utest.failed_num ++;
        LOG_E("[  ASSERT  ] at (%s); func: (%s:%d); msg: (%s)", file_basename(file), func, line, msg);
    }
    else
    {
        LOG_D("[  PASSED  ] (%s:%d) is passed", func, line);
        local_utest.error = UTEST_PASSED;
        local_utest.passed_num ++;
    }
}

void utest_assert_string(const char *a, const char *b, rt_bool_t equal, const char *file, int line, const char *func, const char *msg)
{
    if (a == RT_NULL || b == RT_NULL)
    {
        utest_assert(0, file, line, func, msg);
    }

    if (equal)
    {
        if (rt_strcmp(a, b) == 0)
        {
            utest_assert(1, file, line, func, msg);
        }
        else
        {
            utest_assert(0, file, line, func, msg);
        }
    }
    else
    {
        if (rt_strcmp(a, b) == 0)
        {
            utest_assert(0, file, line, func, msg);
        }
        else
        {
            utest_assert(1, file, line, func, msg);
        }
    }
}

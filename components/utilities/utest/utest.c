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

static utest_suite_export_t cmd_table = RT_NULL;
static rt_size_t cmd_num;
static struct utest local_utest = {UTEST_PASSED};

#if defined(__ICCARM__) || defined(__ICCRX__)         /* for IAR compiler */
#pragma section="UtestCmdTab"
#endif

int utest_init(void)
{
    /* initialize the utest commands table.*/
#if defined(__CC_ARM)                                 /* ARM C Compiler */
    extern const int UtestCmdTab$$Base;
    extern const int UtestCmdTab$$Limit;
    cmd_table = (utest_suite_export_t)&UtestCmdTab$$Base;
    cmd_num = (utest_suite_export_t)&UtestCmdTab$$Limit - cmd_table;
#elif defined (__ICCARM__) || defined(__ICCRX__)      /* for IAR Compiler */
    cmd_table = (utest_cmd_t)__section_begin("UtestCmdTab");
    cmd_num = (utest_suite_export_t)__section_end("UtestCmdTab") - cmd_table;
#elif defined (__GNUC__)                              /* for GCC Compiler */
    extern const int __rtatcmdtab_start;
    extern const int __rtatcmdtab_end;
    cmd_table = (utest_suite_export_t)&__rtatcmdtab_start;
    cmd_num = (utest_suite_export_t) &__rtatcmdtab_end - cmd_table;
#endif /* defined(__CC_ARM) */

    LOG_D("[----------] total utest suite cmd: (%d)", cmd_num);
    return cmd_num;
}
INIT_COMPONENT_EXPORT(utest_init);

static void utest_cmd_list(void)
{
    rt_size_t i = 0;

    LOG_D("Commands list : ");

    for (i = 0; i < cmd_num; i++)
    {
        LOG_D("%s", cmd_table[i].name);
    }
}
MSH_CMD_EXPORT_ALIAS(utest_cmd_list, utest_cmd_list, output all utest cmd);

static char *file_basename(const char *file)
{
    char *ptr = RT_NULL;
    if ((ptr = strrchr(file, '\\')) != RT_NULL )
    {
        return ptr + 1;
    }
    else if ((ptr = strrchr(file, '/')) != RT_NULL )
    {
        return ptr + 1;
    }
    return ptr;
}

static void utest_run(const char *utest_name)
{
    rt_size_t i = 0;

    LOG_D("[----------] total utest suite num: (%d)", cmd_num);

    LOG_I("[==========] utest run started");
    while(i < cmd_num)
    {
        if (utest_name && rt_strcmp(utest_name, cmd_table[i].name))
        {
            i++;
            continue;
        }
        cmd_table[i].runner();
        i++;
    }
    LOG_I("[==========] utest run finished");
}

static void cmd_utest_run(int argc, char** argv)
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
MSH_CMD_EXPORT_ALIAS(cmd_utest_run, utest_run, utest_run [suite_name]);

utest_t utest_handle_get(void)
{
    return (utest_t)&local_utest;
}


void utest_unit_run(test_unit_func func, const char *unit_func_name)
{
    LOG_I("[==========] utest unit name: (%s)", unit_func_name);
    local_utest.error = UTEST_PASSED;
    func();
}

void utest_assert(int value, const char *file, int line, const char *func, const char *msg)
{
    if (!(value))
    {
        local_utest.error = UTEST_FAILED;
        LOG_E("[  FAILED  ] (assert) at %s (%s:%d); [msg] %s", file_basename(file), func, line, msg);
    }
    else
    {
        LOG_D("[  PASSED  ] (%s) is passed", func);
        local_utest.error = UTEST_PASSED;
    }
}

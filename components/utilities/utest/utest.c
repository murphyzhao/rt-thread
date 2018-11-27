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

/**
 * 0: UTEST_ERR_PASS; 1: UTEST_ERR_SKIP; 2: UTEST_ERR_FAIL; 3: UTEST_ERR_FATAL
*/
struct utest
{
    rt_int32_t err_level;
};
typedef struct utest *utest_t;

static struct utest local_utest = {0};

void _utest_suite_run(struct utest_suite *suite)
{
    local_utest.err_level = UTEST_ERR_LEVEL_LOW;
    LOG_I("[==========] utest suite name: (%s)", suite->name);
    if (suite->unit != RT_NULL)
    {
        rt_uint32_t i = 0;

        LOG_D("[----------] utest unit start");
        while(i < suite->unit_size)
        {
            // local_utest.err_level = UTEST_ERR_LEVEL_LOW;
            if (suite->unit[i].name == RT_NULL)
            {
                LOG_D("[----------] utest unit end");
                i++;
                continue;
            }
            LOG_I("[==========] utest unit name: (%s)",
                suite->unit[i].name);

            if (local_utest.err_level == UTEST_ERR_LEVEL_FATAL)
            {
                LOG_I("[   SKIP   ] utest unit name: (%s) skipped",
                    suite->unit[i].name);
                i++;
                continue;
            }

            if (suite->unit[i].setup)
            {
                suite->unit[i].setup();
            }

            if (suite->unit[i].test)
            {
                suite->unit[i].test();
            }

            if (suite->unit[i].teardown)
            {
                suite->unit[i].teardown();
            }

            i++;
        }
    }
    else
    {
        LOG_I("[==========] suite (%s) no unit exist", suite->name);
    }
}

static utest_cmd_t cmd_table = RT_NULL;
static rt_size_t cmd_num;

#if defined(__ICCARM__) || defined(__ICCRX__)         /* for IAR compiler */
#pragma section="UtestCmdTab"
#endif

int utest_init(void)
{
    /* initialize the utest commands table.*/
#if defined(__CC_ARM)                                 /* ARM C Compiler */
    extern const int UtestCmdTab$$Base;
    extern const int UtestCmdTab$$Limit;
    cmd_table = (utest_cmd_t)&UtestCmdTab$$Base;
    cmd_num = (utest_cmd_t)&UtestCmdTab$$Limit - cmd_table;
#elif defined (__ICCARM__) || defined(__ICCRX__)      /* for IAR Compiler */
    cmd_table = (utest_cmd_t)__section_begin("UtestCmdTab");
    cmd_num = (utest_cmd_t)__section_end("UtestCmdTab") - cmd_table;
#elif defined (__GNUC__)                              /* for GCC Compiler */
    extern const int __rtatcmdtab_start;
    extern const int __rtatcmdtab_end;
    cmd_table = (utest_cmd_t)&__rtatcmdtab_start;
    cmd_num = (utest_cmd_t) &__rtatcmdtab_end - cmd_table;
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

static void utest_run(const char *suite_name)
{
    rt_size_t i = 0;

    LOG_D("[----------] total utest suite num: (%d)", cmd_num);

    LOG_I("[==========] utest run started");
    while(i < cmd_num)
    {
        if (suite_name && rt_strcmp(suite_name, cmd_table[i].name))
        {
            i++;
            continue;
        }
        cmd_table[i].cmd_func();
        i++;
    }
    LOG_I("[==========] utest run finished");
}

static void cmd_utest_run(int argc, char** argv)
{
    char suite_name[128];

    if (argc == 1)
    {
        utest_run(RT_NULL);
    }
    else if (argc == 2)
    {
        rt_memset(suite_name, 0x0, sizeof(suite_name));
        rt_memcpy(suite_name, argv[1], strlen(argv[1]));
        utest_run(suite_name);
    }
    else
    {
        LOG_E("[$$$$$$$$$$] [run] error at (%s:%d)", __func__, __LINE__);
    }
}
MSH_CMD_EXPORT_ALIAS(cmd_utest_run, utest_run, utest_run [suite_name]);

void utest_fail(void)
{
    return;
}

void _utest_assert(int cond, int32_t err_level, const char *file, int line, const char *msg)
{
    if (!(cond))
    {
        local_utest.err_level = err_level;
        LOG_E("[  FAILED  ] [assert] at %s:%d; [msg] %s", file, line, msg);
        utest_fail();
    }
#if ENABLE_UTEST_ASSERT_VERBOSE == 2
    else
    {
        LOG_D("[  PASSED  ] ASSERT SUCC line:%d", line);
    }
#endif
}

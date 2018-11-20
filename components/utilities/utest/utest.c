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
#include <rtdevice.h>
#include <rthw.h>
#include <finsh.h>

#define UTEST_DEBUG

#undef DBG_SECTION_NAME
#undef DBG_LEVEL
#undef DBG_COLOR
#undef DBG_ENABLE

#define DBG_ENABLE
#define DBG_SECTION_NAME          "utest"
#ifdef UTEST_DEBUG
#define DBG_LEVEL                 DBG_LOG
#else
#define DBG_LEVEL                 DBG_INFO
#endif
#define DBG_COLOR
#include <rtdbg.h>

static rt_slist_t utest_suite_list;

void _utest_suite_run(struct utest_suite *suite)
{
    rt_base_t level;
    level = rt_hw_interrupt_disable();
    rt_slist_append(&utest_suite_list, &suite->list);
    rt_hw_interrupt_enable(level);
}

int utest_init(void)
{
    rt_slist_init(&utest_suite_list);
    return 0;
}
INIT_COMPONENT_EXPORT(utest_init);

void utest_run(void)
{
    rt_base_t level;
    struct utest_suite *suite;
    rt_slist_t *curr_list = &utest_suite_list;
    if (rt_slist_isempty(curr_list))
    {
        LOG_I("========== no testsuite exist ==========");
        return;
    }

    LOG_I("========== utest run started ==========");
    while(curr_list->next != RT_NULL)
    {
        level = rt_hw_interrupt_disable();
        suite = rt_slist_entry(curr_list->next, struct utest_suite, list);
        curr_list = curr_list->next;
        rt_hw_interrupt_enable(level);
        
        LOG_D("---------- utest suite name: (%s) ----------", suite->name);
        if (suite->unit != RT_NULL)
        {
            rt_uint32_t i = 0;

            while(i < suite->unit_size)
            {
                if (suite->unit[i].name == RT_NULL)
                {
                    LOG_D("---------- utest unit end ----------");
                    break;
                }
                LOG_D("---------- utest unit name: (%s) ----------",
                    suite->unit[i].name);
                i++;
            }
        }
        else
        {
            LOG_D("---------- suite->unit is NULL ----------");
        }
    }
    LOG_I("========== utest run finished ==========");

}

MSH_CMD_EXPORT(utest_run, utest_run);

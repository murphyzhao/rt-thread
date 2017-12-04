/*
 * File      : rtgui_demo.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2008 - 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-8      Tangyuxin    first version
 */

#include <rtthread.h>
#include "jerry-api.h"
#include "jerry-port.h"

#ifdef RT_USING_GUIENGINE

extern char* read_sources(const char *script_file_names[], int files_count, size_t *out_source_size_p);
extern void gui_js_mainwin(void);
static void gui_js_entry(void *parameter)
{
    

    /* Initialize engine */
    // jerry_init (JERRY_INIT_EMPTY);



    gui_js_mainwin();
}

int gui_js_main(void)
{
	rt_thread_t tid;
    rt_device_t device;
    rt_err_t  err;
    
    device = rt_device_find("lcd");
    if(device == RT_NULL)
    {
        rt_kprintf("Not found LCD driver\n");
        return RT_ERROR;
    }
    
    err = rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
    if (err != RT_EOK)
    {
        rt_kprintf("Open LCD driver fail\n");
        return RT_ERROR;
    }
    
    /* set graphic device */
    rtgui_graphic_set_device(device);
    
	tid = rt_thread_create("gui_js_demo", gui_js_entry, RT_NULL, 1024 * 32, 20, 100);
	if (tid)
	{
		rt_thread_startup(tid);
	}

	return 0;
}
INIT_APP_EXPORT(gui_js_main);
#endif /* RT_USING_GUIENGINE */

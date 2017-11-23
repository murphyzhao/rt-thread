/*
* File      : persimmon_demo.c
* This file is part of RT-Thread RTOS
* COPYRIGHT (C) 2009-2016 RT-Thread Develop Team
*/
#include <rtthread.h>

extern void ui_main(void);
static void demo_ui_thread_entry(void* parameter)
{
    rt_kprintf("gui runing...\n");

    ui_main();
}

int persimmon_ui_demo(void)
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
    
	tid = rt_thread_create("demo", demo_ui_thread_entry, RT_NULL, 1024 * 32, 20, 100);
	if (tid)
	{
		rt_thread_startup(tid);
	}

	return 0;
}

INIT_APP_EXPORT(persimmon_ui_demo);
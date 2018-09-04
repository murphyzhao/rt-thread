/*
 *  RT-Thread Wi-Fi Device
 *
 * COPYRIGHT (C) 2014 - 2018, Shanghai Real-Thread Technology Co., Ltd
 *
 *  This file is part of RT-Thread (http://www.rt-thread.org)
 *
 *  All rights reserved.
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
 * 2018-08-19     RT-Thread    the first verion
 */

#ifndef __WLAN_WORKQUEUE_H__
#define __WLAN_WORKQUEUE_H__

#include <ipc/workqueue.h>

#ifndef RT_WLAN_WORKQUEUE_THREAD_NAME
#define RT_WLAN_WORKQUEUE_THREAD_NAME  ("wlan_job")
#endif

#ifndef RT_WLAN_WORKQUEUE_THREAD_SIZE
#define RT_WLAN_WORKQUEUE_THREAD_SIZE  (2048)
#endif

#ifndef RT_WLAN_WORKQUEUE_THREAD_PRIO
#define RT_WLAN_WORKQUEUE_THREAD_PRIO  (20)
#endif

int rt_wlan_workqueue_init(void);

rt_err_t rt_wlan_workqueue_dowork(void (*func)(void *parameter), void *parameter);

struct rt_workqueue *rt_wlan_get_workqueue(void);

#endif

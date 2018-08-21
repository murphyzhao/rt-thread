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
 * 2018-08-14     RT-Thread    the first verion
 */

#ifndef __WLAN_PROT_H__
#define __WLAN_PROT_H__

#define WLAN_PROT_NAME_LEN  (8)

typedef enum
{
    RT_WLAN_PROT_LWIP = 0,
    RT_WLAN_PROT_MAX
}rt_wlan_prot_type_t;

typedef enum
{
    RT_WLAN_PROT_EVT_INIT_DONE = 0,
    RT_WLAN_PROT_EVT_CONNECT,
    RT_WLAN_PROT_EVT_DISCONNECT,
    RT_WLAN_PROT_EVT_AP_START,
    RT_WLAN_PROT_EVT_AP_STOP,
    RT_WLAN_PROT_EVT_AP_ASSOCIATED,
    RT_WLAN_PROT_EVT_AP_DISASSOCIATED,
    RT_WLAN_PROT_EVT_MAX,
} rt_wlan_prot_event_t;

struct rt_wlan_prot;
struct rt_wlan_prot_ops
{
    rt_err_t (*protocol_recv)(struct rt_wlan_device *wlan, void *buff, int len);
    struct rt_wlan_prot * (*dev_reg_callback)(struct rt_wlan_prot *prot, struct rt_wlan_device *wlan);
    void (*dev_unreg_callback)(struct rt_wlan_prot *prot, struct rt_wlan_device *wlan);
};

struct rt_wlan_prot
{
    char name[WLAN_PROT_NAME_LEN];
    rt_wlan_prot_type_t type;
    const struct rt_wlan_prot_ops *ops;
};

typedef void (*rt_wlan_prot_event_handler)(struct rt_wlan_prot *port, struct rt_wlan_device *wlan, int event);

rt_err_t rt_wlan_prot_attach(const char *name, rt_wlan_prot_type_t type);

rt_err_t rt_wlan_prot_attach_dev(struct rt_wlan_device *wlan, rt_wlan_prot_type_t type);

rt_err_t rt_wlan_prot_detach(const char *name);

rt_err_t rt_wlan_prot_detach_dev(struct rt_wlan_device *wlan);

rt_err_t rt_wlan_prot_regisetr(struct rt_wlan_prot *prot);

int rt_wlan_prot_transfer_dev(struct rt_wlan_device *wlan, void *buff, int len);

int rt_wlan_dev_transfer_prot(struct rt_wlan_device *wlan, void *buff, int len);

rt_err_t rt_wlan_prot_event_register(struct rt_wlan_prot *prot, rt_wlan_prot_event_t event, rt_wlan_prot_event_handler handler);

rt_err_t rt_wlan_prot_event_unregister(struct rt_wlan_prot *prot, rt_wlan_prot_event_t event);

void rt_wlan_prot_dump(void);
#endif

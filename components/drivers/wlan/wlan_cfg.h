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
 * 2018-08-03     RT-Thread    the first verion
 */

#ifndef __WLAN_CFG_H__
#define __WLAN_CFG_H__

#include <wlan_dev.h>

#define RT_WLAN_CFG_INFO_MAX    (5)

#define RT_WLAN_CFG_MAGIC       (0x426f6d62)

struct rt_wlan_cfg_info
{
    struct rt_wlan_info info;
    struct rt_wlan_key key;
};

typedef int (*rt_wlan_wr)(void *buff, int len);

struct rt_wlan_cfg_ops
{
    int (*read_cfg)(void *buff, int len);
    int (*get_len)(void);
    int (*write_cfg)(void *buff, int len);
};

void rt_wlan_cfg_init(void);
void rt_wlan_cfg_set_ops(const struct rt_wlan_cfg_ops *ops);
int rt_wlan_cfg_get_num(void);
int rt_wlan_cfg_read (struct rt_wlan_cfg_info *cfg_info, int num);
int rt_wlan_cfg_read_index(struct rt_wlan_cfg_info *cfg_info, int index);
rt_err_t rt_wlan_cfg_save (struct rt_wlan_cfg_info *cfg_info); /* auto save config to flash */
rt_err_t rt_wlan_cfg_cache_refresh(void);
rt_err_t rt_wlan_cfg_cache_save(void);
void rt_wlan_cfg_delete_all(void);
void rt_wlan_cfg_dump(void);
#endif

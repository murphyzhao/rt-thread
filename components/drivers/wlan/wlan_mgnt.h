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
 
#ifndef __WLAN_MGNT_H__
#define __WLAN_MGNT_H__

#include <wlan_dev.h>

#ifndef RT_WLAN_SCAN_WAIT_MS
#define RT_WLAN_SCAN_WAIT_MS       (10 * 1000)
#endif

#ifndef RT_WLAN_SCAN_CACHE_NUM
#define RT_WLAN_SCAN_CACHE_NUM     (50)
#endif

#ifndef RT_WLAN_CONNECT_WAIT_MS
#define RT_WLAN_CONNECT_WAIT_MS    (10 * 1000)
#endif

#ifndef RT_WLAN_EBOX_NUM
#define RT_WLAN_EBOX_NUM           (10)
#endif

#if RT_WLAN_EBOX_NUM < 1
#error "event box num Too little"
#endif

/*state fot station*/
#define RT_WLAN_STATE_CONNECT     (0x1 << 0)
#define RT_WLAN_STATE_CONNECTING  (0x1 << 1)
#define RT_WLAN_STATE_READY       (0x1 << 2)
#define RT_WLAN_STATE_POWERSAVE   (0x1 << 3)

/*flags fot station*/
#define RT_WLAN_STATE_AUTOEN      (0x1 << 0)

/*state fot ap*/
#define RT_WLAN_STATE_ACTIVE      (0x1 << 0)

typedef enum {
    RT_WLAN_EVT_READY = 0,              /* connect and prot is ok, You can send data*/
    RT_WLAN_EVT_SCAN_DONE,              /* Scan a info */
    RT_WLAN_EVT_SCAN_REPORT,            /* Scan end */
    RT_WLAN_EVT_STA_CONNECTED,          /* connect success */
    RT_WLAN_EVT_STA_CONNECTED_FAIL,     /* connection failed */
    RT_WLAN_EVT_STA_DISCONNECTED,       /* disconnect */
    RT_WLAN_EVT_AP_START,               /* AP start */
    RT_WLAN_EVT_AP_STOP,                /* AP stop */
    RT_WLAN_EVT_AP_ASSOCIATED,          /* sta associated */
    RT_WLAN_EVT_AP_DISASSOCIATED,       /* sta disassociated */
    RT_WLAN_EVT_MAX
} rt_wlan_event_t;

typedef void (*rt_wlan_event_handler)(int event, struct rt_wlan_buff *buff, void *parameter);

struct rt_wlan_scan_result
{
    rt_int32_t num;
    struct rt_wlan_info *info;
};

int rt_wlan_init(void);

rt_err_t rt_wlan_set_mode(const char *dev_name, rt_wlan_mode_t mode);

rt_wlan_mode_t rt_wlan_get_mode(const char *dev_name);

rt_err_t rt_wlan_connect(const char *ssid, const char *password);

rt_err_t rt_wlan_connect_adv(struct rt_wlan_info *info, const char *password);

rt_err_t rt_wlan_disconnect(void);

int rt_wlan_is_connected(void);

rt_err_t rt_wlan_set_mac(rt_uint8_t *mac);

rt_err_t rt_wlan_get_mac(rt_uint8_t *mac);

rt_err_t rt_wlan_get_info(struct rt_wlan_info *info);

int rt_wlan_get_rssi(void);

rt_err_t rt_wlan_start_ap(const char *ssid, const char *password);

rt_err_t rt_wlan_start_ap_adv(struct rt_wlan_info *info, const char *password);

int rt_wlan_ap_is_active(void);

rt_err_t rt_wlan_ap_stop(void);

rt_err_t rt_wlan_ap_get_info(struct rt_wlan_info *info);

int rt_wlan_ap_get_sta_num(void);

int rt_wlan_ap_get_sta_info(struct rt_wlan_info *info, int num);

rt_err_t rt_wlan_ap_deauth_sta(rt_uint8_t *mac);

rt_err_t rt_wlan_ap_set_country(rt_country_code_t country_code);

rt_country_code_t rt_wlan_ap_get_country(void);

void rt_wlan_config_autoreconnect(rt_bool_t enable);

rt_bool_t rt_wlan_get_autoreconnect_mode(void);

rt_err_t rt_wlan_scan(void);

struct rt_wlan_scan_result *rt_wlan_scan_sync(void);

struct rt_wlan_scan_result *rt_wlan_scan_with_info(struct rt_wlan_info *info);

int rt_wlan_scan_get_info_num(void);

int rt_wlan_scan_get_info(struct rt_wlan_info *info, int num);

struct rt_wlan_scan_result *rt_wlan_scan_get_result(void);

void rt_wlan_scan_result_clean(void);

int rt_wlan_scan_find_cache(struct rt_wlan_info *info, struct rt_wlan_info *out_info, int num);

rt_bool_t rt_wlan_find_best_by_cache(const char *ssid, struct rt_wlan_info *info);

rt_err_t rt_wlan_enable_powersave(void);

rt_err_t rt_wlan_disable_powersave(void);

rt_err_t rt_wlan_register_event_handler(rt_wlan_event_t event, rt_wlan_event_handler handler, void *parameter);

rt_err_t rt_wlan_unregister_event_handler(rt_wlan_event_t event);

void rt_wlan_mgnt_lock(void);

void rt_wlan_mgnt_unlock(void);
#endif

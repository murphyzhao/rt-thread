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

#include <rthw.h>
#include <rtthread.h>
#include <wlan_dev.h>
#include <wlan_prot.h>
#include <netif/ethernetif.h>
#include <lwip/netifapi.h>
#ifdef LWIP_USING_DHCPD
#include <dhcp_server.h>
#endif

#define DBG_ENABLE
#define DBG_LEVEL DBG_INFO
#define DBG_SECTION_NAME  "WLAN.lwip"
#define DBG_COLOR
#include <rtdbg.h>

struct lwip_prot_des
{
    struct rt_wlan_prot prot;
    struct eth_device eth;
    rt_int8_t connected_flag;
};

static void netif_set_connected(struct rt_wlan_device *wlan, int connected)
{
    struct lwip_prot_des *lwip_prot = (struct lwip_prot_des *)wlan->prot;
    struct eth_device *eth_dev = &lwip_prot->eth;

    lwip_prot->connected_flag = connected;

    if (connected)
    {
        if (wlan->mode == RT_WLAN_STATION)
        {
            LOG_D("F:%s L:%d dhcp start run", __FUNCTION__, __LINE__);
            netifapi_netif_set_link_up(eth_dev->netif);
            dhcp_start(eth_dev->netif);
        }
        else if (wlan->mode == RT_WLAN_AP)
        {
            LOG_D("F:%s L:%d dhcpd start run", __FUNCTION__, __LINE__);

            netifapi_netif_set_link_up(eth_dev->netif);
#ifdef LWIP_USING_DHCPD
            {
                char netif_name[8];
                int i;

                rt_memset(netif_name, 0, sizeof(netif_name));
                for (i = 0; i < sizeof(eth_dev->netif->name); i++)
                {
                    netif_name[i] = eth_dev->netif->name[i];
                }
                dhcpd_start(netif_name);
            }
#endif
        }     
    }
    else
    {
        if (wlan->mode == RT_WLAN_STATION)
        {
            LOG_D("F:%s L:%d dhcp stop run", __FUNCTION__, __LINE__);
            netifapi_netif_set_link_down(eth_dev->netif);
            dhcp_stop(eth_dev->netif);
        }
        else if (wlan->mode == RT_WLAN_AP)
        {
            LOG_D("F:%s L:%d dhcpd stop run", __FUNCTION__, __LINE__);
            netifapi_netif_set_link_down(eth_dev->netif);
        }
    }
}

static void rt_wlan_lwip_event_handle(struct rt_wlan_prot *port, struct rt_wlan_device *wlan, int event)
{
    switch(event)
    {
    case RT_WLAN_PROT_EVT_CONNECT:
    {
        LOG_D("event: CONNECT");
        netif_set_connected(wlan, 1);
        break;
    }
    case RT_WLAN_PROT_EVT_DISCONNECT:
    {
        LOG_D("event: DISCONNECT");
        netif_set_connected(wlan, 0);
        break;
    }
    case RT_WLAN_PROT_EVT_AP_START:
    {
        LOG_D("event: AP_START");
        netif_set_connected(wlan, 1);
        break;
    }
    case RT_WLAN_PROT_EVT_AP_STOP:
    {
        LOG_D("event: AP_STOP");
        netif_set_connected(wlan, 0);
        break;
    }
    case RT_WLAN_PROT_EVT_AP_ASSOCIATED:
    {
        LOG_D("event: ASSOCIATED");
        break;
    }
    case RT_WLAN_PROT_EVT_AP_DISASSOCIATED:
    {
        LOG_D("event: DISASSOCIATED");
        break;
    }
    default :
    {
        LOG_D("event: UNKNOWN");
        break;
    }
    }
}

static rt_err_t rt_wlan_lwip_protocol_control(rt_device_t device, int cmd, void *args)
{
    struct eth_device *eth_dev = (struct eth_device *)device;
    struct rt_wlan_device *wlan;
    rt_err_t err = RT_EOK;

    RT_ASSERT(eth_dev != RT_NULL);

    LOG_D("F:%s L:%d device:0x%08x user_data:0x%08x", __FUNCTION__, __LINE__, eth_dev, eth_dev->parent.user_data);

    switch(cmd)
    {
    case NIOCTL_GADDR:
        /* get MAC address */
        wlan = eth_dev->parent.user_data;
        err = rt_device_control((rt_device_t)wlan, RT_WLAN_CMD_GET_MAC, args);
        break;
    default :
        break;
    }
    return err;
}

static rt_err_t rt_wlan_lwip_protocol_recv(struct rt_wlan_device *wlan, void *buff, int len)
{
    struct eth_device *eth_dev = &((struct lwip_prot_des *)wlan->prot)->eth;

    LOG_D("F:%s L:%d run", __FUNCTION__, __LINE__);

    if (eth_dev)
    {
        struct pbuf *p;
        int count = 0;

        while ((p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM)) == RT_NULL)
        {
            LOG_W("F:%s L:%d wait for pbuf_alloc!", __FUNCTION__, __LINE__);
            rt_thread_delay(1);
            count++;

            //wait for 10ms or give up!!
            if (count >= 10)
            {
                LOG_E("F:%s L:%d pbuf allocate fail!!!", __FUNCTION__, __LINE__);
                return -RT_ENOMEM;
            }
        }

        /*copy data dat -> pbuf*/
        pbuf_take(p, buff, len);

        if ((eth_dev->netif->input(p, eth_dev->netif)) != ERR_OK)
        {
            LOG_E("F:%s L:%d IP input error", __FUNCTION__, __LINE__);
            pbuf_free(p);
            p = RT_NULL;
        }
        LOG_D("F:%s L:%d netif iput success! len:%d", __FUNCTION__, __LINE__, len);
    }

    return RT_EOK;
}

static rt_err_t rt_wlan_lwip_protocol_send(rt_device_t device, struct pbuf* p)
{
    struct rt_wlan_device *wlan = ((struct eth_device *)device)->parent.user_data;
    rt_uint8_t *frame;

    LOG_D("F:%s L:%d run", __FUNCTION__, __LINE__);

    if (wlan)
    {
        /* sending data directly */
        if (p->len == p->tot_len)
        {
            frame = (rt_uint8_t *)p->payload;
            rt_wlan_prot_transfer_dev(wlan, frame, p->tot_len);
            LOG_D("F:%s L:%d run len:%d", __FUNCTION__, __LINE__, p->tot_len);
            return RT_EOK;
        }
        frame = rt_malloc(p->tot_len);
        if (frame == RT_NULL)
        {
            LOG_E("F:%s L:%d malloc out_buf fail\n", __FUNCTION__, __LINE__);
            return -RT_ENOMEM;
        }
        /*copy pbuf -> data dat*/
        pbuf_copy_partial(p, frame, p->tot_len, 0);
        /* send data */
        rt_wlan_prot_transfer_dev(wlan, frame, p->tot_len);
        LOG_D("F:%s L:%d run len:%d", __FUNCTION__, __LINE__, p->tot_len);
        rt_free(frame);
    }

    return RT_EOK;
}

static struct rt_wlan_prot *rt_wlan_lwip_protocol_register(struct rt_wlan_prot *prot, struct rt_wlan_device *wlan)
{
    struct eth_device *eth = RT_NULL;
    static rt_uint8_t id = 0;
    char name[4];
    rt_device_t device = RT_NULL;
    struct lwip_prot_des *lwip_prot;

    if (wlan == RT_NULL || prot == RT_NULL)
        return RT_NULL;;

    LOG_D("F:%s L:%d is run wlan:0x%08x", __FUNCTION__, __LINE__, wlan);

    do
    {
        /* find ETH device name */
        name[0] = 'w';
        name[1] = '0' + id++;
        name[2] = '\0';
        device = rt_device_find(name);
    } while (device);

    if (id > 9)
    {
        LOG_E("F:%s L:%d not find Empty name", __FUNCTION__, __LINE__, name);
        return RT_NULL;
    }

    if (rt_device_open((rt_device_t)wlan, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
    {
        LOG_E("F:%s L:%d open wlan failed", __FUNCTION__, __LINE__);
        return RT_NULL;
    }

    lwip_prot = rt_malloc(sizeof(struct lwip_prot_des));
    if (lwip_prot == RT_NULL)
    {
        LOG_E("F:%s L:%d malloc mem failed", __FUNCTION__, __LINE__);
        rt_device_close((rt_device_t)wlan);
        return RT_NULL;
    }
    rt_memset(lwip_prot, 0, sizeof(struct lwip_prot_des));

    eth = &lwip_prot->eth;
    eth->parent.init       = RT_NULL;
    eth->parent.open       = RT_NULL;
    eth->parent.close      = RT_NULL;
    eth->parent.read       = RT_NULL;
    eth->parent.write      = RT_NULL;
    eth->parent.control    = rt_wlan_lwip_protocol_control;
    eth->parent.user_data  = wlan;
    eth->eth_rx     = RT_NULL;
    eth->eth_tx     = rt_wlan_lwip_protocol_send;

    /* register ETH device */
    if (eth_device_init(eth, name) != RT_EOK)
    {
        LOG_E("eth device init failed");
        rt_device_close((rt_device_t)wlan);
        rt_free(lwip_prot);
        return RT_NULL;
    }
    rt_memcpy(&lwip_prot->prot, prot, sizeof(struct rt_wlan_prot));

    LOG_I("eth device init ok name:%s", name);

    return &lwip_prot->prot;
}

static void rt_wlan_lwip_protocol_unregister(struct rt_wlan_prot *prot, struct rt_wlan_device *wlan)
{
    /*TODO*/
    LOG_D("F:%s L:%d is run wlan:0x%08x", __FUNCTION__, __LINE__, wlan);
}

static struct rt_wlan_prot_ops ops = 
{
    rt_wlan_lwip_protocol_recv,
    rt_wlan_lwip_protocol_register,
    rt_wlan_lwip_protocol_unregister
};

int rt_wlan_lwip_init(void)
{
    static struct rt_wlan_prot prot;
    rt_wlan_prot_event_t event;

    rt_memset(&prot, 0, sizeof(prot));
    rt_strncpy(&prot.name[0], "lwip", RT_WLAN_PROT_NAME_LEN);
    prot.type = RT_WLAN_PROT_LWIP;
    prot.ops = &ops;

    if (rt_wlan_prot_regisetr(&prot) != RT_EOK)
    {
        LOG_E("F:%s L:%d protocol regisetr failed", __FUNCTION__, __LINE__);
        return -1;
    }

    for (event = RT_WLAN_PROT_EVT_INIT_DONE; event < RT_WLAN_PROT_EVT_MAX; event++)
    {
        rt_wlan_prot_event_register(&prot, event, rt_wlan_lwip_event_handle);
    }

    return 0;
}
INIT_PREV_EXPORT(rt_wlan_lwip_init);

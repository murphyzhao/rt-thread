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

#define DBG_ENABLE
#define DBG_LEVEL DBG_INFO
#define DBG_SECTION_NAME  "WLAN.prot"
#define DBG_COLOR
#include <rtdbg.h>

struct rt_wlan_prot_event_des
{
    rt_wlan_prot_event_handler handler;
    struct rt_wlan_prot *prot;
};

static struct rt_wlan_prot *_prot[RT_WLAN_PROT_MAX];

static struct rt_wlan_prot_event_des prot_event_tab[RT_WLAN_PROT_EVT_MAX][RT_WLAN_PROT_MAX];

static void rt_wlan_prot_event_handle(struct rt_wlan_device *wlan, rt_wlan_dev_event_t event, struct rt_wlan_buff *buff, void *parameter)
{
    int i;
    struct rt_wlan_prot *wlan_prot;
    struct rt_wlan_prot *prot;
    rt_wlan_prot_event_handler handler;
    rt_wlan_prot_event_t prot_event;

    LOG_D("F:%s L:%d event:%d", __FUNCTION__, __LINE__, event);

    wlan_prot = wlan->prot;
    handler = RT_NULL;
    prot = RT_NULL;
    switch (event)
    {
    case RT_WLAN_DEV_EVT_INIT_DONE:
    {
        LOG_D("L%d event: INIT_DONE", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_INIT_DONE;
        break;
    }
    case RT_WLAN_DEV_EVT_CONNECT:
    {
        LOG_D("L%d event: CONNECT", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_CONNECT;
        break;
    }
    case RT_WLAN_DEV_EVT_DISCONNECT:
    {
        LOG_D("L%d event: DISCONNECT", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_DISCONNECT;
        break;
    }
    case RT_WLAN_DEV_EVT_AP_START:
    {
        LOG_D("L%d event: AP_START", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_AP_START;
        break;
    }
    case RT_WLAN_DEV_EVT_AP_STOP:
    {
        LOG_D("L%d event: AP_STOP", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_AP_STOP;
        break;
    }
    case RT_WLAN_DEV_EVT_AP_ASSOCIATED:
    {
        LOG_D("L%d event: AP_ASSOCIATED", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_AP_ASSOCIATED;
        break;
    }
    case RT_WLAN_DEV_EVT_AP_DISASSOCIATED:
    {
        LOG_D("L%d event: AP_DISASSOCIATED", __LINE__);
        prot_event = RT_WLAN_PROT_EVT_AP_DISASSOCIATED;
        break;
    }
    default:
    {
        return;
    }
    }
    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if ((prot_event_tab[prot_event][i].handler != RT_NULL) &&
            (prot_event_tab[prot_event][i].prot->type == wlan_prot->type))
        {
            handler = prot_event_tab[prot_event][i].handler;
            prot = prot_event_tab[prot_event][i].prot;
            break;
        }
    }

    if (handler != RT_NULL)
    {
        handler(prot, wlan, prot_event);
    }
}

static struct rt_wlan_device *rt_wlan_prot_find_by_name(const char *name)
{
    rt_device_t device;

    if (name == RT_NULL)
    {
        LOG_E("F:%s L:%d Parameter Wrongful", __FUNCTION__, __LINE__);
        return RT_NULL;
    }
    device = rt_device_find(name);
    if (device == RT_NULL)
    {
        LOG_E("F:%s L:%d not find wlan dev!! name:%s", __FUNCTION__, __LINE__, name);
        return RT_NULL;
    }
    return (struct rt_wlan_device *)device;
}

rt_err_t rt_wlan_prot_attach(const char *name, rt_wlan_prot_type_t type)
{
    struct rt_wlan_device *wlan;

    wlan = rt_wlan_prot_find_by_name(name);
    if (wlan == RT_NULL)
    {
        return -RT_ERROR;
    }
    return rt_wlan_prot_attach_dev(wlan, type);
}

rt_err_t rt_wlan_prot_detach(const char *name)
{
    struct rt_wlan_device *wlan;

    wlan = rt_wlan_prot_find_by_name(name);
    if (wlan == RT_NULL)
    {
        return -RT_ERROR;
    }
    return rt_wlan_prot_detach_dev(wlan);
}

rt_err_t rt_wlan_prot_attach_dev(struct rt_wlan_device *wlan, rt_wlan_prot_type_t type)
{
    int i = 0;
    struct rt_wlan_prot *prot = wlan->prot;
    rt_wlan_dev_event_t event;

    if (wlan == RT_NULL)
    {
        LOG_E("F:%s L:%d wlan is null", __FUNCTION__, __LINE__);
        return -RT_ERROR;
    }

    if (wlan->prot != RT_NULL && ((struct rt_wlan_prot *)(wlan->prot))->type == type)
    {
        LOG_D("prot is register");
        return RT_EOK;
    }

    /* if prot not NULL */
    if (prot != RT_NULL)
        rt_wlan_prot_detach_dev(wlan);

#ifdef RT_WLAN_PROT_LWIP_PBUF_FORCE
    if (type != RT_WLAN_PROT_LWIP)
    {
        return -RT_ERROR;
    }
#endif
    /* find prot */
    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if ((_prot[i] != RT_NULL) && (_prot[i]->type == type))
        {
            /* attach prot */
            wlan->prot = _prot[i]->ops->dev_reg_callback(_prot[i], wlan);
            break;
        }
    }

    if (i >= RT_WLAN_PROT_MAX)
    {
        LOG_E("F:%s L:%d not find wlan protocol", __FUNCTION__, __LINE__);
        return -RT_ERROR;
    }

    for (event = RT_WLAN_DEV_EVT_INIT_DONE; event < RT_WLAN_DEV_CALLBACK_PROMISC; event ++)
    {
        if (rt_wlan_dev_register_event_handler(wlan, event, rt_wlan_prot_event_handle, RT_NULL) != RT_EOK)
        {
            LOG_E("prot register event filed:%d", event);
        }
    }

    return RT_EOK;
}

rt_err_t rt_wlan_prot_detach_dev(struct rt_wlan_device *wlan)
{
    struct rt_wlan_prot *prot = wlan->prot;
    rt_wlan_dev_event_t event;

    if (prot == RT_NULL)
        return RT_EOK;

    for (event = RT_WLAN_DEV_EVT_INIT_DONE; event < RT_WLAN_DEV_EVT_MAX; event ++)
    {
        rt_wlan_dev_unregister_event_handler(wlan, event, rt_wlan_prot_event_handle);
    }

    /* detach prot */
    prot->ops->dev_unreg_callback(prot, wlan);
    wlan->prot = RT_NULL;

    return RT_EOK;
}

rt_err_t rt_wlan_prot_regisetr(struct rt_wlan_prot *prot)
{
    int i;

    /* Parameter checking */
    if ((prot == RT_NULL) || 
        (prot->type < RT_WLAN_PROT_LWIP) ||
        (prot->type >= RT_WLAN_PROT_MAX) ||
        (prot->ops->prot_recv == RT_NULL) ||
        (prot->ops->dev_reg_callback == RT_NULL))
    {
        LOG_E("F:%s L:%d Parameter Wrongful", __FUNCTION__, __LINE__);
        return -RT_EINVAL;
    }

    /* save prot */
    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if ((_prot[i] == RT_NULL) || (_prot[i]->type == prot->type))
        {
            _prot[i] = prot;
            break;
        }
    }

    /* is full */
    if (i >= RT_WLAN_PROT_MAX)
    {
        LOG_E("F:%s L:%d Space full", __FUNCTION__, __LINE__);
        return -RT_ERROR;
    }

    return RT_EOK;
}

rt_err_t rt_wlan_prot_event_register(struct rt_wlan_prot *prot, rt_wlan_prot_event_t event, rt_wlan_prot_event_handler handler)
{
    int i;

    if ((prot == RT_NULL) || (handler == RT_NULL))
    {
        return -RT_EINVAL;
    }

    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if (prot_event_tab[event][i].handler == RT_NULL)
        {
            prot_event_tab[event][i].handler = handler;
            prot_event_tab[event][i].prot = prot;
            return RT_EOK;
        }
    }

    return -RT_ERROR;
}

rt_err_t rt_wlan_prot_event_unregister(struct rt_wlan_prot *prot, rt_wlan_prot_event_t event)
{
    int i;

    if (prot == RT_NULL)
    {
        return -RT_EINVAL;
    }

    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if ((prot_event_tab[event][i].handler != RT_NULL) && 
            (prot_event_tab[event][i].prot == prot))
        {
            rt_memset(&prot_event_tab[event][i], 0, sizeof(struct rt_wlan_prot_event_des));
            return RT_EOK;
        }
    }

    return -RT_ERROR;
}

rt_err_t rt_wlan_prot_transfer_dev(struct rt_wlan_device *wlan, void *buff, int len)
{
    if (wlan->ops->wlan_send != RT_NULL)
    {
        return wlan->ops->wlan_send(wlan, buff, len);
    }
    return -RT_ERROR;
}

rt_err_t rt_wlan_dev_transfer_prot(struct rt_wlan_device *wlan, void *buff, int len)
{
    struct rt_wlan_prot *prot = wlan->prot;

    if (prot != RT_NULL)
    {
        return prot->ops->prot_recv(wlan, buff, len);
    }
    return -RT_ERROR;
}

extern int rt_wlan_prot_ready_event(struct rt_wlan_device *wlan);
int rt_wlan_prot_ready(struct rt_wlan_device *wlan)
{
    return rt_wlan_prot_ready_event(wlan);
}

void rt_wlan_prot_dump(void)
{
    int i;
    char *type;

    rt_kprintf("id    name     type \n");
    rt_kprintf("--  --------  ------\n");
    for (i = 0; i < RT_WLAN_PROT_MAX; i++)
    {
        if (_prot[i] != RT_NULL)
        {
            rt_kprintf("%2d  ", i);
            rt_kprintf("%-8.8s  ", _prot[i]->name);
            switch (_prot[i]->type)
            {
            case RT_WLAN_PROT_LWIP: type = "LWIP"; break;
            default: type = "UNKNOWN"; break;
            }
            rt_kprintf("%-7.7s\n", type);
        }
    }
}


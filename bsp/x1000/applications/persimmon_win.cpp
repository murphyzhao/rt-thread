/*
* File      : persimmon_win.c
* This file is part of RT-Thread RTOS
* COPYRIGHT (C) 2009-2016 RT-Thread Develop Team
*/
#include "persimmon_animation.h"
#include <rtthread.h>

#include <pm_application.h>
#include <pm_window.h>


extern "C" {
void ui_main(void)
{
    Application *app = new Application("MainWin");
    if (app)
    {
        // MainWin *win = new MainWin();
		PersimmonDemoWin *win = new PersimmonDemoWin();
        if (win)
        {
            win->show(true);
            delete win;
        }

        delete app;
    }
}
}
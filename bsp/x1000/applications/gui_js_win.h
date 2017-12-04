/*
* File      : animation.h
* This file is part of RT-Thread RTOS
* COPYRIGHT (C) 2009-2016 RT-Thread Develop Team
*/
#pragma once

#include <rtthread.h>
#include <pm_window.h>
#include <pm_button.h>

#define TTF			"/TTF/demo.ttf"

using namespace Persimmon;

class GuiJsWin : public Window
{
public:
	GuiJsWin();
	virtual ~GuiJsWin();

	// virtual bool handleGestureEvent(struct rtgui_event_gesture *gev, const struct rtgui_gesture *gest);

	void onButton(int id);

	Button* getButtonInstance(void)
	{
		return button;
	}

	bool getIsPressedValue( )
	{
		return ispressed;
	}
	
	
private:
	Button* button;
	bool ispressed;

};

extern "C" {
void gui_js_mainwin(void);
}
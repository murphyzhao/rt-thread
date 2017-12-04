#include <rtthread.h>

#ifdef RT_USING_GUIENGINE

#include "gui_js_win.h"
#include <pm_application.h>
#include <pm_window.h>
#include <pm_button.h>
#include <resource.h>
#include <pm_container.h>
// #include <base_win.h>

#include "jerryscript.h"
#include "jerryscript-port.h"
#include "jerryscript-core.h"
// extern char* read_sources(const char *script_file_names[], int files_count, size_t *out_source_size_p);

// #include "jerry_run.h"


using namespace Persimmon;
GuiJsWin::GuiJsWin() : Window("button")
{
	rt_kprintf(" print in GuiJsWin( ) function\n");
    setBackground(WHITE);
    button = new Button(getImage("sd/button/button_nor.png"), getImage("sd/button/button_down.png"));
	button->setId(1);
	button->moveTo(20, 10);
	button->setBackground(TRANSPARENT);
	button->clicked.connect(this, &GuiJsWin::onButton);
	button->setText("OpenLED");
	button->setFont(rtgui_font_refer(TTF, 20));
	addChild(button);

	ispressed = false;

}

GuiJsWin::~GuiJsWin()
{

}

char* read_js_sources(const char *script_file_names[], int files_count, size_t *out_source_size_p)
{
	int i;
	char* source_buffer = NULL;
	char *source_buffer_tail = NULL;
	size_t total_length = 0;
	FILE *file = NULL;

	for (i = 0; i < files_count; i++)
	{
		const char *script_file_name = script_file_names[i];

		file = fopen(script_file_name, "r");
		if (file == NULL) {
			jerry_port_log(JERRY_LOG_LEVEL_ERROR, "Failed to fopen [%s]\n",
					script_file_name);
			return NULL;
		}

		int fseek_status = fseek(file, 0, SEEK_END);
		if (fseek_status != 0)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR,
					"Failed to fseek fseek_status(%d)\n", fseek_status);
			fclose(file);
			return NULL;
		}

		long script_len = ftell(file);
		if (script_len < 0)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR,
					"Failed to ftell script_len(%ld)\n", script_len);
			fclose(file);
			break;
		}

		total_length += (size_t) script_len;

		fclose(file);
		file = NULL;
	}

	if (total_length <= 0)
	{
		jerry_port_log(JERRY_LOG_LEVEL_ERROR, "There's nothing to read\n");
		return NULL;
	}

	source_buffer = (char*) rt_malloc(total_length);

	if (source_buffer == NULL)
	{
		jerry_port_log(JERRY_LOG_LEVEL_ERROR, "Out of memory error\n");
		return NULL;
	}

	memset(source_buffer, 0, sizeof(char) * total_length);
	source_buffer_tail = source_buffer;

	for (i = 0; i < files_count; i++)
	{
		const char *script_file_name = script_file_names[i];
		file = fopen(script_file_name, "r");

		if (file == NULL)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR, "Failed to fopen [%s]\n",
					script_file_name);
			break;
		}

		int fseek_status = fseek(file, 0, SEEK_END);
		if (fseek_status != 0)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR,
					"Failed to fseek fseek_status(%d)\n", fseek_status);
			break;
		}

		long script_len = ftell(file);
		if (script_len < 0)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR,
					"Failed to ftell script_len(%ld)\n", script_len);
			break;
		}

		rewind(file);

		const size_t current_source_size = (size_t) script_len;
		size_t bytes_read = fread(source_buffer_tail, 1, current_source_size,
				file);
		if (bytes_read < current_source_size)
		{
			jerry_port_log(JERRY_LOG_LEVEL_ERROR,
					"Failed to fread bytes_read(%d)\n", bytes_read);
			break;
		}

		fclose(file);
		file = NULL;

		source_buffer_tail += current_source_size;
	}

	if (file != NULL)
	{
		fclose(file);
	}

	if (i < files_count)
	{
		jerry_port_log(JERRY_LOG_LEVEL_ERROR, "Failed to read script N%d\n",
				i + 1);
		rt_free(source_buffer);
		return NULL;
	}

	*out_source_size_p = (size_t) total_length;

	return source_buffer;
} /* read_sources */
extern "C"
{
	extern int js_func_executor (uint8_t* js_func_name, int32_t args[], uint8_t argv);
}
void 
GuiJsWin::onButton(int id)
{
	int32_t js_func_args[10];
	ispressed = !ispressed;

	// switch(id)
	// {
	// case 1:
	// {
		

		// call js onbutton1 func 

        // rt_kprintf("OpenLED is clicked!\n");
		// if (ispressed)
		// {
		// 	button->setText("CloseLED");
		// }
		// else{
		// 	button->setText("OpenLED");
		// }
		js_func_args[0] = id;
		js_func_args[1] = (int32_t) ispressed;
		js_func_executor((uint8_t*)"js_onButton", js_func_args, 2);
		
    // }
    // }
}


GuiJsWin *win;

extern "C" {
extern int jerry_main(void);
void gui_js_mainwin(void)
{
    Application *app = new Application("MainWin");
    if (app)
    {
		win = new GuiJsWin();
		
        if (win)
        {
			jerry_main();
            win->show(true);
            delete win;
        }
		

        // delete app;
    }    
}
}

#endif
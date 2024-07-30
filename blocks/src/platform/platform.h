#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN64
	#include "Windows/platform_win64.c"
#else
	#include "Linux/platform_linux.h"
#endif

#define initial_width 700
#define initial_height 800

#define display_name "blocks"

struct window_state {
	unsigned int window_width;
	unsigned int window_height;
};

extern bool active;

extern struct window_state window_state;

void sleep_for_ms(unsigned int time_in_milliseconds);

void set_console_cursor_position(int x, int y);

double get_time();

char get_key_state(int key);

void draw_to_window(unsigned int* buffer, int width, int height);

void* create_thread(void* address, void* args);

void join_thread(void* thread_handle);

void Entry();

#endif // PLATFORM_H

#ifndef PLATFORM_WIN32_H
#define PLATFORM_WIN32_H

#include "headers.h"

struct window_state {
	unsigned int window_width;
	unsigned int window_height;
};

extern bool active;

extern struct window_state window_state;

void sleep_for_ms(unsigned int time_in_milliseconds);

void set_console_curser(int x, int y);

double get_time();

char get_key_state(int key);

void draw_to_window(unsigned int* buffer, int width, int height);

void* create_thread(void* address, void* args);

void join_thread(void* thread_handle);

int Entry();

#endif // !PLATFORM_H


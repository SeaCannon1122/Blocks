#ifndef PLATTFORM_H
#define PLATTFORM_H

#include <windows.h>
#include "headers.h"

struct RENDER_STATE {
	unsigned int* buffer;
	unsigned int buffer_width;
	unsigned int buffer_height;
};

extern bool active;
extern bool msgcheck;
extern bool writing_to_buffer;

extern struct RENDER_STATE render_state;

void sleepforms(unsigned int _time_in_milliseconds);

void clear_console();

double get_time();

short keystate(int key);

void drawWindow(unsigned int* buffer, int width, int height);

void* create_thread(void* address, void* args, int* mainthreadID);

void join_thread(void* thread_handle);

POINT GetMousePos();

#endif // !PLATTFORM_H


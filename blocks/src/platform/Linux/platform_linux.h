#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#ifndef _WIN64

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

#define KEY_SPACE XK_space
#define KEY_SHIFT XK_Shift_L
#define KEY_ARROW_LEFT XK_Left
#define KEY_ARROW_RIGHT XK_Right
#define KEY_ARROW_UP XK_Up
#define KEY_ARROW_DOWN XK_Down

#endif

#endif
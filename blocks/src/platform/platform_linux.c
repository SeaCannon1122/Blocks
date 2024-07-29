#if defined(__linux__)


#include "headers.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>

bool p_active = true;

struct window_state p_window_state;

Display* display;
Window window;
GC gc;
XImage* ximage;
int screen;
unsigned char keyStates[256] = { 0 };

void p_sleep_for_ms(unsigned int time_in_milliseconds) {
    usleep(time_in_milliseconds * 1000);
}

void p_set_console_cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

double p_get_time() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return current_time.tv_sec * 1000.0 + current_time.tv_usec / 1000.0;
}

void p_draw_to_window(unsigned int* buffer, int width, int height) {
    if (ximage) {
        XDestroyImage(ximage);
    }
    ximage = XCreateImage(display, DefaultVisual(display, screen), 24, ZPixmap, 0, (char*)buffer, width, height, 32, 0);
    XPutImage(display, window, gc, ximage, 0, 0, 0, 0, width, height);
    XFlush(display);
}

void* p_create_thread(void* (*address)(void*), void* args) {
    pthread_t thread;
    pthread_create(&thread, NULL, address, args);
    return (void*)thread;
}

void p_join_thread(void* thread_handle) {
    pthread_join((pthread_t)thread_handle, NULL);
}

char p_get_key_state(int key) {
    char keyState = 0;
    char keys_return[32];
    XQueryKeymap(display, keys_return);

    KeyCode kc = XKeysymToKeycode(display, key);
    bool isKeyPressed = !!(keys_return[kc >> 3] & (1 << (kc & 7)));

    if (isKeyPressed) keyState |= 0b0001;
    if (isKeyPressed != keyStates[key]) keyState |= 0b0010;
    keyStates[key] = isKeyPressed;

    return keyState;
}

void WindowControl() {
    XEvent event;
    while (p_active) {
        while (XPending(display) > 0) {
            XNextEvent(display, &event);
            if (event.type == ConfigureNotify) {
                XConfigureEvent xce = event.xconfigure;
                p_window_state.window_width = xce.width;
                p_window_state.window_height = xce.height;
            }
            else if (event.type == DestroyNotify || event.type == ClientMessage) {
                p_active = false;
            }
        }
        p_sleep_for_ms(10);
    }

    printf("Linux: received stop signal\n");
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open X display\n");
        return -1;
    }

    screen = DefaultScreen(display);
    int black = BlackPixel(display, screen);
    int white = WhitePixel(display, screen);

    window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 700, 800, 1, black, white);

    XSetStandardProperties(display, window, "Blocks", "Blocks", None, NULL, 0, NULL);

    Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);

    gc = XCreateGC(display, window, 0, NULL);

    XMapWindow(display, window);

    p_window_state.window_width = 700;
    p_window_state.window_height = 800;

    void* mainthread = p_create_thread((void* (*)(void*))Entry, NULL);

    WindowControl();

    p_join_thread(mainthread);

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

#endif
#ifndef _WIN64

#define KEY_SPACE XK_space
#define KEY_SHIFT XK_Shift_L
#define KEY_ARROW_LEFT XK_Left
#define KEY_ARROW_RIGHT XK_Right
#define KEY_ARROW_UP XK_Up
#define KEY_ARROW_DOWN XK_Down

#include "platform_linux.h"
#include "headers.h"

bool active = true;

struct window_state window_state;

Display* display;
Window window;
Atom wmDeleteMessage;
XImage* ximage;
int screen;
unsigned int* image_buffer;

int max_width;
int max_height;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool keyStates[256] = { 0 };

void sleep_for_ms(unsigned int _time_in_milliseconds) {
    usleep(_time_in_milliseconds * 1000);
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec * 1000 + (double)tv.tv_usec / 1000;
}

void* create_thread(void* address, void* args) {
    pthread_t *thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, (void* (*)(void*))address, args);
    return thread;
}

void join_thread(void* thread_handle) {
    pthread_join(*(pthread_t*)thread_handle, NULL);
    free(thread_handle);
}

void set_console_cursor_position(int x, int y) {
    // X11 does not have direct console cursor manipulation; this is a placeholder
}

void draw_to_window(unsigned int* buffer, int width, int height) {
    
    for(int i = 0; i < width && i < max_width; i++) {
        for (int j = 0; j < height && j < max_height; j++) image_buffer[i + j * max_width] = buffer[i + (height - j - 1) * width];
    }
    
    XPutImage(display, window, DefaultGC(display, screen), ximage, 0, 0, 0, 0, width, height);
}


char get_key_state(int key) {
    char keys[32];
    XQueryKeymap(display, keys);

    KeySym keysym = (KeySym)key;
    KeyCode keycode = XKeysymToKeycode(display, keysym);

    int byteIndex = keycode / 8;
    int bitIndex = keycode % 8;

    return (keys[byteIndex] & (1 << bitIndex) ? 0b0001 : 0b0000);
}

void WindowControl() {
    XEvent event;
    while (active) {
        while (XPending(display)) {
            XNextEvent(display, &event);
            switch (event.type) {
                case ConfigureNotify:
                    window_state.window_width = clamp_int(event.xconfigure.width, 0, max_width);
                    window_state.window_height = clamp_int(event.xconfigure.height, 0, max_height);
                    break;
                case ClientMessage:
                    if ((Atom)event.xclient.data.l[0] == wmDeleteMessage) {
                        active = false;
                    }
            }

        }
    
        sleep_for_ms(10);
    }
    
    printf("Linux: received stop signal\n");
    return;
}


int main(int argc, char *argv[]) {
    XInitThreads();

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        return 1;
    }

    screen = DefaultScreen(display);

    max_width = DisplayWidth(display, screen);
    max_height = DisplayHeight(display, screen);

    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, initial_width, initial_height, 1, BlackPixel(display, screen), WhitePixel(display, screen));
    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);
    XStoreName(display, window, display_name);
    wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    XMapWindow(display, window);

    image_buffer = (unsigned int*) malloc(max_width * max_height * sizeof(unsigned int));

    ximage = XCreateImage(display, DefaultVisual(display, screen), DefaultDepth(display, screen), ZPixmap, 0, (char *)image_buffer, max_width, max_height, 32, 0);

    window_state.window_width = initial_width;
    window_state.window_height = initial_height;

    double startTime = get_time();

    void* mainthread = create_thread(Entry, NULL);

    WindowControl();

    join_thread(mainthread);

    XDestroyImage(ximage);
    XCloseDisplay(display);

    return 0;
}


#endif

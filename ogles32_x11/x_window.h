#ifndef _XWIND_HH_
#define _XWIND_HH_

#include <X11/Xlib.h>
#include <stdlib.h>

#define VisualScreenMask 0x2

Display *display;
Window window;
Screen *screen;
XEvent ev;

int window_init();

int Xevent_loop();

#endif
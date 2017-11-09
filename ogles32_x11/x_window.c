#ifndef __XWIN_H_
        #include <stdio.h>
        #include "x_window.h"
#endif

int window_init()
{
        printf("%s\n",__func__);
        /////////////////////////   X-Window   //////////////////////////
        
        
        int screenId;
        

        typedef struct
        {
                Visual *visual;
                VisualID visualid;
                int screen;
                int depth;
#if defined(__cplusplus) || defined(c_plusplus)
                int c_class; /* C++ */
#else
                int class;
#endif
        unsigned long red_mask;
        unsigned long green_mask;
        unsigned long blue_mask;
        int colormap_size;
        int bits_per_rgb;
        } XVisualInfo;

        typedef XID Colormap;

        XVisualInfo *visual;
        Colormap *colormap = NULL;

        // Open the display
        display = XOpenDisplay(NULL);
        if (display == NULL)
        {
                printf("%s\n", "Could not open display");
                return 1;
        }
        screen = DefaultScreenOfDisplay(display);
        screenId = DefaultScreen(display);

        // Getting memory for the colormap
        colormap = (Colormap *)malloc(sizeof(Colormap));
        if (colormap == NULL)
        {
                printf("Error while allocating memory for the colormap");
                return -1;
        }

        // Open the window
        window = XCreateSimpleWindow(display, RootWindowOfScreen(screen), 0, 0, 320, 200, 1, BlackPixel(display, screenId), WhitePixel(display, screenId));

        XStoreName(display, window, "Hello_X_Window");

        // Show the window
        XClearWindow(display, window);
        XMapWindow(display, window);
        XMapRaised(display, window);

        return 0;
}

int Xevent_loop()
{
        while (1)
        {
                XNextEvent(display, &ev);
                // Do something
        }
        // Cleanup
        XDestroyWindow(display, window);
        XFree(screen);
        XCloseDisplay(display);

        return 0;
}
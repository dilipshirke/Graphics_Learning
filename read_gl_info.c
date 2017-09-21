#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};
int main(int argc, char ** argv)
{
        EGLBoolean eRetStatus;
        EGLDisplay egl_dpy;
        EGLNativeWindowType eglWindow = 0;
        EGLConfig config;
        EGLContext context;
        EGLSurface surface;
        EGLConfig configs[2];
        NativeWindowType native_window;
        EGLint num_config;
        EGLint major, minor;
        EGLint context_attribs[] = {EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
                                        EGL_CONTEXT_MINOR_VERSION_KHR, 0,
                                        EGL_NONE};
        EGLint config_count;
        EGLint cfg_attribs[] = {EGL_BUFFER_SIZE,    EGL_DONT_CARE,
                                EGL_DEPTH_SIZE,         16,
                                EGL_RED_SIZE,       5,
                                EGL_GREEN_SIZE,     6,
                                EGL_BLUE_SIZE,      5,
                                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                                EGL_NONE};

        /* get an EGL display connection */
        egl_dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        eRetStatus = eglInitialize(egl_dpy, &major, &minor);
        if( eRetStatus != EGL_TRUE )
        {
                printf("Error : eglInitialize\n");
        }
        else
        {
                printf("EGL (%d.%d) Initialized\n", major, minor);
        }

        eRetStatus = eglChooseConfig (egl_dpy, cfg_attribs, configs, 2, &config_count);
        if( eRetStatus != EGL_TRUE  || !config_count)
                printf ("Error: eglChooseConfig\n");


        surface = eglCreateWindowSurface(egl_dpy, configs[0], eglWindow, NULL);
        if (surface == EGL_NO_SURFACE)
        {
                printf("error eglCreateWindowSurface\n");
        }

        eRetStatus = eglBindAPI(EGL_OPENGL_ES_API);
        if (eRetStatus != EGL_TRUE)
        {
                printf("error eglBindAPI\n");
        }

        context = eglCreateContext (egl_dpy, configs[0], EGL_NO_CONTEXT, context_attribs);
        if (context == EGL_NO_CONTEXT)
        {
                printf("error eglCreateContext");
        }

        eRetStatus = eglMakeCurrent (egl_dpy, surface, surface, context);
        if( eRetStatus != EGL_TRUE )
                printf("error eglMakeCurrent");


        glClear(GL_COLOR_BUFFER_BIT);
        printf("Vendor = %s\n",glGetString(GL_VENDOR));
        printf("Renderer = %s\n",glGetString(GL_RENDERER));
        printf("Version = %s\n",glGetString(GL_VERSION));
        printf("GLSL version = %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("Supported GL Ext = %s\n",glGetString(GL_EXTENSIONS));

        return EXIT_SUCCESS;
}
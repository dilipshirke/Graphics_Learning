#ifndef __GL_EGL_R_H_
#define __GL_EGL_R_H_
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES2/gl2.h>
    #include <time.h>


#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_MAP_READ_BIT                   0x0001


EGLDisplay egl_display;
EGLSurface egl_surface;

int ShaderProgram;
//Create Buffer Objects and fill the shader_data
static int VAO, VBO, SSBO;
float sum = 0.0;

int pre_render_work();
#endif

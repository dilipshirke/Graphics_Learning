#ifndef __GL_EGL_H_
#define __GL_EGL_H_
    #include <stdio.h>
    #include "gl_egl_render.h"
    #include "x_window.h"
#endif


int pre_render_work()
{
    //point vertices
    printf("%s\n",__func__);
    float vertices[] =
        {
            //1
            +0.0f, +0.0f,
            +0.78f, +0.0f, +0.2f,
            +0.4f, +0.0f,
            +0.78f, +0.0f, +0.2f,
            +0.4f, +0.4f,
            +0.78f, +0.0f, +0.2f,

            //2
            +0.0f, +0.0f,
            +0.58f, +0.06f, +0.26f,
            +0.4f, +0.4f,
            +0.58f, +0.06f, +0.26f,
            +0.0f, +0.75f,
            +0.58f, +0.06f, +0.26f,

            //3
            +0.0f, +0.0f,
            +1.0f, +0.6f, +0.0f,
            +0.0f, +0.4f,
            +1.0f, +0.6f, +0.0f,
            -0.4f, +0.4f,
            +1.0f, +0.6f, +0.0f,
            //4
            +0.0f, +0.0f,
            +0.99f, +0.84f, +0.30f,
            -0.4f, +0.4f,
            +0.99f, +0.84f, +0.30f,
            -0.75f, +0.0f,
            +0.99f, +0.84f, +0.30f,

            //5
            +0.0f, +0.0f,
            +0.027f, +0.69f, +0.039f,
            -0.4f, +0.0f,
            +0.027f, +0.69f, +0.039f,
            -0.4f, -0.4f,
            +0.027f, +0.69f, +0.039f,

            //6
            +0.0f, +0.0f,
            +0.62f, +0.85f, +0.44f,
            -0.4f, -0.4f,
            +0.62f, +0.85f, +0.44f,
            +0.0f, -0.75f,
            +0.62f, +0.85f, +0.44f,

            //7
            +0.0f, +0.0f,
            +0.011f, +0.21f, +0.89f,
            +0.0f, -0.4f,
            +0.011f, +0.21f, +0.89f,
            +0.4f, -0.4f,
            +0.011f, +0.21f, +0.89f,

            //8
            +0.0f, +0.0f,
            +0.44f, +0.55f, +0.89f,
            +0.4f, -0.4f,
            +0.44f, +0.55f, +0.89f,
            +0.75f, +0.0f,
            +0.44f, +0.55f, +0.89f,
        };    

    //vertext attrib array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //shader storage buffer object
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}

void render_image()
{
    printf("%s\n",__func__);   
    //bind vertex attribute buffer object
    glBindVertexArray(VAO);
    
    //clear window
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //render
    glDrawArrays(GL_TRIANGLES, 0, 24);  
    //toggle framebuffer
    eglSwapBuffers(egl_display, egl_surface);
}


float read_from_shader()
{
        printf("%s\n",__func__);
        //read addition of uniforms
    float shader_data = 0.0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    float *p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 1*sizeof(float), GL_MAP_READ_BIT);
    //memcpy(&shader_data, p, sizeof(float));
    printf("shader data = %f\n",*p);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return shader_data;
}

int set_uniforms()
{
        printf("%s\n",__func__);   
    //Fill the uniform varibles values
    GLint loc;
    char str[8];    
    char *uniform_vars[] = {"VAR0", "VAR1", "VAR2", "VAR3"};
    int i = 0;

    srand((unsigned int)time(NULL));
    
    for (i = 0; i < 4; i++)
    {
        sprintf(str, "%s%d", "VAR", i);
        loc = glGetUniformLocation(ShaderProgram, uniform_vars[i]);
        if (loc != -1)
        {
            float temp = (((float)rand() / (float)(RAND_MAX)));
            glUniform1f(loc, temp);
            printf("temp = %f\n", temp);
            sum += temp;
            printf("\nuniform location found\n");
        }
        else
            printf("\nuniform location NOT found\n");
    }
    return 0;

}


int EGL_init()
{
        printf("%s\n",__func__);              
        EGLBoolean eRetStatus;
        EGLint major, minor;
        EGLint cfg_attribs[] = {EGL_BUFFER_SIZE, EGL_DONT_CARE,
                                EGL_DEPTH_SIZE, 16,
                                EGL_RED_SIZE, 5,
                                EGL_GREEN_SIZE, 6,
                                EGL_BLUE_SIZE, 5,
                                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                                EGL_NONE};
        EGLConfig configs[2];
        EGLint config_count = 0;

        
        EGLContext egl_context;
        EGLint context_attribs[] = {EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
                                    EGL_CONTEXT_MINOR_VERSION_KHR, 0,
                                    EGL_NONE};

        egl_display = eglGetDisplay((EGLNativeDisplayType)display);

        eRetStatus = eglInitialize(egl_display, &major, &minor);
        if (eRetStatus != EGL_TRUE)
        {
                printf("Error : eglInitialize\n");
        }
        else
        {
                printf("EGL (%d.%d) Initialized\n", major, minor);
        }

        eRetStatus = eglChooseConfig(egl_display, cfg_attribs, configs, 2, &config_count);
        if (eRetStatus != EGL_TRUE || !config_count)
                printf("Error: eglChooseConfig\n");

        printf("Number of config = %d\n", config_count);

        egl_surface = eglCreateWindowSurface(egl_display, configs[0], window, NULL);
        if (egl_surface == EGL_NO_SURFACE)
        {
                printf("Error: eglCreateWindowSurface\n");
        }

        eRetStatus = eglBindAPI(EGL_OPENGL_ES_API);
        if (eRetStatus != EGL_TRUE)
        {
                printf("error eglBindAPI\n");
        }

        egl_context = eglCreateContext(egl_display, configs[0], EGL_NO_CONTEXT, context_attribs);
        if (egl_context == EGL_NO_CONTEXT)
        {
                printf("error eglCreateContext");
        }

        eRetStatus = eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);
        if (eRetStatus != EGL_TRUE)
                printf("error eglMakeCurrent");

        return 0;

}

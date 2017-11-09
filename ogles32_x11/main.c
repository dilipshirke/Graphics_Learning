#ifndef __MAIN_H_
#define __MAIN_H_
    #include <stdio.h>   
#endif

int main(int argc, char **argv)
{
    window_init();
    EGL_init();       
    pre_render_work();
    LoadShader();    
    set_uniforms();
    render_image();
    Xevent_loop();               
    return 0;
}
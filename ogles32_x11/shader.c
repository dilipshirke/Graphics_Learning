#ifndef __SHADER_H_
#define __SHADER_H_
    #include <stdio.h>
    #include "shader.h"
#endif


int CheckStatus(int shaderid, char *msg)
{
   int success;
    printf("%s\n",__func__);

   glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);
   if (!success)
   {
       printf("Error: Failed to compile %s", msg);
   }
   return success;
}

int LoadShader()
{
    int VertexShader;
    int FragShader;
    int GeometryShader;
    int success;


    printf("%s\n",__func__);
    //Vertex shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(VertexShader);
    CheckStatus(VertexShader, "VertexShader");

    
    //Fragment shader
    FragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(FragShader);
    CheckStatus(FragShader, "Fragment Shader");
    
    //Geometry shader
    GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(GeometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(GeometryShader);
    CheckStatus(GeometryShader, "GeometryShader");

    //Linker - Linked compiled shader
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, GeometryShader);
    glAttachShader(ShaderProgram, FragShader);
    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        printf("Error: Failed to link shaders.\n");
    }
    
    glUseProgram(ShaderProgram);
    return 0;
}
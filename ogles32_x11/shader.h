#ifndef __SHADER_HH_
#define __SHADER_HH_
#include <GLES2/gl2.h>


#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_MAP_READ_BIT                   0x0001

int ShaderProgram;

//vertex shader source string
const char *vertexShaderSource =
"#version 310 es\n"
"layout (location = 0) in highp vec2 aPos;\n"
"layout (location = 1) in highp vec3 aColor;\n"

"out highp vec3 vColor;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"	vColor = aColor;\n"
"}\n\0";

//geometry shader source string
const char *fragmentShaderSource =
"#version 310 es\n"
"#extension GL_EXT_shader_pixel_local_storage2 : require\n"
"in highp vec3 fColor;"
"out highp vec4 outColor;\n"   
    
"layout(binding=0) buffer shader_data\n"
"{\n"
"       highp float value;\n"
"}ShaderBlock;\n"

"uniform highp float VAR0;\n"
"uniform highp float VAR1;\n"
"uniform highp float VAR2;\n"
"uniform highp float VAR3;\n"


"void main()\n"
"{\n"
"	outColor = vec4(fColor, 1.0);\n"    
"   ShaderBlock.value = VAR0 + VAR1 + VAR2 + VAR3;\n"
"}\n\0";

//fragment shader source string
// Geometry Shader
const char *geometryShaderSource =
"#version 310 es\n"
"#extension GL_EXT_geometry_shader : enable\n"
    
"layout (triangles) in;\n"
"layout (triangle_strip, max_vertices = 6) out;\n"
"in highp vec3 vColor[];\n"
"out highp vec3 fColor;\n"

"void main()\n"
"{\n"

"       fColor=vColor[0];\n"

"	    gl_Position = gl_in[0].gl_Position;\n"
"       EmitVertex();\n"
"       gl_Position = gl_in[1].gl_Position;\n"
"       EmitVertex();\n"
"       gl_Position = gl_in[2].gl_Position;\n"
"       EmitVertex();"
"       EndPrimitive();\n"

"}\n\0";


int CheckStatus(int shaderid, char *msg);
int LoadShader();

#endif
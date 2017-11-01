#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <time.h>
#include <pthread.h>

GLFWwindow *window;
int ShaderProgram;
//Create Buffer Objects and fill the shader_data
static int VAO, VBO, SSBO;
float sum = 0.0;

//vertex shader source string
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"

    "out vec3 vColor;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "	vColor = aColor;\n"
    "}\n\0";

//geometry shader source string
const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 fColor;\n"
    "out vec4 outColor;\n"

    "layout(std430, binding=0) buffer shader_data\n"
    "{\n"
    "       float value;\n"
    "}ShaderBlock;\n"

    "uniform float VAR0;\n"
    "uniform float VAR1;\n"
    "uniform float VAR2;\n"
    "uniform float VAR3;\n"

    "void main()\n"
    "{\n"
    "	outColor = vec4(fColor, 0.5f);\n"
    "       ShaderBlock.value = VAR0 + VAR1 + VAR2 + VAR3;\n"
    "}\n\0";

//fragment shader source string
// Geometry Shader
const char *geometryShaderSource =
    "#version 330 core\n"
    "layout (triangles) in;\n"
    "layout (triangle_strip, max_vertices = 6) out;\n"
    "in vec3 vColor[];\n"
    "out vec3 fColor;\n"

    "void main()\n"
    "{\n"

    "       fColor=vColor[0];\n"

    "	gl_Position = gl_in[0].gl_Position;\n"
    "       EmitVertex();\n"
    "       gl_Position = gl_in[1].gl_Position;\n"
    "       EmitVertex();\n"
    "       gl_Position = gl_in[2].gl_Position;\n"
    "       EmitVertex();"
    "       EndPrimitive();\n"

    "}\n\0";

int CheckStatus(int shaderid, char *msg)
{
    int success;
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

    return 0;
}

int Init_Window(void)
{
    if (!glfwInit())
    {
        printf("Error : Failed to Initilize glfw3.\n");
        return -1;
    }

    window = glfwCreateWindow(800, 600, "New Window", NULL, NULL);
    if (window == NULL)
    {
        printf("Error: Failed to create window.\n");
        return -1;
    }
    glfwMakeContextCurrent(window);
    return 0;
}

int pre_render_work()
{
    //point vertices
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

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        printf("Error: Failed to initialize GLEW\n");
        return -1;
    }

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

int set_uniforms()
{
    //Fill the uniform varibles values
    GLint loc;
    char str[8];    
    char *uniform_vars[] = {"VAR0", "VAR1", "VAR2", "VAR3"};

    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < 4; i++)
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

float read_from_shader()
{
    //read addition of uniforms
    float shader_data = 0.0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    GLvoid *p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), GL_MAP_READ_BIT);
    memcpy(&shader_data, p, sizeof(float));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return shader_data;
}


void render_image()
{
    //render
    glBindVertexArray(VAO);
    
    //clear window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 24);
    glfwSwapBuffers(window);

}

int main()
{

    //Create Native Window and Initilize
    Init_Window();

    //initial all buffer objects and filled with data
    pre_render_work();  
   
    //Load Shader
    LoadShader();

    //Use shader
    glUseProgram(ShaderProgram);
    
    //set uniform varible values 
    set_uniforms(); 
    
    render_image();

    printf("Shader data = %f\n", read_from_shader());
    printf("CPU sum data = %f\n", sum);

    do
    {
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    //delete or release all resources

    return 0;
}
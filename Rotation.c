//Headers

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#define PI 3.14159265
#define ANGLE -90

// Vertex Shader
const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform float u_CosB;\n"
	"uniform float u_SinB;\n"
	"void main()\n"
	"{\n"
	"	gl_Position.x = aPos.x * u_CosB - aPos.y * u_SinB;\n"
	"       gl_Position.y = aPos.x * u_SinB + aPos.y * u_CosB;\n"
	"       gl_Position.z = aPos.z;\n"
	"       gl_Position.w = 1.0;\n"
	"}\n\0";

// Fragment Shader
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
	"}\n\0";
	


int main(int agrc, char **argv)
{
	//do windowing related stuff here

	if ( !glfwInit())
	{
		printf("Error: Failed to initialize GLFW\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
	if (window == NULL) 
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("Error: Failed to initialize GLEW\n");
		return -1;
	}

	// Shader related stuff
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		printf("Error: Failed to compile vertex shader.\n");
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
        {
                printf("Error: Failed to compile fragment shader.\n");
        }
	

	//link shaders	

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
        {
                printf("Error: Failed to link shaders.\n");
        }

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);	
	
	//stuff for rotation 
        float radian, cosB, sinB;
        int u_CosB, u_SinB;

        radian = PI * ANGLE / 180.0;
        cosB = cos(radian);
        sinB = sin(radian);

        u_CosB = glGetUniformLocation(shaderProgram, "u_CosB");
        u_SinB = glGetUniformLocation(shaderProgram, "u_SinB");

	if( u_CosB != -1 && u_SinB != -1)
	{
		glUniform1f(u_CosB, cosB);
		glUniform1f(u_SinB, sinB);
	}
	else
	{
		printf("Error: Uniform location not found.\n");
	}


	//declare vertices
	GLfloat verts[] = 
	{
		+0.0f, +0.5f, +0.0f,
		-0.5f, -0.5f, +0.0f,
		+0.5f, -0.5f, +0.0f,					
	};
	
	//VBO related activity
	//declare VAO, VBO 
	GLuint VAO, VBO, EBO;

	//get unique name/ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	
	// Bind VAO first, then bind and set VBOs and then configure vertex attributes
	//bind VAO
	glBindVertexArray(VAO);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//copy data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);

	do{
		glfwPollEvents();
	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	return 0;
	
}

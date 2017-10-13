#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 	//for sleep call
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include <math.h>
#define PI 3.14159265
#define ANGLE 10


const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"		
	"}\n\0";

// Fragment Shader
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
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
	"	FragColor = vec4(1.0f, 0.2f, 0.1f, 1.0f);\n"	
	"       ShaderBlock.value = VAR1 + VAR2 + VAR3 + VAR0 ;\n"
	"}\n\0";

// Geometry Shader
const char *geometryShaderSource = 
	"#version 330 core\n"
	"layout (triangles) in;\n"
	"layout (triangle_strip, max_vertices = 9) out;\n"
	"uniform float u_CosB;\n"
	"uniform float u_SinB;\n"

	"vec4 Triangle[3];\n"
	"void main()\n"
	"{\n"

	"       Triangle[0] = gl_in[0].gl_Position;\n"
        "       Triangle[1] = gl_in[1].gl_Position;\n"
        "       Triangle[2] = gl_in[2].gl_Position;\n"

	"//       Triangle[0] = Triangle[0]* u_CosB - Triangle[1] * u_SinB;\n"
        "//       Triangle[1] = Triangle[0] * u_CosB + Triangle[1] * u_CosB;\n"
        "//       Triangle[2] = Triangle[2];\n"

	
		"gl_Position = Triangle[0];\n"
		"EmitVertex();\n"
		"gl_Position = Triangle[1];\n"
		"EmitVertex();\n"
		"gl_Position = Triangle[2];\n"
		"EmitVertex();\n"
		"EndPrimitive();\n"
	"}\n\0";




int main(int argc, char **argv)
{
	if ( !glfwInit())
	{
		printf("Error: Failed to initialize GLFW\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "House with Geometry Shader", NULL, NULL);
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

	//point vertices
	float vertices[] = 
	{
		//1
		0.0f, 0.0f, 0.0f, 
		0.5f, 0.0f, 0.0f, 	
		0.5f, 0.5f, 0.0f, 

		//2	 
		 0.0f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f, 
	
		//3 	
		-0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		
		//4 
		0.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f		 
	};


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
	
	
	int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
                printf("Error: Failed to compile geometry shader.\n");
        }
	
	

	//link shaders	

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
        {
                printf("Error: Failed to link shaders.\n");
        }



	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);


	//VBO related activity
	//declare VAO, VBO 
	GLuint VAO, VBO, EBO, SSBO;
	GLushort indices[]= {0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8};
	

	//get unique name/ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind VAO first, then bind and set VBOs and then configure vertex attributes
	//bind VAO
	glBindVertexArray(VAO);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

	
	
	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
        GLint loc;
	char str[8];
	float sum = 0.0;
	srand((unsigned int)time(NULL));
	char *uniform_vars[] = {"VAR0", "VAR1", "VAR2", "VAR3"};
	for(int i=0; i<4; i++)
	{
		sprintf(str, "%s%d", "VAR", i);
		loc = glGetUniformLocation(shaderProgram, str);
        	if(loc != -1)
        	{
			float temp = (((float)rand()/(float)(RAND_MAX)));
                	glUniform1f(loc, temp);
			printf("temp = %f\n", temp);
			sum += temp;
                	printf("\nuniform location found\n");
        	}
        	else
                	printf("\nuniform location NOT found\n");

	}
	
	
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

	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);


	glfwSwapBuffers(window);
	/*	
	float shader_data;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
        GLvoid* p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), GL_MAP_READ_BIT);
        memcpy(&shader_data, p, sizeof(float));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        printf("Shader data = %f\n",(shader_data));
	printf("CPU sum data = %f\n",sum);
	*/
	do{
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
		glfwPollEvents();
		sleep(10);		
	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	
return 0;

	
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

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

// Fragment Shader
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

	//get unique name/ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	
	// Bind VAO first, then bind and set VBOs and then configure vertex attributes
	//bind VAO
	glBindVertexArray(VAO);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	//copy data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char*)(2 * sizeof(float))); 

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
		loc = glGetUniformLocation(shaderProgram, uniform_vars[i]);
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


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 24);


	glfwSwapBuffers(window);
	
	float shader_data;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
        GLvoid* p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float), GL_MAP_READ_BIT);
        memcpy(&shader_data, p, sizeof(float));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        printf("Shader data = %f\n",(shader_data));
	printf("CPU sum data = %f\n",sum);
	
	do{
		glfwPollEvents();
	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


return 0;

	
}

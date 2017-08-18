//Headers

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
   #include "stb_image.h"

// Vertex Shader
const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	ourColor = aColor;\n"	
	"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"	
	"}\n\0";

// Fragment Shader
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"

	"void main()\n"
	"{\n"
	"	FragColor = texture(ourTexture, TexCoord);\n"
	"}\n\0";
	


int main(int agrc, char **argv)
{
	//do windowing related stuff here

	if ( !glfwInit())
	{
		printf("Error: Failed to initialize GLFW\n");
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Simple_Texture", NULL, NULL);
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

	//Texture related stuff
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//set the texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load and generate the texture
	int width, height, nrChannels;
  	unsigned char *data = stbi_load("/home/dilip/Pictures/container.jpg", &width, &height, &nrChannels, 0);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}	
	else
	{
		printf("Error: Failed to load texture.\n");		
	}
	stbi_image_free(data);	


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



	//declare vertices
	GLfloat verts[] = 
	{
		//position		//color		//texture coords
		+0.5f, +0.5f, +0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		+0.5f, -0.5f, +0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, +0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,					
		-0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint indices[] = 
	{
		0, 1, 3,
		1, 2, 3
	};
	
	//VBO related activity
	//declare VAO, VBO 
	GLuint VAO, VBO, EBO;

	//get unique name/ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind VAO first, then bind and set VBOs and then configure vertex attributes
	//bind VAO
	glBindVertexArray(VAO);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//copy data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);	
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);

	do{
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);


        	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	        glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	return 0;
	
}

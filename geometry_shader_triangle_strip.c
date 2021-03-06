#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"\n"
	"\n"
	
	"out VS_OUT\n" 
	"{\n"
	"	vec3 color;\n"
	"} vs_out;\n"
	
	
	"void main()\n"
	"{\n"
	"       vs_out.color = aColor;\n"
	"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"		
	"}\n\0";

// Fragment Shader
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"in vec3 fColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(fColor, 1.0f);\n"	
	"}\n\0";

// Geometry Shader
const char *geometryShaderSource = 
	"#version 330 core\n"
	"layout (points) in;\n"
	"layout (triangle_strip, max_vertices = 5) out;\n"
	"in VS_OUT\n" 
	"{\n"
	"	vec3 color;\n"
	"} gs_in[];\n"
	
	"out vec3 fColor;\n"
	"void build_house(vec4 position)"
	"{"
	"	fColor = gs_in[0].color;\n"
	"	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);\n"
	"	EmitVertex();\n"
	"	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);\n"
        "       EmitVertex();\n"
	"	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);\n"
        "       EmitVertex();\n"
	"	gl_Position = position + vec4( 0.2, 0.2, 0.0, 0.0);\n"
        "       EmitVertex();\n"
	"	gl_Position = position + vec4( 0.0, 0.4, 0.0, 0.0);\n"
	"       fColor = vec3(1.0, 1.0, 1.0);\n"
        "       EmitVertex();\n"
	"	EndPrimitive();\n"
	"}\n"
	
	"void main()\n"
	"{\n"
	"	build_house(gl_in[0].gl_Position);\n"
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
	float points[] = 
	{
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);

	glfwSwapBuffers(window);

	do{
		glfwPollEvents();
	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
return 0;

	
}

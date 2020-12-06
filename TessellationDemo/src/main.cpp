#include <iostream>

#include "Utils.h"
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


/*
* GLOBALS
*/
GLuint g_Vertices, g_Indices;
GLuint g_Program, g_TempProgram;

static void ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool CreateProgram(GLuint& program)
{
	GLuint vs, fs, tesc, tese;
	if (!LoadShader("assets/shaders/tessellation.vert", GL_VERTEX_SHADER, vs)) return false;
	if (!LoadShader("assets/shaders/tessellation.frag", GL_FRAGMENT_SHADER, fs)) return false;
	if (!LoadShader("assets/shaders/tessellation.tesc", GL_TESS_CONTROL_SHADER, tesc)) return false;
	if (!LoadShader("assets/shaders/tessellation.tese", GL_TESS_EVALUATION_SHADER, tese)) return false;

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glAttachShader(program, tesc);
	glAttachShader(program, tese);

	glLinkProgram(program);
	GLint linked = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint size = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);

		if (size != 0)
		{
			char* errorMsg = new char[size];
			glGetProgramInfoLog(program, size, NULL, errorMsg);
			LOG_ERROR("Failed to link: %s", errorMsg);

			delete[] errorMsg;
		}
		glDeleteProgram(program);

		return false;
	}

	return true;
}

bool Init()
{
	const glm::vec3 color(0.0f, 1.0f, 0.0f);
	SVertex vertices[4] =
	{
		{ {-0.5f, 0.5f, 0.0f},	color},
		{ {-0.5f, -0.5f, 0.0f}, color},
		{ {0.5f, -0.5f, 0.0f},	color},
		{ {0.5f, 0.5f, 0.0f},	color}
	};
	glGenBuffers(1, &g_Vertices);
	glBindBuffer(GL_ARRAY_BUFFER, g_Vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * 4, vertices, GL_STATIC_DRAW);

	uint32_t indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};
	glGenBuffers(1, &g_Indices);
	glBindBuffer(GL_ARRAY_BUFFER, g_Indices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

	if (!CreateProgram(g_Program))
	{
		LOG_ERROR("Failed to create program!");
		return false;
	}

	return true;
}

void ReleaseProgram()
{
	glDeleteProgram(g_Program);
}

void Release()
{
	ReleaseProgram();
	glDeleteBuffers(1, &g_Vertices);
	glDeleteBuffers(1, &g_Indices);
	glfwTerminate();
}

void Render()
{

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_Program);

	glBindBuffer(GL_ARRAY_BUFFER, g_Vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Indices);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SVertex),
		0
	);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(SVertex),
		(void*)offsetof(SVertex, SVertex::Color)
	);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		LOG_INFO("Rebuilding Program");
		if (CreateProgram(g_TempProgram))
		{
			ReleaseProgram();
			g_Program = g_TempProgram;
			LOG_INFO("Succeded Rebuilding Program");
		}
		else
		{
			LOG_ERROR("Failed to Rebuild Program!");
		}
	}
}

int main(void) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GLFWwindow* window;

	if (!glfwInit())
	{
		LOG_ERROR("Failed to init GLFW");
		return -1;
	}

	window = glfwCreateWindow(640, 480, "TessellationDemo", NULL, NULL);
	if (!window)
	{
		LOG_ERROR("Failed to init window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LOG_ERROR("Failed to init GLEW: %s", glewGetErrorString(err));
		return -1;
	}

	if (!Init())
	{
		LOG_ERROR("Failed to init resources");
		return false;
	}


	glfwSetKeyCallback(window, KeyCallback);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Render();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	Release();

	glfwTerminate();
	return 0;
}
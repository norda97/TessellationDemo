#include <iostream>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Utils.h"
#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


/*
* GLOBALS
*/
GLuint g_Vertices, g_Indices;
GLuint g_UniformSettings, g_UniformSettingsBlockIndex;
GLuint g_Program, g_TempProgram;

std::string g_TessSpacing;

Utils::SUniformSettings g_Settings = {};

static void ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool CreateProgram(
	GLuint& program,
	GLuint vs = GL_FALSE,
	GLuint fs = GL_FALSE,
	GLuint tesc = GL_FALSE,
	GLuint tese = GL_FALSE)
{
	using namespace Utils;

	program = glCreateProgram();
	if (vs != GL_FALSE) glAttachShader(program, vs);
	if (fs != GL_FALSE) glAttachShader(program, fs);
	if (tesc != GL_FALSE) glAttachShader(program, tesc);
	if (tese != GL_FALSE) glAttachShader(program, tese);

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
	using namespace Utils;
	// Create Buffers
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
	}

	GLuint vs, fs, tesc, tese;
	if (!LoadShader("assets/shaders/tessellation.vert", GL_VERTEX_SHADER, vs, nullptr)) return false;
	if (!LoadShader("assets/shaders/tessellation.frag", GL_FRAGMENT_SHADER, fs, nullptr)) return false;
	if (!LoadShader("assets/shaders/tessellation.tesc", GL_TESS_CONTROL_SHADER, tesc, nullptr)) return false;
	
	{
		g_TessSpacing = "equal_spacing";
		std::string teseLayout = "layout(triangles, " + g_TessSpacing + ", ccw) in;\n";
		std::vector<const GLchar*> sources = { "#version 420\n", teseLayout.c_str() };
		if (!LoadShader("assets/shaders/tessellation.tese", GL_TESS_EVALUATION_SHADER, tese, &sources)) return false;
	}

	if (!CreateProgram(g_Program, vs, fs, tesc, tese))
	{
		LOG_ERROR("Failed to create program!");
		return false;
	}

	// Create Uniform
	{
		glGenBuffers(1, &g_UniformSettings);
		glBindBuffer(GL_UNIFORM_BUFFER, g_UniformSettings);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SUniformSettings), &g_Settings, GL_DYNAMIC_DRAW);

		//GLuint uniformBlockIndex = glGetUniformBlockIndex(g_Program, "Settings");
		//glUniformBlockBinding(g_Program, uniformBlockIndex, 0);
	}
	return true;
}

void ReleaseProgram()
{
	glDeleteProgram(g_Program);
}

void RebuildProgram()
{
	using namespace Utils;

	GLuint vs, fs, tesc, tese;
	if (!LoadShader("assets/shaders/tessellation.vert", GL_VERTEX_SHADER, vs, nullptr));
	if (!LoadShader("assets/shaders/tessellation.frag", GL_FRAGMENT_SHADER, fs, nullptr));
	if (!LoadShader("assets/shaders/tessellation.tesc", GL_TESS_CONTROL_SHADER, tesc, nullptr));
	std::string teseLayout = "layout(triangles, " + g_TessSpacing + ", ccw) in;\n";
	std::vector<const GLchar*> sources = { "#version 420\n", teseLayout.c_str() };
	if (!LoadShader("assets/shaders/tessellation.tese", GL_TESS_EVALUATION_SHADER, tese, &sources));

	if (CreateProgram(g_TempProgram, vs, fs, tesc, tese))
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

void Release()
{
	ReleaseProgram();
	glDeleteBuffers(1, &g_Vertices);
	glDeleteBuffers(1, &g_Indices);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Render()
{
	using namespace Utils;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_Program);

	glBindBuffer(GL_ARRAY_BUFFER, g_Vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_Indices);
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, g_UniformSettings, 0, sizeof(SUniformSettings));

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
		RebuildProgram();
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

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1280, 720, "TessellationDemo", NULL, NULL);
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


	// Init ImGui
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 420");

	}

	glfwSetKeyCallback(window, KeyCallback);

	bool show_demo_window = false;
	bool show_another_window = false;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Start the Dear ImGui frame
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Tessellation Settings");                    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

				ImGui::Text("This is some useful text.");             
				ImGui::Checkbox("Demo Window", &show_demo_window);     
				ImGui::Checkbox("Another Window", &show_another_window);

				static glm::vec3 scale = glm::vec3(1.0f);
				ImGui::InputFloat3("Scale", &scale[0]);
				g_Settings.ModelMatrix = glm::scale(glm::mat4(1.0f), scale);

				static int style_idx = 0;
				if (ImGui::Combo("Tesselation Spacing", &style_idx, "equal_spacing\0fractional_even_spacing\0fractional_odd_spacing\0"))
				{
					switch (style_idx)
					{
					case 0: g_TessSpacing = "equal_spacing"; break;
					case 1: g_TessSpacing = "fractional_even_spacing"; break;
					case 2: g_TessSpacing = "fractional_odd_spacing"; break;
					}

					LOG_INFO("Rebuilding Program");
					RebuildProgram();
				}

				static glm::ivec4 tessLevels = glm::ivec4(1);
				ImGui::SliderInt("TessLevelInner", &tessLevels.x, 1, 32);
				ImGui::SliderInt("TessLevelOuter0", &tessLevels.y, 1, 32);
				ImGui::SliderInt("TessLevelOuter1", &tessLevels.z, 1, 32);
				ImGui::SliderInt("TessLevelOuter2", &tessLevels.w, 1, 32);

				g_Settings.TessLevels.x = (float)tessLevels.x;
				g_Settings.TessLevels.y = (float)tessLevels.y;
				g_Settings.TessLevels.z = (float)tessLevels.z;
				g_Settings.TessLevels.w = (float)tessLevels.w;

				if (ImGui::Button("Button"))
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::End();
			}
		}

		glBindBuffer(GL_UNIFORM_BUFFER, g_UniformSettings);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Utils::SUniformSettings), &g_Settings);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		Render();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	Release();

	glfwTerminate();
	return 0;
}
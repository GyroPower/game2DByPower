#include<glad/gl.h>
#include<GLFW/glfw3.h>
#include<ImGui/imgui.h>
#include<ImGui/backends/imgui_impl_glfw.h>
#include<ImGui/backends/imgui_impl_opengl3.h>
#include<glm/glm.hpp>
#include<iostream>
#include"sandBox/sandBox.h"
#include"Texture/texture.h"

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

SandBox sandbox;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

// This project needs to be document better, there is alot to explain and give a hint of for what is use
// something, i will extend this more.
// but the goal of the project is to give an interface to quickly developt a 2d game with classes 
// for the entities and the renderer and tile maps to create levels, it needs to be more extended and 
// change somethings for be more flexible 


int main() {

	if (!glfwInit())
	{
		std::cout << "Error initialize glfw\n";
		return -1;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "game2DByPower", nullptr, nullptr);


	if (window == nullptr)
		return -1;

	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Error to initialize GLAD\n";
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::vec4 clear_color(0.45f, 0.55f, 0.60f, 1.0f);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	/*io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;*/

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420 core");

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	
	sandbox.initSandBox(WINDOW_WIDTH, WINDOW_HEIGHT);

	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		// the spot of workflow to call the gui function to create the gui

		sandbox.renderGUI();

		//end of the spot, if we try to call a function of imgui to create a button or something, it will
		//raise an exception
		ImGui::Render();
		sandbox.processInput(deltaTime);
		sandbox.update(deltaTime);
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		sandbox.renderScene();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS)
			sandbox.keys[key] = true;
		else if (action == GLFW_RELEASE) {
			sandbox.keys[key] = false;
			sandbox.keyProcessed[key] = false;
		}

	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	sandbox.setCameraScroll((float)yoffset);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	sandbox.GetMousePos(glm::vec2((float)xpos, (float)ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
		sandbox.keys[button] = true;
	else if (action == GLFW_RELEASE)
	{
		sandbox.keys[button] = false;
		sandbox.keyProcessed[button] = false;
	}
}
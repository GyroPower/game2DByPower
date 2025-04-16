#include<glad/gl.h>
#include<GLFW/glfw3.h>
#include<backend/imgui_impl_glfw.h>
#include<backend/imgui_impl_opengl3.h>
#include<imgui.h>
#include<iostream>
#include"sandBox/sandBox.h"

SandBox sandbox;

void ResizeWindowCallback(GLFWwindow*, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

int main()
{
	if (!glfwInit())
	{
		std::cout << "Error initialize glfw\n";
		return -1;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "game2DbyPower", nullptr, nullptr);


	if (window == nullptr)
		return -1;

	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Error to initialize GLAD\n";
		return -1;
	}
	glfwSetWindowSizeCallback(window, ResizeWindowCallback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	ImVec4 clearColor(1.0f, 0.5f, 0.5f, 1.0f);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420 core");

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	
	sandbox.initSandBox(WINDOW_WIDTH, WINDOW_HEIGHT);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
        //Render space to call the method or functions to render the GUI
		sandbox.renderGUI();
		ImGui::Render();

		sandbox.processInput(deltaTime);
		sandbox.update(deltaTime);

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
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
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(button, action);

	if (!io.WantCaptureMouse)
	{
		if (action == GLFW_PRESS)
			sandbox.keys[button] = true;
		else if (action == GLFW_RELEASE)
		{
			sandbox.keys[button] = false;
			sandbox.keyProcessed[button] = false;
		}
	}
}

void ResizeWindowCallback(GLFWwindow*, int width, int height)
{
	sandbox.m_resizeCamera(width, height);
}
#pragma once
#include "App.h"

int Window::width = 800;
int Window::height = 600;

Window::Window(int width, int height, const char* title, bool fullscreen, bool vsync)
	: fullscreen(fullscreen), VSync(vsync) {
	glfwSetErrorCallback(error_callback);

	// https://www.glfw.org/documentation.html
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		std::exit(-1);
	}
	if (fullscreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		window = glfwCreateWindow(mode->width, mode->height, title, monitor, nullptr);
	}
	else {
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	auto this_inst = static_cast<Window*>(glfwGetWindowUserPointer(window));
	this_inst->width = width;
	this_inst->height = height;

	// set viewport
	glViewport(0, 0, width, height);
	//now your canvas has [0,0] in bottom left corner, and its size is [width x height] 

	this_inst->update_projection_matrix();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->handle_key_event(key, action);
	}
}
void Window::handle_key_event(int key, int action) {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_F:
			fullscreen = !fullscreen;
			set_fullscreen(fullscreen);
			break;
		case GLFW_KEY_V:
			VSync = !VSync;
			set_vsync(VSync);
			break;
		default:
			break;
		}
	}
}
void Window::window_iconify_callback(GLFWwindow* window, int iconified) {
	if (iconified) {
		std::cout << "Window was iconified." << std::endl;
	}
	else {
		std::cout << "Window was opened." << std::endl;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	auto this_inst = static_cast<Window*>(glfwGetWindowUserPointer(window));
	this_inst->FOV += 10 * yoffset;
	this_inst->FOV = std::clamp(this_inst->FOV, 20.0f, 170.0f); // limit FOV to reasonable values...

	this_inst->update_projection_matrix();
}

void Window::update_projection_matrix(void)
{
	if (height < 1)
		height = 1;   // avoid division by 0

	float ratio = static_cast<float>(width) / height;

	projection_matrix = glm::perspective(
		glm::radians(FOV),   // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		ratio,               // Aspect Ratio. Depends on the size of your window.
		0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		20000.0f             // Far clipping plane. Keep as little as possible.
	);
}

void App::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		camera.ProcessMouseMovement(static_cast<GLfloat>(last_cursor_xpos - xpos), static_cast<GLfloat>(ypos - last_cursor_ypos));
	}
	last_cursor_xpos = xpos;
	last_cursor_ypos = ypos;
}

void Window::set_vsync(bool state) {
	this->VSync = state;
	if (state) {
		glfwSwapInterval(1);
		std::cout << "Vsync On" << std::endl;
		return;
	}

	glfwSwapInterval(0);
	std::cout << "Vsync Off" << std::endl;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_1:
			std::cout << "<-" << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_2:
			std::cout << "->" << std::endl;
			break;
		default:
			break;
		}
	}
}

void Window::error_callback(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
}

bool Window::isVSynced() const
{
	return VSync;
}

bool Window::isFullscreen() const {
	return fullscreen;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

GLFWmonitor* Window::getMonitor() {
	return monitor;
}

const GLFWvidmode* Window::getMode() {
	return mode;
}

GLFWwindow* Window::getWindow() const
{
	return window;
}

void Window::set_fullscreen(bool fullscreen) {
	this->fullscreen = fullscreen;

	if (fullscreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE); //GLFW_DONT_CARE seems to be the issue
		std::cout << "Fullscreen" << std::endl;
	}
	else {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, nullptr, 150, 150, mode->width / 2, mode->height / 2, GLFW_DONT_CARE);
		std::cout << "Windowed" << std::endl;
	}
}

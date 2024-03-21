#include "Window.h"

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
	glfwSetCursorPosCallback(window, cursor_pos_callback);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	std::cout << "Current size: " << width << "x" << height << std::endl;
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
	if (yoffset > 0.0) {
		std::cout << "tocis dolu...\n";
	}
	if (yoffset < 0.0) {
		std::cout << "tocis nahoru...\n";
	}
	if (xoffset > 0.0) {
		std::cout << "tocis doprava...\n";
	}
	if (xoffset < 0.0) {
		std::cout << "tocis doleva...\n";
	}
}


void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	std::cout << "?" << std::ends;
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

bool Window::getVSyncState() const {
	return VSync;
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
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
		std::cout << "Fullscreen" << std::endl;
	}
	else {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, nullptr, 150, 150, mode->width / 2, mode->height / 2, GLFW_DONT_CARE);
		std::cout << "Windowed" << std::endl;
	}
}

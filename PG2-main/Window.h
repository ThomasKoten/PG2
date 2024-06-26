#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window
{
public:
	Window(int width, int height, const char* title, bool fullscreen = false, bool vsync = true);
	~Window();

	void set_fullscreen(bool fullscreen);
	void set_vsync(bool vsync);

	void handle_key_event(int key, int action);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void window_iconify_callback(GLFWwindow* window, int iconified);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	bool isVSynced() const { return VSync; }
	bool isFullscreen() const { return fullscreen; }

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	GLFWmonitor* getMonitor() const { return monitor; }
	const GLFWvidmode* getMode() const { return mode; }
	GLFWwindow* getWindow() const { return window; }

	glm::mat4 getProjection() const { return projection_matrix; }

	void update_projection_matrix(void);


private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);
	GLFWwindow* window = nullptr;
	bool VSync;
	bool fullscreen;

	GLFWmonitor* monitor{};
	const GLFWvidmode* mode{};

	static int width;
	static int height;
	float FOV = 89.0f;
	glm::mat4 projection_matrix = glm::identity<glm::mat4>();

};

#endif


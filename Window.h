#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>
class Window
{
public:
	Window(int width, int height, const char* title, bool fullscreen = false, bool vsync = false);
	~Window();

	void set_fullscreen(bool fullscreen);
	void set_vsync(bool vsync);

	//void handle_scroll_event(double xoffset, double yoffset);
	void handle_key_event(int key, int action);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void window_iconify_callback(GLFWwindow* window, int iconified);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	bool isVSynced() const;
	bool isFullscreen() const;
	bool getVSyncState() const;

	GLFWmonitor* getMonitor();
	const GLFWvidmode* getMode();
	GLFWwindow* getWindow() const;

private:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void error_callback(int error, const char* description);
	GLFWwindow* window = nullptr;
	bool VSync;
	bool fullscreen;

	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
};

#endif


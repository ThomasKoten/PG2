#pragma once
#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <string>
#include <unordered_map>

// OpenGL Extension Wrangler : allow all multiplatform GL functions
#include <GL/glew.h> 
// WGLEW = Windows GL Extension Wrangler (change for different platform) 
// platform specific functions (in this case Windows)
#include <GL/wglew.h> 

// GLFW toolkit
// Uses GL calls to open GL context, i.e. GLEW must be first.
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "ShaderProgram.h"
#include "Model.h"
#include "Mesh.h"
#include "gl_err_callback.h"

class App
{
public:
	App();
	void glfw_init();
	void glfw_register_callback();
	void glew_init();
	void glew_register_callback();

	void report();
	void init_assets();
	bool init(void);
	int run(void);
	bool getVSyncState() const {
		return VSyncOn;
	}
	void setVSyncState(bool state) {
		VSyncOn = state;
	}

	GLFWmonitor* getMonitor() {
		return monitor;
	}
	const GLFWvidmode* getMode() {
		return mode;
	}
	
	
	bool isFullscreen() {
		return glfwGetWindowMonitor(window) != nullptr;
	}
	~App();

private:
	static void error_callback(int error, const char* description);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool VSyncOn = false;

	std::unordered_map<std::string, Model> scene;
	ShaderProgram shader;

	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
	GLFWwindow* window = nullptr;
};


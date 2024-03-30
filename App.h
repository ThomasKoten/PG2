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


#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include "gl_err_callback.h"
#include "Window.h"
#include "Camera.h"

class App
{
public:
	App();
	void glfw_init();
	void glfw_register_callback();
	void glew_init();
	void glew_register_callback();

	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	void report();
	void init_assets();
	bool init(void);
	int run(void);

	~App();


private:
	Window* window;

	std::vector<Model> scene_test;
	Shader shader;

	std::unordered_map<std::string, Mesh> scene;

	static Camera camera;
	static double last_cursor_xpos;
	static double last_cursor_ypos;


};


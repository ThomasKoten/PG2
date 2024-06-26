#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> 
// GLFW toolkit
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
	//void glfw_init();
	//void glfw_register_callback();
	//void glew_init();
	//void glew_register_callback();

	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

	std::vector<glm::vec3> moveLight(std::vector<glm::vec3>& position, int index, float time, float speed);
	void flyingBird(Model& bird, float time, float radius, float speed);
	void rollBall(Model& ball, float time, float initialX, float amplitude, float speed);
	void jump(Model& model, float initialY, float gravity, float jumpSpeed);

	void report();
	void init_assets();
	bool init();
	int run();

	~App();

private:
	Window* window;
	Shader shader;

	std::unordered_map<std::string, Mesh> scene;

	static Camera camera;
	float camera_height = 10.0;
	static double last_cursor_xpos;
	static double last_cursor_ypos;

	std::vector<Model> scene_opaque;
	std::vector<Model> scene_transparent;
	std::vector<Model> scene_heightmap;
	float verticalVelocity = 5.0f;
	float spinDegree = 1.0f;
};
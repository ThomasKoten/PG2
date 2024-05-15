﻿#pragma once //Light a material definovat v Shader i App
#include "App.h"
#include "Model.h"
#include "Shader.h"
#include <opencv2\opencv.hpp>
#include <cmath>

void getString(const std::string name, GLenum symb_const) {
	const char* mystring = (const char*)glGetString(symb_const);
	std::cout << name << ": " << mystring << '\n';
}

void getInt(const std::string& name, GLenum pname) {
	GLint myInt;
	glGetIntegerv(pname, &myInt);
	std::cout << name << ": " << myInt << '\n';
}

Camera App::camera = Camera(glm::vec3(0, 0, 1000));
double App::last_cursor_xpos{};
double App::last_cursor_ypos{};


App::App()
{
	// default constructor
	// nothing to do here (so far...)
	std::cout << "Constructed...\n";
	window = new Window(800, 600, "OpenGL Window");
}

bool App::init()
{
	try {
		glfwWindowHint(GLFW_SAMPLES, 1);
		glfwSetCursorPosCallback(window->getWindow(), cursor_pos_callback);

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//glfwSwapInterval(1);


		// init glew
		// http://glew.sourceforge.net/basic.html
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			//glewInit failed, something is wrong
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

		wglewInit();

		if (GLEW_ARB_debug_output)
		{
			glDebugMessageCallback(MessageCallback, 0);
			glEnable(GL_DEBUG_OUTPUT);

			//default is asynchronous debug output, use this to simulate glGetError() functionality
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			std::cout << "GL_DEBUG enabled." << std::endl;
		}
		else
			std::cout << "GL_DEBUG NOT SUPPORTED!" << std::endl;

		init_assets();
	}

	catch (std::exception const& e) {
		std::cerr << "Init failed : " << e.what() << "\n";
		//throw;
		exit(-1);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::cout << "Initialized...\n";
	return true;
}

int App::run(void)
{
	double previousTime = glfwGetTime();
	int nbFrames = 0;
	double elapsedTime;

	glm::vec3 rgb_white = { 1.0f, 1.0f, 1.0f };
	glm::vec4 rgba_white = { 1.0f, 1.0f, 1.0f, 1.0f };

	std::vector<glm::vec3> light_positions = {
		glm::vec3(0, 0, 0), //R->L, U->D, F->B
		glm::vec3(0, 0, 0), //R->L, U->D, F->B
	};

	std::vector<glm::vec3> light_colors = {
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};


	try {
		window->update_projection_matrix();
		glViewport(0, 0, window->getWidth(), window->getHeight());

		camera.Position = glm::vec3(0, 270, 0);
		double last_frame_time = glfwGetTime();
		glm::vec3 camera_movement{};

		glm::vec4 green = { 0.0f, 0.5f, 0.0f, 1.0f };
		while (!glfwWindowShouldClose(window->getWindow()))
		{
			double currentTime = glfwGetTime();
			nbFrames++;
			elapsedTime = currentTime - previousTime;
			if (elapsedTime >= 1.0) { // If last prinf() was more than 1 sec ago
				previousTime = currentTime;
				// printf and reset timer
				std::cout << (double(nbFrames) / elapsedTime) << std::endl;
				nbFrames = 0;
			}

			// Clear OpenGL canvas, both color buffer and Z-buffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			double delta_t = glfwGetTime() - last_frame_time;
			last_frame_time = glfwGetTime();

			camera_movement = camera.ProcessInput(window->getWindow(), static_cast<float>(delta_t));
			camera.Position += camera_movement;
			float terrain_height = scene_heightmap[0].GetHeightAtPosition(camera.Position.x, camera.Position.z);
			camera.Position.y = terrain_height + camera_height; //Zakomentovat pro "no_clip"

			shader.activate();
			glm::mat4 view = glm::mat4(1.0f);
			view = camera.GetViewMatrix();

			//glm::mat4 projection = glm::mat4(1.0f);
			//projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

			shader.setUniform("projection", window->getProjection());
			shader.setUniform("view", view);
			shader.setUniform("ambient_material", rgb_white);
			shader.setUniform("diffuse_material", { 1.0f ,1.0f ,1.0f });
			shader.setUniform("specular_material", { 0.7f,0.7f ,0.7f });
			shader.setUniform("specular_shininess", 1.0f);
			shader.setUniform("num_lights", 2);
			shader.setUniformArray("light_positions", light_positions);
			shader.setUniformArray("light_colors", light_colors);
			/*
			uniform int num_lights;
			uniform vec3 light_positions[MAX_LIGHTS];
			uniform vec3 light_colors[MAX_LIGHTS];
			*/

			for (auto& model : scene_heightmap) {
				shader.setUniform("transform", model.getTransMatrix());
				model.Draw(shader);
			}

			double time = glfwGetTime();
			float radius = 100.0f;
			float speed = 2.0f;

			flyingBird(scene_opaque[1], time, radius, speed);

			for (auto& model : scene_opaque) {
				shader.setUniform("transform", model.getTransMatrix());
				model.Draw(shader);
			}
			// - Draw transparent objects
			glEnable(GL_BLEND);         // enable blending
			glDisable(GL_CULL_FACE);    // no polygon removal
			glDepthMask(GL_FALSE);      // set Z to read-only


			shader.setUniform("ambient_material", rgb_white);
			shader.setUniform("diffuse_material", rgb_white);
			shader.setUniform("specular_material", rgb_white);
			shader.setUniform("specular_shininess", 1.0f);
			// TODO: sort by distance from camera, from far to near
			for (auto& model : scene_transparent) {
				shader.setUniform("transform", model.getTransMatrix());
				model.Draw(shader);
			}
			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);


			// Swap front and back buffers
			glfwSwapBuffers(window->getWindow());

			// Poll for and process events
			glfwPollEvents();
		}
	}
	catch (std::exception const& e) {
		std::cerr << "App failed : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Finished OK...\n";
	return EXIT_SUCCESS;
}

App::~App()
{
	exit(EXIT_SUCCESS);
	std::cout << "Bye...\n";
}



void App::init_assets()
{
	// load models, load textures, load shaders, initialize level, etc...
	shader = Shader(VS_PATH, FS_PATH);

	//scene_test.push_back(my_model);
	scene_opaque.push_back(Model("./resources/obj/cube_tri_vnt.obj", "./resources/textures/box_rgb888.png", { 5.0f, 260.0f, -3.0f }));
	scene_opaque.push_back(Model("./resources/obj/condor.obj", "./resources/textures/condor.tif", { 0.0f, 260.0f, 0.0f }, 30.0f));
	scene_transparent.push_back(Model("./resources/obj/bunny_tri_vnt.obj", "./resources/textures/Glass.png", { 2.0f, 240.0f, 8.0f }, 1.0f, { 1.0f, 0.0f, 0.0f, -20.0f }));

	// == HEIGHTMAP ==
	std::filesystem::path heightspath("./resources/textures/heights.png");
	std::filesystem::path texturepath("./resources/textures/tex_256.png");
	auto model = Model(heightspath, texturepath, { 0.0f,0.0f, 0.0f }, 1.0f, { 0.0f, 1.0f, 0.0f, 0.0f }, true);
	scene_heightmap.push_back(model);
}

void App::report() {
	GLint contextProfile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfile);
	getString("Version", GL_VERSION);
	getString("Vendor", GL_VENDOR);
	getString("Renderer", GL_RENDERER);
	getString("Shading version", GL_SHADING_LANGUAGE_VERSION);
	getInt("Context profile mask", GL_CONTEXT_PROFILE_MASK);

	if (contextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
		std::cout << "Context is core profile" << '\n';
	else if (contextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		std::cout << "Context is compatibility profile." << '\n';
	else
		std::cout << "Context has no profile." << '\n';

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
}

void App::flyingBird(Model& bird, float time, float radius, float speed) {
	float x = radius * cos(speed * time);
	float z = radius * sin(speed * time);

	glm::vec3 direction = glm::normalize(glm::vec3(speed * x, 0.0f, -speed * z));

	// Calculate orientation angle
	float angle = atan2(direction.z, direction.x);
	float angle_deg = glm::degrees(angle);

	// Update model orientation
	bird.rotation.w = angle_deg;
	//Update model position
	bird.position = glm::vec3(x, bird.position.y, z);

}


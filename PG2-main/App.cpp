﻿#include "App.h"
#include <filesystem>

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
	std::cout << "Constructed...\n";
	window = new Window(800, 600, "OpenGL Window", false, false);
}

bool App::init()
{
	try {
		glfwWindowHint(GLFW_SAMPLES, 1);
		glfwSetCursorPosCallback(window->getWindow(), cursor_pos_callback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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

	glm::vec3 rgb_white = { 0.2f, 0.2f, 0.2f };
	glm::vec4 rgba_white = { 1.0f, 1.0f, 1.0f, 1.0f };


	//POINT LIGHT SOURCE
	std::vector<glm::vec3> PL_pos = {
		glm::vec3(0,-1,0), //R->L, U->D, F->B
		//glm::vec3(0, 0, 0), //R->L, U->D, F->B
	};
	std::vector<glm::vec3> PL_col = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		//glm::vec3(0.0f, 0.0f, 1.0f),
	};
	std::vector<float> PL_constants = { 1.0f };
	std::vector<float> PL_linears = { 0.01f };
	std::vector<float> PL_quadratics = { 0.0f };


	//DIRECTIONAL LIGHT SOURCE
	std::vector<glm::vec3> DL_directions = {
		glm::vec3(-0.5f, 1.0f, -1.0f)  // Direction vector
	};
	std::vector<glm::vec3> DL_colors = {
		glm::vec3(0.4f, 0.4f, 0.4f)    // Color (white)
	};

	//SPOTLIGHT SOURCE
	std::vector<glm::vec3> SL_positions = {
	glm::vec3(0.0f, 10.0f, -275.0f)   // Position
	};
	std::vector<glm::vec3> SL_directions = {
		glm::vec3(0.0f, -1.0f, 0.0f)   // Direction
	};
	std::vector<glm::vec3> SL_colors = {
		glm::vec3(1.0f, 0.0f, 0.0f)    // Color (red)
	};
	std::vector<float> SL_cutoffs = {
		glm::cos(glm::radians(12.5f))  // Cutoff angle (in radians)
	};

	try {
		window->update_projection_matrix();
		glViewport(0, 0, window->getWidth(), window->getHeight());

		camera.Position = glm::vec3(0, 270, 30);
		double last_frame_time = glfwGetTime();
		glm::vec3 camera_movement{};

		glm::vec4 green = { 0.0f, 0.5f, 0.0f, 1.0f };
		while (!glfwWindowShouldClose(window->getWindow()))
		{
			glfwPollEvents();
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

			shader.setUniform("projection", window->getProjection());
			shader.setUniform("view", view);
			shader.setUniform("ambient_material", rgb_white);
			shader.setUniform("diffuse_material", { 1.0f ,1.0f ,1.0f });
			shader.setUniform("specular_material", { 0.7f,0.7f ,0.7f });
			shader.setUniform("specular_shininess", 1.0f);
			//POINT
			shader.setUniform("num_point_lights", 1);
			shader.setUniformArray("point_light_positions", PL_pos);
			shader.setUniformArray("point_light_colors", PL_col);
			shader.setUniformArray("point_light_constants", PL_constants);
			shader.setUniformArray("point_light_linears", PL_linears);
			shader.setUniformArray("point_light_quadratics", PL_quadratics);
			//DIR
			shader.setUniform("num_directional_lights", 1);
			shader.setUniformArray("directional_light_directions", DL_directions);
			shader.setUniformArray("directional_light_colors", DL_colors);
			//SPOT
			shader.setUniform("num_spot_lights", 1);
			shader.setUniformArray("spot_light_positions", moveLight(SL_positions, 0, 1.0f, last_frame_time));
			shader.setUniformArray("spot_light_directions", SL_directions);
			shader.setUniformArray("spot_light_colors", SL_colors);
			shader.setUniformArray("spot_light_cutoffs", SL_cutoffs);

			for (auto& model : scene_heightmap) {
				shader.setUniform("transform", model.getTransMatrix());
				model.Draw(shader);
			}

			float radius = 100.0f;
			float speed = 2.0f;

			float amplitude = 10.0f;
			float centerX = 30.0f;

			float jumpSpeed = 0.008;
			float gravity = 2.8f;


			flyingBird(scene_opaque[0], last_frame_time, radius, speed);
			rollBall(scene_opaque[1], last_frame_time, centerX, amplitude, speed);
			jump(scene_transparent[0], 240.0f, gravity, jumpSpeed);


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

			for (auto& model : scene_transparent) {
				shader.setUniform("transform", model.getTransMatrix());
				model.Draw(shader);
			}
			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);


			// Swap front and back buffers
			glfwSwapBuffers(window->getWindow());
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

	scene_opaque.push_back(Model("./resources/objects/condor.obj", "./resources/materials/condor.mtl", { 0.0f, 260.0f, 0.0f }, 30.0f));
	scene_opaque.push_back(Model("./resources/objects/soccer_ball.obj", "./resources/materials/soccer_ball.mtl", { 20.0f, 248.0f, 30.0f }, 0.2f, { 0.0f, 0.0f, 1.0f, 0.0f }));
	scene_transparent.push_back(Model("./resources/objects/bunny_tri_vnt.obj", "./resources/textures/Glass.png", { 2.0f, 240.0f, 8.0f }, 1.0f, { 1.0f, 0.0f, 0.0f, -20.0f }));

	// == HEIGHTMAP ==
	std::filesystem::path heightspath("./resources/textures/heights.png");
	std::filesystem::path texturepath("./resources/textures/tex_256.png");
	scene_heightmap.push_back(Model(heightspath, texturepath, { 0.0f, 0.0f, 0.0f }, 1.0f, { 0.0f, 1.0f, 0.0f, 0.0f }, true));
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


	bird.rotation.w = angle_deg; // Update model orientation
	bird.position = glm::vec3(x, bird.position.y, z); //Update model position

}

void App::rollBall(Model& ball, float time, float centerX, float amplitude, float speed) {
	float x = amplitude * sin(speed * time) + centerX;

	float distanceTraveled = centerX - x;
	float rotationAngle = distanceTraveled * 360.0f;


	ball.position.x = x; // Update ball position
	ball.rotation.w = glm::radians(rotationAngle); // Update ball rotation
}

void App::jump(Model& model, float initialY, float gravity, float jumpSpeed) {
	model.position.y += verticalVelocity * jumpSpeed; // Update vertical position
	spinDegree += 0.5;
	model.rotation = { 0.0f, 1.0f, 0.0f, spinDegree };
	if (model.position.y >= initialY) {
		verticalVelocity -= gravity * jumpSpeed; // Apply gravity
	}
	else {
		verticalVelocity = 10.0;
	}
}

std::vector<glm::vec3>  App::moveLight(std::vector<glm::vec3>& position, int index, float time, float speed) {
	float yOffset = 50.0f * sin(speed * time);

	// Update the light position
	position[0].y = 10 + yOffset;
	position[0].z += 0.01;

	return position;
}




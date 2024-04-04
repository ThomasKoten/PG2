#pragma once //Light a material definovat v Shader i App
#include "App.h"
#include "Model.h"
#include "Shader.h"

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

	std::cout << "Initialized...\n";
	return true;
}

int App::run(void)
{
	double previousTime = glfwGetTime();
	int nbFrames = 0;
	double elapsedTime;
	glm::vec3 rgb_orange = { 0.208f, 0.157f, 0.118f };
	//glm::vec3 rgb_orange = { 0.255f, 0.098f, 0.0f };
	glm::vec3 rgb_white = { 1.0f, 1.0f, 1.0f };
	glm::vec4 rgba_white = { 1.0f, 1.0f, 1.0f, 1.0f };

	try {
		window->update_projection_matrix();
		glViewport(0, 0, window->getWidth(), window->getHeight());

		camera.Position = glm::vec3(0, 0, 10);
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

			shader.activate();
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 view = glm::mat4(1.0f);
			view = camera.GetViewMatrix();

			//glm::mat4 projection = glm::mat4(1.0f);
			//projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
			
			shader.setUniform("projection", window->getProjection());
			shader.setUniform("transform", trans);
			shader.setUniform("view", view);
			shader.setUniform("ambient_material", rgb_orange);
			shader.setUniform("diffuse_material", rgb_orange);
			shader.setUniform("specular_material", rgb_white);
			shader.setUniform("specular_shinines", 10.0f);

			for (auto& model : scene_test) {
				model.Draw(shader);
			}

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

	//std::filesystem::path model_path("./resources/obj/bunny_tri_vn.obj");
	//std::filesystem::path model_path("./resources/obj/bunny_tri_vnt.obj");
	//std::filesystem::path model_path("./resources/obj/cube_triangles.obj");
	//std::filesystem::path model_path("./resources/obj/cube_triangles_normals_tex.obj");
	//std::filesystem::path model_path("./resources/obj/plane_tri_vnt.obj");
	std::filesystem::path model_path("./resources/obj/sphere_tri_vnt.obj");
	//std::filesystem::path model_path("./resources/obj/teapot_tri_vnt.obj");
	Model my_model{ model_path };

	scene_test.push_back(my_model);
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

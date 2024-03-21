
#include "App.h"
#include "Model.h"

int xpos, ypos, width, height;
void getString(const std::string name, GLenum symb_const) {
	const char* mystring = (const char*)glGetString(symb_const);
	std::cout << name << ": " << mystring << '\n';
}

void getInt(const std::string& name, GLenum pname) {
	GLint myInt;
	glGetIntegerv(pname, &myInt);
	std::cout << name << ": " << myInt << '\n';
}


App::App()
{
	// default constructor
	// nothing to do here (so far...)
	std::cout << "Constructed...\n";
}

void App::error_callback(int error, const char* description) {
	std::cerr << "Error: " << description << std::endl;
}


void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
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
void window_iconify_callback(GLFWwindow* window, int iconified) {
	if (iconified) {
		std::cout << "Window was iconified." << std::endl;
	}
	else {
		std::cout << "Window was opened." << std::endl;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	std::cout << "Current size: " <<width<< "x" <<height<< std::endl;
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	std::cout << "?"<<std::ends;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
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


void App::init_assets()
{
	// load models, load textures, load shaders, initialize level, etc...
	std::filesystem::path VS_path("./resources/basic/basic.vert");
	std::filesystem::path FS_path("./resources/basic/basic.frag");
	shader = ShaderProgram(VS_path, FS_path);

	//std::filesystem::path model_path("./resources/obj/bunny_tri_vn.obj");
	//std::filesystem::path model_path("./resources/obj/bunny_tri_vnt.obj");
	//std::filesystem::path model_path("./resources/obj/cube_triangles.obj");
	//std::filesystem::path model_path("./resources/obj/cube_triangles_normals_tex.obj");
	//std::filesystem::path model_path("./resources/obj/plane_tri_vnt.obj");
	std::filesystem::path model_path("./resources/obj/sphere_tri_vnt.obj");
	//std::filesystem::path model_path("./resources/obj/teapot_tri_vnt.obj");
	Model my_model{ model_path };

	scene.insert({ "obj_test", my_model });
}

void App::report() {
	GLint contextProfile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfile);
	getString("Version", GL_VERSION);
	getString("Vendor", GL_VENDOR);
	getString("Renderer", GL_RENDERER);
	getString("Shading version", GL_SHADING_LANGUAGE_VERSION);
	getInt("Context profile mask", GL_CONTEXT_PROFILE_MASK);
	//getInt("Context compatibility", GL_CONTEXT_COMPATIBILITY_PROFILE_BIT);
	getInt("Context core profile bit", GL_CONTEXT_CORE_PROFILE_BIT);

	if (contextProfile & GL_CONTEXT_CORE_PROFILE_BIT)
		std::cout << "Context is core profile" << '\n';
	else if (contextProfile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		std::cout << "Context is compatibility profile." << '\n';
	else
		std::cout << "Context has no profile." << '\n';
}

bool App::init()
{
	try {
		glfwSetErrorCallback(error_callback);

		// https://www.glfw.org/documentation.html
		if (!glfwInit())
			return false;

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwSwapInterval(1);
		// open window (GL canvas) with no special properties
		// https://www.glfw.org/docs/latest/quick.html#quick_create_window
		window = glfwCreateWindow(800, 600, "OpenGL context", NULL, NULL);
		if (!window) {
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		glfwSetWindowIconifyCallback(window, window_iconify_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, key_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_pos_callback);

		// init glew
		// http://glew.sourceforge.net/basic.html
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			//glewInit failed, something is wrong
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

		wglewInit();
	}
	catch (std::exception const& e) {
		std::cerr << "Init failed : " << e.what() << std::endl;
		throw;
	}

	glfwGetWindowPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &width, &height);

	std::cout << "Initialized...\n";

	report();

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


	return true;
}

int App::run(void)
{
	double previousTime = glfwGetTime();
	int nbFrames = 0;
	double elapsedTime;
	try {
		glm::vec4 my_rgba = { 0.0f, 1.0f, 0.0f, 1.0f };
		while (!glfwWindowShouldClose(window))
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Swap front and back buffers
			glfwSwapBuffers(window);

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
	if (window)
		glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
	std::cout << "Bye...\n";
}



void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto this_inst = static_cast<App*>(glfwGetWindowUserPointer(window));
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_V:
			if (this_inst->getVSyncState()) {
				glfwSwapInterval(0);
				std::cout << "Vsync Off" << std::endl;
				this_inst->setVSyncState(false);
			}
			else {
				glfwSwapInterval(1);
				std::cout << "Vsync On" << std::endl;
				this_inst->setVSyncState(true);
			}
			break;
		case GLFW_KEY_F:
			if (this_inst->isFullscreen()) {
				glfwSetWindowMonitor(window, nullptr, xpos, ypos, width, height, 0);
				std::cout << "Windowed" << std::endl;
			}
			else {
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
				std::cout << "Fullscreen" << std::endl;
				
			}
			break;
		default:
			break;
		}
	}
}

